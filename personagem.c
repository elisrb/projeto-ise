#include "personagem.h"
#include "sprites/sprites.h"
#include "perifericos.h"

void start_player(Jogador *player, int dx, int dy, int dir) {
    player->x = dx;
    player->y = dy;
    player->direcao = dir;
    player->movendo = 0;
    player->frame_atual = 1;
    player->timer_animacao = 0;
    player->timer_movimento = 0;
    player->movendo_anterior = 0; 
}

void atualizar_animacao_jogador(Jogador *player) {
    if (player->movendo) {

        if (!player->movendo_anterior) {
            if(player->direcao == ESQUERDA || player->direcao == DIREITA){
                player->frame_atual = 1;
            }
            else{
                player->frame_atual = 0;
            }
            player->timer_animacao = 0;
        } else {
            player->timer_animacao++;
            if (player->timer_animacao >= 10) {
                player->timer_animacao = 0;
                player->frame_atual = (player->frame_atual + 1) % 4;
            }
        }
    } else {
        player->timer_animacao = 0;
        if (player->direcao == ESQUERDA || player->direcao == DIREITA) {
            player->frame_atual = 0;
        } else {
            // Para Cima e Baixo, mantém o frame 1 parado
            player->frame_atual = 1; 
        }
    }

    player->movendo_anterior = player->movendo;
}


void desenhar_jogador(int camera_x, int camera_y, const Jogador *player) {
    int tela_x = player->x - camera_x;
    int tela_y = player->y - camera_y;

    const unsigned short (*sprite_atual)[16] = SPRITES_RED[player->direcao][player->frame_atual];

    for (int y = 0; y < SPRITE_TAMANHO; y++) {
        for (int x = 0; x < SPRITE_TAMANHO; x++) {
            unsigned short cor_pixel = sprite_atual[y][x];

            if (cor_pixel != 0xFBE5) {
                write_pixel(tela_x + x, tela_y + y, cor_pixel);
            }
        }
    }
}

void mover_jogador(Jogador *player, unsigned char tecla) {
    switch (tecla) {
        case 0x1D: case 0x75: // W ou seta CIMA
            player->direcao = CIMA;
            player->y -= VELOCIDADE;
            player->movendo = 1;
            break;

        case 0x1B: case 0x72: // S ou seta BAIXO
            player->direcao = BAIXO;
            player->y += VELOCIDADE;
            player->movendo = 1;
            break;

        case 0x1C: case 0x6B: // A ou seta ESQUERDA
            player->direcao = ESQUERDA;
            player->x -= VELOCIDADE;
            player->movendo = 1;
            break;

        case 0x23: case 0x74: // D ou seta DIREITA
            player->direcao = DIREITA;
            player->x += VELOCIDADE;
            player->movendo = 1;
            break;

        default:

            player->movendo = 0;
            break;
    }
}