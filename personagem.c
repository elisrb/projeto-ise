#include "personagem.h"
#include "sprites/sprites.h"
#include "perifericos.h"
#include "sprites/fundos.h"
#include "sprites/colisao.h"


int camera_x = 0;
int camera_y = 0;

struct Cenario *cenario_atual;
const unsigned short *pixels_cenario_atual;


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
    int tela_x = player->x - camera_x + OFFSET_X;
    int tela_y = player->y - camera_y + OFFSET_Y;

    if (tela_x >= OFFSET_X - SPRITE_TAMANHO && tela_x < (OFFSET_X + CAM_LARGURA) &&
        tela_y >= OFFSET_Y - SPRITE_TAMANHO && tela_y < (OFFSET_Y + CAM_ALTURA)) {

        // CORREÇÃO AQUI: Declara como um ponteiro para um array de 16 elementos
        const unsigned short (*sprite_atual)[16] = SPRITES_RED[player->direcao][player->frame_atual];

        for (int y = 0; y < SPRITE_TAMANHO; y++) {
            for (int x = 0; x < SPRITE_TAMANHO; x++) {
                // CORREÇÃO AQUI: Acessa como matriz bidimensional [y][x]
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

    int prox_x = player->x;
    int prox_y = player->y;
    player->movendo = 1;

    switch (tecla) {
        case 0x1D: case 0x75: // W / CIMA
            player->direcao = CIMA;
            prox_y -= VELOCIDADE;
            break;

        case 0x1B: case 0x72: // S / BAIXO
            player->direcao = BAIXO;
            prox_y += VELOCIDADE;
            break;

        case 0x1C: case 0x6B: // A / ESQUERDA
            player->direcao = ESQUERDA;
            prox_x -= VELOCIDADE;
            break;

        case 0x23: case 0x74: // D / DIREITA
            player->direcao = DIREITA;
            prox_x += VELOCIDADE;
            break;

        default:
            player->movendo = 0;
            return; // Nenhuma tecla válida, sai sem fazer nada
    }

    // --- CHECAGEM DE TERRENO ---
    enum Terreno resultado_terreno = checar_colisao(prox_x, prox_y);

    if (resultado_terreno != OBSTACULO) {
        // Movimento livre permitido! Atualiza a posição real
        player->x = prox_x;
        player->y = prox_y;

        if (resultado_terreno == GRAMA) {
            // Aqui você pode colocar uma lógica futura de sorteio (ex: rand() % 100)
            // para disparar batalhas contra Pokémons selvagens!
        }
    }
}

void carregar_cenario(struct Cenario *novo_cenario, const unsigned short *novos_pixels) {
    cenario_atual = novo_cenario;
    pixels_cenario_atual = novos_pixels;
    
    // Opcional: reposicionar o Red dependendo de onde ele está entrando
    // player.x = ...
    // player.y = ...
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
    if (!cenario_atual || !pixels_cenario_atual) return;

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
                    unsigned short cor = pixels_cenario_atual[mundo_y * mapa_largura + mundo_x];
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

    // Calcula a largura em número de blocos (tiles) para a matemática de índice linear
    int colunas_de_tiles = cenario_atual->largura / 16;
    int linhas_de_tiles = cenario_atual->altura / 16;

    // Pontos do Sprite de 16x16 para testar colisão (Cantos)
    // Subtraímos 1 pixel na direita/baixo para não ler o bloco seguinte sem querer
    int pontos_x[2] = { prox_x, prox_x + 15 };
    int pontos_y[2] = { prox_y, prox_y + 15 };

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int tile_x = pontos_x[i] / 16;
            int tile_y = pontos_y[j] / 16;

            // 1. Proteção de bordas do mapa
            if (tile_x < 0 || tile_x >= colunas_de_tiles || 
                tile_y < 0 || tile_y >= linhas_de_tiles) {
                return OBSTACULO; 
            }

            // 2. Mapeia a coordenada da matriz 2D para o array linear
            int indice = tile_y * colunas_de_tiles + tile_x;
            enum Terreno tipo_bloco = cenario_atual->mapa_colisao[indice];

            // 3. Se qualquer um dos cantos encostar em um obstáculo, bloqueia
            if (tipo_bloco == OBSTACULO) {
                return OBSTACULO;
            }
        }
    }

    // Se nenhum canto encostou em obstáculo, podemos verificar se ele está pisando na grama
    // Pegamos o centro do Red para definir se ele de fato está "dentro" da grama
    int centro_tile_x = (prox_x + 8) / 16;
    int centro_tile_y = (prox_y + 8) / 16;
    int indice_centro = centro_tile_y * colunas_de_tiles + centro_tile_x;
    
    return cenario_atual->mapa_colisao[indice_centro];
}