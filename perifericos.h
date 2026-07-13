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
#define F_TR        0xFBE5   /* cor "transparente", write_pixel ignora */

/* Cor padrão usada pelo clear() */
extern short SISTEM_STANDARD_COLOR;
#define SSC SISTEM_STANDARD_COLOR

/* ===================== Ciclo de vida ===================== */

/* Abre /dev/mem e mapeia todos os periféricos necessários.
 * Retorna 0 em sucesso, -1 em erro (confira perror/stderr). */
int  hw_init(void);

/* Desfaz os mmaps e fecha /dev/mem. Chamar antes de sair do programa. */
void hw_cleanup(void);

/* ===================== VGA / vídeo ===================== */
void inicializar_double_buffering(void);
void inverter_buffers(void);

void write_pixel(int x, int y, short cor);
void clear(void);

void write_char(int x, int y, char c);
void write_text(int x, int y, char *text);

/* ===================== Teclado PS/2 ===================== */

/* Retorna o byte bruto recebido (0 se não houver dado novo).
 * Não filtra break codes (0xF0) nem extended (0xE0). */
unsigned char keyboard_input(void);

/* Consome o FIFO do PS/2 e retorna o último scancode "make" válido,
 * ignorando break codes (0xF0) e o prefixo extended (0xE0). */
unsigned char keyboard_input_filtrado(void);

int checar_colisao(int prox_x, int prox_y);

/* ===================== Timing ===================== */

/* Espera 'ms' milissegundos. Implementado com usleep (sem busy-wait
 * em registrador de timer, compatível com espaço de usuário Linux). */
void delay(uint32_t ms);

#endif /* PERIPHERALS_H */