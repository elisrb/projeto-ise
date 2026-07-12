#ifndef PERIFERICOS_SDL_H
#define PERIFERICOS_SDL_H

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
#define F_TR        -1   /* cor "transparente", write_pixel ignora */

/* Cor padrão usada pelo clear() */
extern short SISTEM_STANDARD_COLOR;
#define SSC SISTEM_STANDARD_COLOR

/* ===================== Ciclo de vida ===================== */

/* Inicializa a janela SDL2, o renderer e a textura VGA (RGB565).
 * Retorna 0 em sucesso, -1 em erro. */
int  hw_init(void);

/* Destroi a janela/renderer/textura e finaliza o SDL2. */
void hw_cleanup(void);

/* ===================== VGA / vídeo ===================== */
void inicializar_double_buffering(void);
void inverter_buffers(void);

void write_pixel(int x, int y, short cor);
void clear(void);

/* Sem fonte de verdade na simulação -- imprime no terminal pra debug. */
void write_char(int x, int y, char c);
void write_text(int x, int y, char *text);

/* ===================== Teclado PS/2 (emulado) ===================== */

/* Retorna o scancode PS/2 de uma tecla pressionada AGORA (reflete o
 * estado atual do teclado -- não consome, não depende de "toque novo").
 * Chamar todo frame dá movimento contínuo; 0 se nada estiver ativo. */
unsigned char keyboard_input(void);

/* Mantido por compatibilidade de nome com o resto do código -- agora é
 * só um alias de keyboard_input(). */
unsigned char keyboard_input_filtrado(void);

/* ===================== Timing ===================== */

/* Espera 'ms' milissegundos via SDL_Delay. */
void delay(uint32_t ms);

#endif /* PERIFERICOS_SDL_H */