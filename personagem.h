#ifndef JOGADOR_H
#define JOGADOR_H

#define SPRITE_TAMANHO 16

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

#endif /* JOGADOR_H */