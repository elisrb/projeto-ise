#include "perifericos_sdl.h"
#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ===================== Constantes do Hardware Virtual ===================== */
#define LARGURA_VGA 320
#define ALTURA_VGA  240
#define ESCALA_TELA 2  // Aumenta a janela para 640x480 para ficar mais legível no PC

/* ===================== Estado Interno do SDL2 ===================== */
static SDL_Window   *window   = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture  *vga_texture = NULL;

// Buffers locais simulando a memória da placa (Front e Back buffer para Double Buffering)
static uint16_t pixel_buffer_1[ALTURA_VGA][LARGURA_VGA];
static uint16_t pixel_buffer_2[ALTURA_VGA][LARGURA_VGA];

// Ponteiros para gerenciar qual buffer está sendo desenhado (Back) e qual está fixado (Front)
static uint16_t (*vga_desenho)[LARGURA_VGA] = pixel_buffer_1;
static uint16_t (*vga_exibicao)[LARGURA_VGA] = pixel_buffer_2;

// Registrador virtual simples para o teclado PS/2
static volatile int ps2_virtual_reg = 0;

short SISTEM_STANDARD_COLOR = COR_PRETO;

/* ===================== Funções de Inicialização e Controle ===================== */

int hw_init(void) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow("Simulador DE1-SoC - VGA & PS/2",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              LARGURA_VGA * ESCALA_TELA, ALTURA_VGA * ESCALA_TELA,
                              SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erro ao criar janela: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("Erro ao criar renderizador: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Cria uma textura de 16-bits (RGB565 ou similar) correspondente ao formato de cor da DE1-SoC
    vga_texture = SDL_CreateTexture(renderer,
                                    SDL_PIXELFORMAT_RGB565,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    LARGURA_VGA, ALTURA_VGA);
    if (!vga_texture) {
        printf("Erro ao criar textura VGA: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Inicializa os buffers com a cor preta
    memset(pixel_buffer_1, 0, sizeof(pixel_buffer_1));
    memset(pixel_buffer_2, 0, sizeof(pixel_buffer_2));

    printf("Simulador DE1-SoC inicializado com sucesso!\n");
    return 0;
}

void hw_cleanup(void) {
    if (vga_texture) SDL_DestroyTexture(vga_texture);
    if (renderer)    SDL_DestroyRenderer(renderer);
    if (window)      SDL_DestroyWindow(window);
    SDL_Quit();
    printf("Simulador encerrado.\n");
}

/* ===================== Mecanismo de Atualização da Janela ===================== */

// Processa eventos do Windows (fechar janela, teclas digitadas) e apresenta o
// buffer atual na tela. Só é chamada de dentro deste arquivo (inverter_buffers
// e inicializar_double_buffering) para garantir um único present() por frame.
static void atualizar_simulador(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            hw_cleanup();
            exit(0);
        }

        // Ignora auto-repeat do SO (tecla segurada gerando SDL_KEYDOWN repetido)
        if (event.type == SDL_KEYDOWN && !event.key.repeat) {
            unsigned char scancode = 0;

            // Mapeamento pros codigos PS/2 Set 2 reais -- os MESMOS que a
            // DE1-SoC gera de verdade, pra jogo se comportar igual nos dois
            switch (event.key.keysym.sym) {
                case SDLK_UP:     scancode = 0x75; break;
                case SDLK_DOWN:   scancode = 0x72; break;
                case SDLK_LEFT:   scancode = 0x6B; break;
                case SDLK_RIGHT:  scancode = 0x74; break;
                case SDLK_SPACE:  scancode = 0x29; break;
                case SDLK_RETURN: scancode = 0x5A; break;
                case SDLK_ESCAPE: scancode = 0x76; break;
                case SDLK_w:      scancode = 0x1D; break;
                case SDLK_a:      scancode = 0x1C; break;
                case SDLK_s:      scancode = 0x1B; break;
                case SDLK_d:      scancode = 0x23; break;
                default: break;
            }

            if (scancode != 0) {
                // Bit 15 em 1 indica dado pronto (simulando o comportamento do registrador da DE1-SoC)
                ps2_virtual_reg = 0x8000 | scancode;
            }
        }
    }

    // Atualiza os pixels da janela baseando-se no buffer fixado (vga_exibicao)
    SDL_UpdateTexture(vga_texture, NULL, vga_exibicao, LARGURA_VGA * sizeof(uint16_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, vga_texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

/* ===================== VGA / Vídeo Emulado ===================== */

void inicializar_double_buffering(void) {
    // No simulador, apenas garante os ponteiros apontando para locais distintos
    vga_desenho = pixel_buffer_1;
    vga_exibicao = pixel_buffer_2;
    atualizar_simulador();
}

void inverter_buffers(void) {
    // Troca o ponteiro do buffer de desenho com o de exibição
    uint16_t (*temp)[LARGURA_VGA] = vga_desenho;
    vga_desenho = vga_exibicao;
    vga_exibicao = temp;

    // Renderiza o novo frame na tela e processa as entradas -- único
    // ponto do arquivo que faz poll de evento / present
    atualizar_simulador();
}

void write_pixel(int x, int y, short cor) {
    if (x >= 0 && x < LARGURA_VGA && y >= 0 && y < ALTURA_VGA && cor != F_TR) {
        vga_desenho[y][x] = (uint16_t)cor;
    }
}

void clear(void) {
    for (int y = 0; y < ALTURA_VGA; y++) {
        for (int x = 0; x < LARGURA_VGA; x++) {
            write_pixel(x, y, SSC);
        }
    }
}

// Opcional: Como o terminal de texto da FPGA (Char Base) usa fontes nativas de hardware,
// no PC você pode usar o terminal padrão para debug ou estender via SDL2_ttf se preferir.
void write_char(int x, int y, char c) {
    // Redireciona de forma simples para o terminal para você não perder informações
    printf("[VGA Char Text (%d,%d)]: %c\n", x, y, c);
}

void write_text(int x, int y, char *text) {
    printf("[VGA Text (%d,%d)]: %s\n", x, y, text);
}

/* ===================== Teclado PS/2 Emulado ===================== */

unsigned char keyboard_input(void) {
    int dados = ps2_virtual_reg;
    if (dados & 0x8000) {
        ps2_virtual_reg = 0; // Consome o dado lido (Zera o bit de pronto)
        return (unsigned char)(dados & 0xFF);
    }
    return 0;
}

// OBS: diferente da versão que roda na DE1-SoC de verdade, esta função NÃO
// faz filtragem de break code (0xF0) nem prefixo estendido (0xE0) -- o
// mapeamento de teclas já devolve o código final direto no switch acima,
// então não existe stream bruto pra filtrar aqui. O nome foi mantido só
// por compatibilidade com o resto do código que já chama essa função.
unsigned char keyboard_input_filtrado(void) {
    return keyboard_input();
}

/* ===================== Timing ===================== */

void delay(uint32_t ms) {
    if (ms == 0) return;
    SDL_Delay(ms); // Usa o delay nativo e preciso do SDL
}