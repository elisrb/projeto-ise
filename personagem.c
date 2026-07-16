#include "personagem.h"
#include "pokemons.h"
#include "batalha.h"
#include "sprites/sprites.h"
#include "perifericos.h"
#include "sprites/fundos.h"
#include "sprites/colisao.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int camera_x = 0;
int camera_y = 0;

Cenario *cenario_atual;
int batalha_on;

void start_player(Jogador *player, int dx, int dy, int dir) {
    
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
    player->numero_itens = 0;
    player->numero_pokemons = 0;
}

void atualizar_animacao_jogador(Jogador *player) {
    if (player->movendo) {
        
        int dist_x = abs(player->destino_x - player->x);
        int dist_y = abs(player->destino_y - player->y);
        
        
        int dist_restante = (dist_x > dist_y) ? dist_x : dist_y;
        
        
        int progresso = 16 - dist_restante;

        // ==========================================
        // ANIMAÇÃO VERTICAL (CIMA / BAIXO)
        // ==========================================
        if (player->direcao == CIMA || player->direcao == BAIXO) {
            if (progresso < 4 || progresso > 12) {
                player->frame_atual = 1; // Pés Juntos (frente/costas)
            } else {
                
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
                player->frame_atual = 0; 
            } 
            else if (progresso > 4 && progresso <= 10) {
                player->frame_atual = 1; 
            } 
            else {
                player->frame_atual = 2; 
            }
        }

    } else {
        // ==========================================
        // JOGADOR TOTALMENTE PARADO
        // ==========================================
        if (player->direcao == ESQUERDA || player->direcao == DIREITA) {
            player->frame_atual = 0; 
        } else {
            player->frame_atual = 1; 
        }
    }

    
    player->movendo_anterior = player->movendo;
}

void desenhar_jogador(int camera_x, int camera_y, const Jogador *player) {
    int tela_x = player->x - camera_x + OFFSET_X;
    int tela_y = player->y - camera_y + OFFSET_Y + OFFSET_VISUAL_Y;

    if (tela_x >= OFFSET_X - SPRITE_TAMANHO && tela_x < (OFFSET_X + CAM_LARGURA) &&
        tela_y >= OFFSET_Y - SPRITE_TAMANHO && tela_y < (OFFSET_Y + CAM_ALTURA)) {

        
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
            return; 
    }

    int prox_x = player->x + dx;
    int prox_y = player->y + dy;

    Terreno resultado_terreno = checar_colisao(prox_x, prox_y);

    switch(resultado_terreno) {
        case LIVRE:
            
            player->destino_x = prox_x;
            player->destino_y = prox_y;
            player->movendo = 1;
            break;

        case GRAMA:
            
            if (rand() % 100 < 20) { // 20% de chance de batalha
                batalha_on = 1;
            } else {
                player->destino_x = prox_x;
                player->destino_y = prox_y;
                player->movendo = 1;
            }
            break;

        case PORTA:
            

            

            for (int i = 0; i < cenario_atual->qtd_portas; i++) {
                Porta porta_teste = cenario_atual->portas[i];
                
                if (prox_x/16 == porta_teste.x && prox_y/16 == porta_teste.y) {
                    
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

void capturar_pokemon(Jogador *red, Pokemon *pokemon_capturado) {
    if (red->numero_pokemons >= 6) {
        return;
    }

    // Copia o conteúdo apontado por pokemon_capturado para a struct dentro do vetor
    red->pokemons[red->numero_pokemons] = *pokemon_capturado;
    red->numero_pokemons += 1;
}

void pegar_item(Jogador *red, Item *item) {
    for (int i = 0; i < red->numero_itens; i++) {
        if (strcmp(red->bolsa[i].nome, item->nome) == 0) {
            red->bolsa[i].quantidade += item->quantidade;
            return;
        }
    }

    if (red->numero_itens < 10) {
        int novo_slot = red->numero_itens;

        strcpy(red->bolsa[novo_slot].nome, item->nome);
        red->bolsa[novo_slot].quantidade = item->quantidade;
        
        red->numero_itens++;
    }
}

int calcular_tentativa_captura(Pokemon *inimigo) {
    // 1. Calcula a proporção de HP restante do selvagem (de 0.0 a 1.0)
    double proporcao_hp = (double)inimigo->hp_atual / (double)inimigo->hp_max;

    // 2. Calcula a chance base (HP quase zero = 85% de chance | HP cheio = 25% de chance)
    double chance_final = 0.85 - (proporcao_hp * 0.60);

    // 3. Garante limites seguros (entre 10% e 90%) para manter o jogo justo e emocionante
    if (chance_final < 0.10) chance_final = 0.10;
    if (chance_final > 0.90) chance_final = 0.90;

    // 4. Roda o sorteio (rand() % 100 gera de 0 a 99)
    int numero_sorteado = rand() % 100;
    int alvo_necessario = (int)(chance_final * 100.0);

    // Debug opcional para acompanhar o comportamento no console
    printf("[SISTEMA] HP: %d/%d | Chance: %.1f%% | Sorteado: %d (Alvo: < %d)\n", 
           inimigo->hp_atual, inimigo->hp_max, chance_final * 100.0, numero_sorteado, alvo_necessario);

    if (numero_sorteado < alvo_necessario) {
        return 1; // CAPTURADO!
    } else {
        return 0; // ESCAPOU!
    }
}


char* usar_item(Jogador *red, int indice_item, Pokemon *pokemon_alvo) {
    char * mensagem = "Voce usou um item!";
    if (indice_item < 0 || indice_item >= red->numero_itens) {
        printf("Erro: Item inválido!\n");
        return 0;
    }

    // Usamos o operador & pois a bolsa guarda structs de Item diretamente, e queremos um ponteiro
    Item *item = &(red->bolsa[indice_item]);

    if (item->quantidade <= 0) {
        return 0;
    }

    int usou_com_sucesso = 0;

    if (strcmp(item->nome, "POTION") == 0) {
        if (pokemon_alvo->hp_atual >= pokemon_alvo->hp_max) {
            usou_com_sucesso = 0; 
        } else {
            pokemon_alvo->hp_atual += 20;
            if (pokemon_alvo->hp_atual > pokemon_alvo->hp_max) {
                pokemon_alvo->hp_atual = pokemon_alvo->hp_max;
            }
            printf("%s foi curado em 20 HP!\n", pokemon_alvo->nome);
            usou_com_sucesso = 1;
            mensagem = "Voce curou 20 HP!";
        }
    } 
    else if (strcmp(item->nome, "SUPER POTION") == 0) {
        if (pokemon_alvo->hp_atual >= pokemon_alvo->hp_max) {
            usou_com_sucesso = 0;
        } else {
            pokemon_alvo->hp_atual += 50;
            if (pokemon_alvo->hp_atual > pokemon_alvo->hp_max) {
                pokemon_alvo->hp_atual = pokemon_alvo->hp_max;
            }
            printf("%s foi curado em 50 HP!\n", pokemon_alvo->nome);
            usou_com_sucesso = 1;
            mensagem = "Voce curou 50 HP!";
        }
    }
    else if (strcmp(item->nome, "POKE BALL") == 0) {
        printf("Você lançou uma POKE BALL em %s!\n", pokemon_alvo->nome);
        
        if (calcular_tentativa_captura(pokemon_alvo)) {
            printf("Sucesso! %s foi capturado!\n", pokemon_alvo->nome);
            
            int qtd_antes = red->numero_pokemons;
            
            
            
            capturar_pokemon(red, pokemon_alvo);
            
            if (red->numero_pokemons > qtd_antes) {
                printf("%s foi adicionado ao seu time!\n", pokemon_alvo->nome);
            } else {
                printf("Seu time já estava cheio!\n");
            }
            
            estado_atual = ESTADO_FIM_BATALHA; 
        } 
        else {
            printf("Ah não! %s escapou da POKE BALL!\n", pokemon_alvo->nome);
            estado_atual = ESTADO_PROCESSANDO_TURNO; 
        }
        
        usou_com_sucesso = 1;
        mensagem = "Voce usou POKEBALL";
    }
    else {
        printf("Esse item não tem um efeito programado ainda.\n");
        usou_com_sucesso = 0;
    }

    if (usou_com_sucesso) {
        item->quantidade--;

        if (item->quantidade == 0) {
            for (int i = indice_item; i < red->numero_itens - 1; i++) {
                red->bolsa[i] = red->bolsa[i + 1];
            }
            red->numero_itens--;
        }
    }

    return mensagem;
}