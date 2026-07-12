#include ""

#define SPRITE_TAMANHO 16

typedef enum {
    CIMA = 0,
    DIREITA = 1,
    BAIXO = 2,
    ESQUERDA = 3
} Direcao;

typedef struct{

    int x;
    int y;

    Direcao direcao;
    int movendo;

    int frame_atual;    //frame atual do personagem
    int timer_animacao; //tempo para o pulo da imagem

}Jogador;

void start_player(int dx, int dy, int dir){
    Jogador player;

    player.x = dx;
    player.y = dy;
    player.direcao =  dir;
    player.movendo = 0;
    player.frame_atual = 0;
    player.timer_animacao = 0;

}

void atualizar_animacao_jogador(Jogador player) {
    if (player.movendo) {
        player.timer_animacao++;
        
        if (player.timer_animacao >= 10) { 
            player.timer_animacao = 0;
            
            player.frame_atual = (player.frame_atual + 1) % 3;
        }
    } else {
        player.frame_atual = 1; 
        player.timer_animacao = 0;
    }
}

void desenhar_jogador(int camera_x, int camera_y, Jogador player) {
    int tela_x = player.x - camera_x;
    int tela_y = player.y - camera_y;

    const unsigned short *sprite_atual = SPRITES_RED[player.direcao][player.frame_atual];

    for (int y = 0; y < SPRITE_TAMANHO; y++) {
        for (int x = 0; x < SPRITE_TAMANHO; x++) {
            
            unsigned short cor_pixel = sprite_atual[y * SPRITE_TAMANHO + x];
            
            if (cor_pixel != 0xFBE5) {
                write_pixel(tela_x + x, tela_y + y, cor_pixel);
            }
        }
    }
}