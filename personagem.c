#include "personagem.h"
#include "sprites/sprites.h"
#include "perifericos.h"
#include "sprites/fundos.h"
#include "sprites/colisao.h"
#include <stdlib.h>
//#include<stdio.h> // para o printf de debug

int camera_x = 0;
int camera_y = 0;

Cenario *cenario_atual;

void start_player(Jogador *player, int dx, int dy, int dir) {
    // arredonda pro múltiplo de 16 mais próximo
    player->x = (dx / 16) * 16;
    player->y = (dy / 16) * 16;
    player->destino_x = player->x;
    player->destino_y = player->y;

    player->direcao = dir;
    player->movendo = 0;
    player->frame_atual = 1;
    player->timer_animacao = 0;
    player->timer_movimento = 0;
    player->movendo_anterior = 0;
}

void atualizar_animacao_jogador(Jogador *player) {
    if (player->movendo) {
        // 1. Calcula a distância restante nos dois eixos
        int dist_x = abs(player->destino_x - player->x);
        int dist_y = abs(player->destino_y - player->y);
        
        // Pega a maior distância (já que ele só anda em um eixo por vez)
        int dist_restante = (dist_x > dist_y) ? dist_x : dist_y;
        
        // Transforma a distância restante em progresso dentro do tile (0 a 16 pixels)
        int progresso = 16 - dist_restante;

        // ==========================================
        // ANIMAÇÃO VERTICAL (CIMA / BAIXO)
        // ==========================================
        if (player->direcao == CIMA || player->direcao == BAIXO) {
            if (progresso < 4 || progresso > 12) {
                player->frame_atual = 1; // Pés Juntos (frente/costas)
            } else {
                // Descobre a paridade do tile para alternar o pé que inicia
                int tile_x = player->destino_x / 16;
                int tile_y = player->destino_y / 16;
                int paridade = (tile_x + tile_y) % 2;
                player->frame_atual = (paridade == 0) ? 0 : 2;
            }
        }
        // ==========================================
        // ANIMAÇÃO HORIZONTAL (ESQUERDA / DIREITA) - CICLO DE 3 PASSOS POR TILE
        // ==========================================
        else {
            if (progresso <= 4) {
                player->frame_atual = 0; // 1º tempo: Pés juntos (início do movimento)
            } 
            else if (progresso > 4 && progresso <= 10) {
                player->frame_atual = 1; // 2º tempo: Pé esquerdo (meio do movimento)
            } 
            else {
                player->frame_atual = 2; // 3º tempo: Pé direito (final do movimento)
            }
        }

    } else {
        // ==========================================
        // JOGADOR TOTALMENTE PARADO
        // ==========================================
        if (player->direcao == ESQUERDA || player->direcao == DIREITA) {
            player->frame_atual = 0; // Pés juntos de lado é o frame 0
        } else {
            player->frame_atual = 1; // Pés juntos de frente/costas é o frame 1
        }
    }

    // Guarda o estado para o próximo frame
    player->movendo_anterior = player->movendo;
}

void desenhar_jogador(int camera_x, int camera_y, const Jogador *player) {
    int tela_x = player->x - camera_x + OFFSET_X;
    int tela_y = player->y - camera_y + OFFSET_Y + OFFSET_VISUAL_Y;

    if (tela_x >= OFFSET_X - SPRITE_TAMANHO && tela_x < (OFFSET_X + CAM_LARGURA) &&
        tela_y >= OFFSET_Y - SPRITE_TAMANHO && tela_y < (OFFSET_Y + CAM_ALTURA)) {

        // CORREÇÃO AQUI: Declara como um ponteiro para um array de 16 elementos
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
}

void mover_jogador(Jogador *player, unsigned char tecla) {
    if (!cenario_atual) return;

    // Se já está no meio de um passo, só continua avançando na direção
    // do destino -- ignora tecla nova até completar o tile atual
    if (player->movendo) {
        if (player->x < player->destino_x) player->x += VELOCIDADE;
        if (player->x > player->destino_x) player->x -= VELOCIDADE;
        if (player->y < player->destino_y) player->y += VELOCIDADE;
        if (player->y > player->destino_y) player->y -= VELOCIDADE;

        if (player->x == player->destino_x && player->y == player->destino_y) {
            player->movendo = 0;
        }
        return;
    }

    int dx = 0, dy = 0;
    switch (tecla) {
        case 0x1D: case 0x75: player->direcao = CIMA;      dy = -16; break;
        case 0x1B: case 0x72: player->direcao = BAIXO;     dy =  16; break;
        case 0x1C: case 0x6B: player->direcao = ESQUERDA;  dx = -16; break;
        case 0x23: case 0x74: player->direcao = DIREITA;   dx =  16; break;
        default:
            return; // nenhuma tecla -- não se move, interrompe a função
    }

    int prox_x = player->x + dx;
    int prox_y = player->y + dy;

    Terreno resultado_terreno = checar_colisao(prox_x, prox_y);

    switch(resultado_terreno) {
        case LIVRE:
            //printf("LIVRE\n");
            player->destino_x = prox_x;
            player->destino_y = prox_y;
            player->movendo = 1;
            break;

        case GRAMA:
            //printf("GRAMA\n");
            // sorteio de combate aleatório, se não:
            player->destino_x = prox_x;
            player->destino_y = prox_y;
            player->movendo = 1;
            break;

        case PORTA:
            //printf("PORTA\n");

            /*// loop inteiro de desenhar o próximo frame antes de entrar na porta
            atualizar_camera(prox_x, prox_y);
            atualizar_animacao_jogador(player);
            desenhar_cenario();
            desenhar_jogador(camera_x, camera_y, player);
            inverter_buffers();
            delay(16);*/

            for (int i = 0; i < cenario_atual->qtd_portas; i++) {
                Porta porta_teste = cenario_atual->portas[i];
                //printf("Checando porta %d: (%d, %d) = (%d, %d)\n", i, porta_teste.x, porta_teste.y, prox_x/16, prox_y/16);
                if (prox_x/16 == porta_teste.x && prox_y/16 == porta_teste.y) {
                    //printf("Encontrada porta para %p (%d, %d)\n", porta_teste.destino, porta_teste.novo_x, porta_teste.novo_y);
                    delay(100);
                    cenario_atual = porta_teste.destino;
                    player->x = (porta_teste.novo_x)*16;
                    player->y = (porta_teste.novo_y)*16;
                    player->direcao = BAIXO;
                    player->movendo = 0;
                    break;
                }
            }
            break;
        
        case OBSTACULO:
            //printf("OBSTACULO\n");
            // não faz nada
            break;
    }
}

void atualizar_camera(int jogador_x, int jogador_y) {
    if (!cenario_atual) return;

    // 1. Centraliza a janela dinâmica no Red
    camera_x = jogador_x - (CAM_LARGURA / 2) + 8;
    camera_y = jogador_y - (CAM_ALTURA / 2) + 8;

    // 2. Trava no topo/esquerda
    if (camera_x < 0) camera_x = 0;
    if (camera_y < 0) camera_y = 0;

    // 3. Trava na direita (se o cenário for menor que a janela, fixa em 0)
    if (cenario_atual->largura < CAM_LARGURA) {
        camera_x = 0;
    } else if (camera_x > (cenario_atual->largura - CAM_LARGURA)) {
        camera_x = cenario_atual->largura - CAM_LARGURA;
    }

    // 4. Trava embaixo (se o cenário for mais baixo que a janela, fixa em 0)
    if (cenario_atual->altura < CAM_ALTURA) {
        camera_y = 0;
    } else if (camera_y > (cenario_atual->altura - CAM_ALTURA)) {
        camera_y = cenario_atual->altura - CAM_ALTURA;
    }
}

void desenhar_cenario() {
    if (!cenario_atual || !cenario_atual->fundo) return;

    int mapa_largura = cenario_atual->largura;
    int mapa_altura = cenario_atual->altura;

    for (int y = 0; y < TELA_ALTURA; y++) {
        for (int x = 0; x < TELA_LARGURA; x++) {
            
            // Verifica dinamicamente se o pixel está dentro dos limites da janela configurada
            if (x >= OFFSET_X && x < (OFFSET_X + CAM_LARGURA) &&
                y >= OFFSET_Y && y < (OFFSET_Y + CAM_ALTURA)) {
                
                // Mapeia a posição da tela de volta para o mundo
                int mundo_x = camera_x + (x - OFFSET_X);
                int mundo_y = camera_y + (y - OFFSET_Y);

                // Proteção para cenários que são menores que a própria janela do jogo
                if (mundo_x >= mapa_largura || mundo_y >= mapa_altura) {
                    write_pixel(x, y, 0x0000); 
                } else {
                    unsigned short cor = cenario_atual->fundo[mundo_y * mapa_largura + mundo_x];
                    write_pixel(x, y, cor);
                }
            } else {
                // Desenha a borda preta na região que sobrou
                write_pixel(x, y, 0x0000);
            }
        }
    }
}

int checar_colisao(int prox_x, int prox_y) {
    if (!cenario_atual || !cenario_atual->mapa_colisao) return OBSTACULO;

    // converte a posição do personagem e o tamanho do cenário em blocos
    int colunas_de_tiles = cenario_atual->largura / 16;
    int linhas_de_tiles = cenario_atual->altura / 16;
    int tile_x = prox_x / 16;
    int tile_y = prox_y / 16;

    // impede o jogador de passar da borda
    if (tile_x < 0 || tile_x >= colunas_de_tiles || 
        tile_y < 0 || tile_y >= linhas_de_tiles) {
        return OBSTACULO; 
    }

    // retorna o bloco correspondente do mapa de colisão
    int indice = tile_y * colunas_de_tiles + tile_x;    
    return cenario_atual->mapa_colisao[indice];
}