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

// Estado continuo de teclas -- indexado pelo scancode PS/2 que a gente
// mesmo definiu (0x00 a 0xFF). 1 = pressionada agora, 0 = solta.
static int tecla_pressionada[256] = {0};

short SISTEM_STANDARD_COLOR = COR_PRETO;

/* Mapeamento SDL_Keycode -> scancode PS/2 Set 2 (os MESMOS codigos que
 * a DE1-SoC gera de verdade, pro jogo se comportar igual nos dois
 * ambientes). Usado tanto pro evento de tecla pressionada quanto solta. */
static unsigned char mapear_scancode(SDL_Keycode sym) {
    switch (sym) {
        case SDLK_UP:     return 0x75;
        case SDLK_DOWN:   return 0x72;
        case SDLK_LEFT:   return 0x6B;
        case SDLK_RIGHT:  return 0x74;
        case SDLK_SPACE:  return 0x29;
        case SDLK_RETURN: return 0x5A;
        case SDLK_ESCAPE: return 0x76;
        case SDLK_w:      return 0x1D;
        case SDLK_a:      return 0x1C;
        case SDLK_s:      return 0x1B;
        case SDLK_d:      return 0x23;
        default:          return 0x00;
    }
}

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

static void atualizar_simulador(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            hw_cleanup();
            exit(0);
        }

        // Ignora auto-repeat do SO (tecla segurada gerando SDL_KEYDOWN repetido)
        if (event.type == SDL_KEYDOWN && !event.key.repeat) {
            unsigned char scancode = mapear_scancode(event.key.keysym.sym);
            if (scancode != 0) {
                tecla_pressionada[scancode] = 1;
            }
        }

        // Quando a tecla eh solta, zera o estado -- e o que permite
        // keyboard_input() saber que a tecla nao esta mais ativa
        if (event.type == SDL_KEYUP) {
            unsigned char scancode = mapear_scancode(event.key.keysym.sym);
            if (scancode != 0) {
                tecla_pressionada[scancode] = 0;
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


void write_char(int x, int y, char c) {
    // Redireciona de forma simples para o terminal para você não perder informações
    printf("[VGA Char Text (%d,%d)]: %c\n", x, y, c);
}

void write_text(int x, int y, char *text) {
    printf("[VGA Text (%d,%d)]: %s\n", x, y, text);
}

/* ===================== Teclado PS/2 Emulado ===================== */

unsigned char keyboard_input(void) {
    static const unsigned char ordem_prioridade[] = {
        0x75, 0x1D, // cima (seta / W)
        0x72, 0x1B, // baixo (seta / S)
        0x6B, 0x1C, // esquerda (seta / A)
        0x74, 0x23, // direita (seta / D)
        0x29,       // espaco
        0x5A,       // enter
        0x76,       // esc
    };

    for (size_t i = 0; i < sizeof(ordem_prioridade); i++) {
        unsigned char codigo = ordem_prioridade[i];
        if (tecla_pressionada[codigo]) {
            return codigo;
        }
    }
    return 0;
}

unsigned char keyboard_input_filtrado(void) {
    return keyboard_input();
}

/* ===================== Timing ===================== */

void delay(uint32_t ms) {
    if (ms == 0) return;
    SDL_Delay(ms); // Usa o delay nativo e preciso do SDL
}