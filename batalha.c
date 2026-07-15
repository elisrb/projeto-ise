
#include "batalha.h"
#include "perifericos_sdl.h"
#include "pokemons.h"
#include "sprites/telas_batalha.h"
#include "fonte.h"
#include <stdio.h>

EstadoBatalha estado_atual = ESTADO_INTRO_BATALHA;
OpcaoMenu opcao_selecionada = OPCAO_FIGHT;
int cursor_ataque = 0;

void processar_input_batalha(char tecla) {
    if (tecla >= 'A' && tecla <= 'Z') {
        tecla = tecla + 32; 
    }

    switch (estado_atual) {

        case ESTADO_INTRO_BATALHA:
            if (tecla == 'x') {
                estado_atual = ESTADO_MENU_PRINCIPAL;
                opcao_selecionada = OPCAO_FIGHT; // Reseta o cursor no FIGHT
            }
            break;
        
        case ESTADO_MENU_PRINCIPAL:
            if (tecla == 'd') { // Direita
                if (opcao_selecionada == OPCAO_FIGHT) opcao_selecionada = OPCAO_PKMN;
                else if (opcao_selecionada == OPCAO_ITENS) opcao_selecionada = OPCAO_RUN;
            }
            else if (tecla == 'a') { // Esquerda
                if (opcao_selecionada == OPCAO_PKMN) opcao_selecionada = OPCAO_FIGHT;
                else if (opcao_selecionada == OPCAO_RUN) opcao_selecionada = OPCAO_ITENS;
            }
            else if (tecla == 's') { // Baixo
                if (opcao_selecionada == OPCAO_FIGHT) opcao_selecionada = OPCAO_ITENS;
                else if (opcao_selecionada == OPCAO_PKMN) opcao_selecionada = OPCAO_RUN;
            }
            else if (tecla == 'w') { // Cima
                if (opcao_selecionada == OPCAO_ITENS) opcao_selecionada = OPCAO_FIGHT;
                else if (opcao_selecionada == OPCAO_RUN) opcao_selecionada = OPCAO_PKMN;
            }
            
            else if (tecla == 'x') {
                if (opcao_selecionada == OPCAO_FIGHT) {
                    estado_atual = ESTADO_MENU_LUTAR;
                    cursor_ataque = 0; // Começa no primeiro golpe
                } 
                else if (opcao_selecionada == OPCAO_PKMN) {
                    estado_atual = ESTADO_MENU_POKEMON;
                } 
                else if (opcao_selecionada == OPCAO_ITENS) {
                    estado_atual = ESTADO_MENU_ITENS;
                } 
                else if (opcao_selecionada == OPCAO_RUN) {
                    printf("Correndo da batalha...\n");
                    estado_atual = ESTADO_FIM_BATALHA;
                }
            }
            break;

        case ESTADO_MENU_LUTAR:
            if (tecla == 'd') {
                if (cursor_ataque == 0) cursor_ataque = 1;
                else if (cursor_ataque == 2) cursor_ataque = 3;
            }
            else if (tecla == 'a') {
                if (cursor_ataque == 1) cursor_ataque = 0;
                else if (cursor_ataque == 3) cursor_ataque = 2;
            }
            else if (tecla == 's') {
                if (cursor_ataque == 0) cursor_ataque = 2;
                else if (cursor_ataque == 1) cursor_ataque = 3;
            }
            else if (tecla == 'w') {
                if (cursor_ataque == 2) cursor_ataque = 0;
                else if (cursor_ataque == 3) cursor_ataque = 1;
            }
            
            else if (tecla == 'x') {
                printf("Usando o ataque %d!\n", cursor_ataque);
                estado_atual = ESTADO_PROCESSANDO_TURNO;
            }
            else if (tecla == 'z') {
                estado_atual = ESTADO_MENU_PRINCIPAL;
            }
            break;

        case ESTADO_MENU_POKEMON:
        case ESTADO_MENU_ITENS:
            if (tecla == 'z') {
                estado_atual = ESTADO_MENU_PRINCIPAL;
            }
            break;
            
        case ESTADO_PROCESSANDO_TURNO:
            if (tecla == 'x') {
                estado_atual = ESTADO_MENU_PRINCIPAL;
            }
            break;

        case ESTADO_FIM_BATALHA:
            if (tecla == 'x') {
                estado_atual = ESTADO_MENU_PRINCIPAL;
            }
            break;
    }
}
void desenhar_batalha(void) {
    // 1. Sempre limpa a tela e desenha o fundo básico
    
    // (Desenhe os sprites dos Pokémons ativos aqui usando a função que blindamos anteriormente)

    // 2. Desenha a interface específica do estado atual
    switch (estado_atual) {

        case ESTADO_INTRO_BATALHA:
            // Substitua com os dados dinâmicos do seu Pokémon se preferir
            //desenhar_texto("Wild PIDGEOT", X, Y); 
            //desenhar_texto("wants to fight!", X, Y + 10);
            
            // Dica visual: Desenhar uma setinha piscando no canto inferior direito
            // para o jogador saber que precisa apertar 'X' para continuar.
            break;
        
        case ESTADO_MENU_PRINCIPAL:
            
            // Aqui você desenha os textos "FIGHT", "PKMN", "ITEM", "RUN"
            desenhar_setinha_menu_principal(opcao_selecionada);
            break;

        case ESTADO_MENU_LUTAR:
            // Aqui você desenha a lista de golpes do seu Pokémon ativo
            desenhar_setinha_menu_lutar(cursor_ataque);
            break;

        case ESTADO_MENU_POKEMON:
            // Chame sua função para desenhar a lista/tela dos pokémons do jogador
            break;

        case ESTADO_MENU_ITENS:
            // Chame sua função para renderizar a mochila de itens
            break;

        case ESTADO_PROCESSANDO_TURNO:
            // Chame sua função que renderiza as mensagens textuais na tela
            break;

        case ESTADO_FIM_BATALHA:
            // Desenhe a tela de vitória, XP ou mensagem de fuga
            break;
    }
}

void desenhar_setinha_menu_principal(OpcaoMenu opcao_atual) {
    int x, y;

    for (int i = 0; i < 4; i++) {
    
        switch (i) {
            case OPCAO_FIGHT:
                x = SETA_X_COL0; y = SETA_Y_LIN0;
                break;
            case OPCAO_PKMN:
                x = SETA_X_COL1; y = SETA_Y_LIN0;
                break;
            case OPCAO_ITENS:
                x = SETA_X_COL0; y = SETA_Y_LIN1;
                break;
            case OPCAO_RUN:
                x = SETA_X_COL1; y = SETA_Y_LIN1;
                break;
            default:
                continue;
        }

        if (i == opcao_atual) {
            desenhar_texto(">", x, y);
        } else {
            desenhar_texto(" ", x, y);
        }
    }
}

void desenhar_tela_gameboy(const unsigned short tela[144][160]) {
    for (int y = 0; y < 144; y++) {
        for (int x = 0; x < 160; x++) {
            // Pega a cor do pixel correspondente na matriz
            unsigned short cor = tela[y][x];
            
            // Calcula a coordenada final na VGA somando o offset de centralização
            int vga_x = OFFSET_X + x;
            int vga_y = OFFSET_Y + y;
            
            // Plota o pixel na tela
            write_pixel(vga_x, vga_y, cor);
        }
    }
}

void desenhar_pokemon_frente(int pos_x, int pos_y, const unsigned short *sprite) {
    for (int y = 0; y < 40; y++) {
        for (int x = 0; x < 40; x++) {
            // posição do pixel na tela
            int vga_x = OFFSET_X + pos_x + x;
            int vga_y = OFFSET_Y + pos_y + y;

            // checa se não ultrapassa o tamanho da tela
            if (pos_x + x >= 0 && pos_x + x < TELA_LARGURA_GB &&
                pos_y + y >= 0 && pos_y + y < TELA_ALTURA_GB) {
                
                // cor do pixel atual da matriz do sprite
                unsigned short cor_pixel = sprite[y * 40 + x];

                write_pixel(vga_x, vga_y, cor_pixel);
            }
        }
    }
}

void desenhar_pokemon_costas(const unsigned short *sprite) {
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 32; x++) {
            // posição do pixel na tela
            int vga_x = OFFSET_X + 8 + (x*2);
            int vga_y = OFFSET_Y + 40 + (y*2);

            // checa se não ultrapassa o tamanho da tela
            if (8 + (x*2) >= 0 && 8 + (x*2) + 1 < TELA_LARGURA_GB &&
                40 + (y*2) >= 0 && 40 + (y*2) + 1 < 97) {
                
                // cor do pixel atual da matriz do sprite
                unsigned short cor_pixel = sprite[y * 32 + x];

            // quadruplica o tamanho do pixel
            write_pixel(vga_x, vga_y, cor_pixel);
            write_pixel(vga_x + 1, vga_y, cor_pixel);
            write_pixel(vga_x, vga_y + 1, cor_pixel);
            write_pixel(vga_x + 1, vga_y + 1, cor_pixel);
            }
        }
    }
}

void desenhar_dialogo_batalhas() {
    imprimir_caixa_dialogo();
    desenhar_caractere(8, 12, obter_coordenada_borda("canto_sup_esq"));
    desenhar_caractere(8, 17, obter_coordenada_borda("canto_inf_esq"));
    desenhar_caractere(8, 13, obter_coordenada_borda("borda_vertical"));
}

void desenhar_pokemons_batalhas(Pokemon red,Pokemon desafiante){
    desenhar_tela_gameboy(menu_batalha);
    desenhar_pokemon_costas(red.sprites->costas);
    desenhar_pokemon_frente(100, 16, desafiante.sprites->frente);
    desenhar_dialogo_batalhas();
}