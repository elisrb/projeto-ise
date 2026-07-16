#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include <stdint.h>

/* ===================== Cores (RGB565) ===================== */
#define COR_PRETO   0x0000
#define COR_BRANCO  0xFFFC
#define COR_CURSOR  0xBDF7
#define COR_CINZA   0x8410
#define COR_VERDE   0x0180
#define VERM        0xF800
#define VERM_E      0xB000
#define FOLHA       0x0400
#define ROXO_E      0x480A
#define VERDE_FUNDO 0x1A23
#define VERDE_GRAMA 0x4428
#define ROXO        0x8110
#define AMA2        0xFFE0
#define PT_E        0x0000
#define ROSA        0xFBAE
#define F_TR        0xFBE5   

/* Cor padrão usada pelo clear() */
extern short SISTEM_STANDARD_COLOR;
#define SSC SISTEM_STANDARD_COLOR

/* ===================== Ciclo de vida ===================== */


int  hw_init(void);


void hw_cleanup(void);

/* ===================== VGA / vídeo ===================== */
void inicializar_double_buffering(void);
void inverter_buffers(void);

void write_pixel(int x, int y, short cor);
void clear(void);

void write_char(int x, int y, char c);
void write_text(int x, int y, char *text);

/* ===================== Teclado PS/2 ===================== */


unsigned char keyboard_input(void);


unsigned char keyboard_input_filtrado(void);

/* ===================== Timing ===================== */


void delay(uint32_t ms);

#endif /* PERIPHERALS_H */