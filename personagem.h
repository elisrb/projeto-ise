#ifndef JOGADOR_H
#define JOGADOR_H

#define SPRITE_TAMANHO 16
#define VELOCIDADE 1

typedef enum {
    CIMA = 0,
    DIREITA = 1,
    BAIXO = 2,
    ESQUERDA = 3
} Direcao;

typedef struct {
    int x;
    int y;

    Direcao direcao;
    int movendo;

    int frame_atual;     // frame atual do personagem
    int timer_animacao;  // tempo para o pulo da imagem

    int timer_movimento; // contador de frames desde o último passo
    int movendo_anterior; // estado de 'movendo' no frame passado (detecta transição)
} Jogador;

/* Inicializa 'player' na posição (dx, dy), olhando pra direção 'dir'.
 * Recebe ponteiro porque precisa modificar o Jogador de verdade,
 * não uma cópia local. */
void start_player(Jogador *player, int dx, int dy, int dir);

/* Avança a animação de 'player' (frame_atual/timer_animacao) com
 * base em player->movendo. Chamar uma vez por frame do jogo. */
void atualizar_animacao_jogador(Jogador *player);

/* Desenha o sprite atual de 'player' na tela, já ajustado pela
 * câmera (camera_x, camera_y). */
void desenhar_jogador(int camera_x, int camera_y, const Jogador *player);

/* Atualiza direção/posição de 'player' com base na tecla recebida
 * (scancode PS/2, ex: retorno de keyboard_input_filtrado()). Aceita
 * WASD ou setas. Se 'tecla' não for uma tecla de movimento, para a
 * animação de andar mas mantém a última direção. */
void mover_jogador(Jogador *player, unsigned char tecla);

#endif /* JOGADOR_H */