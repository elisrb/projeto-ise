#include "perifericos.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>

/* ===================== Endereços físicos ===================== */
#define VGA_PIXEL_BUFFER   0xC8000000
#define BUFFER_BACK_SDRAM  0xC0000000
#define FPGA_CHAR_BASE     0xC9000000

#define LW_BRIDGE_BASE     0xFF200000
#define LW_BRIDGE_SPAN     0x00200000  /* 2 MB, cobre PS/2 e o pixel ctrl */

#define VGA_PIXEL_CTRL_OFF 0x00003020
#define PS2_KEYBOARD_OFF   0x00000100

#define SDRAM_SPAN         0x08000000  /* mapeia C0000000 até CFFFFFFF de uma vez */
#define CHAR_SPAN          0x00001000

/* ===================== Estado interno ===================== */
static int dev_mem_fd = -1;

static void *sdram_map    = NULL; /* base = BUFFER_BACK_SDRAM (0xC0000000) */
static void *char_map     = NULL;
static void *lw_bridge_map = NULL;

static volatile short (*vga_char)[128];
static volatile int    *vga_pixel_ctrl;
static volatile int    *ps2_reg;

short SISTEM_STANDARD_COLOR = COR_PRETO;

static volatile short (*vga_desenho)[512];

/* Mapeia 'length' bytes a partir do endereço físico 'phys_addr'. */
static void *map_physical(off_t phys_addr, size_t length) {
    long page_size = sysconf(_SC_PAGESIZE);
    off_t page_base   = phys_addr & ~(off_t)(page_size - 1);
    off_t page_offset = phys_addr - page_base;

    void *map = mmap(NULL, length + page_offset,
                      PROT_READ | PROT_WRITE, MAP_SHARED,
                      dev_mem_fd, page_base);
    if (map == MAP_FAILED) return NULL;

    return (char *)map + page_offset;
}

int hw_init(void) {
    dev_mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (dev_mem_fd < 0) {
        perror("hw_init: open /dev/mem (precisa rodar como root/sudo)");
        return -1;
    }

    sdram_map = map_physical(BUFFER_BACK_SDRAM, SDRAM_SPAN);
    char_map  = map_physical(FPGA_CHAR_BASE, CHAR_SPAN);
    lw_bridge_map = map_physical(LW_BRIDGE_BASE, LW_BRIDGE_SPAN);

    if (!sdram_map || !char_map || !lw_bridge_map) {
        perror("hw_init: mmap");
        hw_cleanup();
        return -1;
    }

    vga_char = (volatile short (*)[128]) char_map;
    vga_pixel_ctrl = (volatile int *) ((char *)lw_bridge_map + VGA_PIXEL_CTRL_OFF);
    ps2_reg = (volatile int *) ((char *)lw_bridge_map + PS2_KEYBOARD_OFF);

    /* vga_desenho começa apontando pro buffer de front até o primeiro swap */
    vga_desenho = (volatile short (*)[512]) sdram_map;

    return 0;
}

void hw_cleanup(void) {
    if (sdram_map)     munmap(sdram_map, SDRAM_SPAN);
    if (char_map)       munmap(char_map, CHAR_SPAN);
    if (lw_bridge_map)  munmap(lw_bridge_map, LW_BRIDGE_SPAN);
    if (dev_mem_fd >= 0) close(dev_mem_fd);

    sdram_map = char_map = lw_bridge_map = NULL;
    dev_mem_fd = -1;
}

/* ===================== VGA / vídeo ===================== */

void inicializar_double_buffering(void) {
    /* offset dentro da janela SDRAM que mapeamos */
    uintptr_t front_off = VGA_PIXEL_BUFFER - BUFFER_BACK_SDRAM;
    uintptr_t back_off  = BUFFER_BACK_SDRAM - BUFFER_BACK_SDRAM;

    *(vga_pixel_ctrl + 1) = (int)((char *)sdram_map + front_off - (char *)sdram_map) + VGA_PIXEL_BUFFER;
    *(vga_pixel_ctrl)     = BUFFER_BACK_SDRAM;

    while ((*(vga_pixel_ctrl + 3) & 0x1) != 0);

    vga_desenho = (volatile short (*)[512]) ((char *)sdram_map + back_off);
}

void inverter_buffers(void) {
    *(vga_pixel_ctrl) = 1;
    while ((*(vga_pixel_ctrl + 3) & 0x1) != 0);

    /* o registrador devolve o endereço FÍSICO do buffer atual;
     * convertemos para o ponteiro virtual dentro do nosso mmap */
    uint32_t buf_fisico = (uint32_t)*(vga_pixel_ctrl);
    vga_desenho = (volatile short (*)[512])
        ((char *)sdram_map + (buf_fisico - BUFFER_BACK_SDRAM));
}

void write_pixel(int x, int y, short cor) {
    if (x >= 0 && x < 320 && y >= 0 && y < 240 && cor != F_TR)
        vga_desenho[y][x] = cor;
}

void clear(void) {
    for (int y = 0; y < 240; y++)
        for (int x = 0; x < 320; x++)
            write_pixel(x, y, SSC);
}

void write_char(int x, int y, char c) {
    if (x >= 0 && x < 80 && y >= 0 && y < 60)
        ((volatile char (*)[128]) vga_char)[y][x] = c;
}

void write_text(int x, int y, char *text) {
    if (x >= 0 && x < 80 && y >= 0 && y < 60)
        for (int i = 0; text[i] != '\0'; i++)
            write_char(x + i, y, text[i]);
}

/* ===================== Teclado PS/2 ===================== */

unsigned char keyboard_input(void) {
    int dados = *ps2_reg;
    return (dados & 0x8000) ? (unsigned char)(dados & 0xFF) : 0;
}

unsigned char keyboard_input_filtrado(void) {
    unsigned char ultimo_valido = 0;
    while (1) {
        int dados = *ps2_reg;
        if (!(dados & 0x8000)) break;

        unsigned char byte = dados & 0xFF;
        if (byte == 0xF0) {
            while (!(*ps2_reg & 0x8000));
            (void)*ps2_reg; /* descarta o código que segue o break code */
        } else if (byte != 0xE0) {
            ultimo_valido = byte;
        }
    }
    return ultimo_valido;
}

/* ===================== Timing ===================== */

void delay(uint32_t ms) {
    if (ms == 0) return;
    usleep((useconds_t)ms * 1000);
}