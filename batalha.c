#include "batalha.h"
#include "perifericos_sdl.h"
#include "pokemons.h"
#include "sprites/telas_batalha.h"
#include "fonte.h"
#include <stdio.h>

EstadoBatalha estado_atual = ESTADO_INTRO_BATALHA;
OpcaoMenu opcao_selecionada = OPCAO_FIGHT;
int cursor_ataque = 0;

void processar_input_batalha(unsigned char tecla) {
    delay(16); // pra não ler a mesma tecla várias vezes
    printf("Tecla pressionada: 0x%02X\n", tecla);
    switch (estado_atual) {

        case ESTADO_INTRO_BATALHA:
            if (tecla == 0x5A) { // X, PROVISORIAMENTE ENTER
                estado_atual = ESTADO_MENU_PRINCIPAL;
                opcao_selecionada = OPCAO_FIGHT;
            }
            break;
        
        case ESTADO_MENU_PRINCIPAL:
            if (tecla == 0x23) { // D - direita
                if (opcao_selecionada == OPCAO_FIGHT) opcao_selecionada = OPCAO_PKMN;
                else if (opcao_selecionada == OPCAO_ITENS) opcao_selecionada = OPCAO_RUN;
            }
            else if (tecla == 0x1C) { // A - esquerda
                if (opcao_selecionada == OPCAO_PKMN) opcao_selecionada = OPCAO_FIGHT;
                else if (opcao_selecionada == OPCAO_RUN) opcao_selecionada = OPCAO_ITENS;
            }
            else if (tecla == 0x1B) { // S - baixo
                if (opcao_selecionada == OPCAO_FIGHT) opcao_selecionada = OPCAO_ITENS;
                else if (opcao_selecionada == OPCAO_PKMN) opcao_selecionada = OPCAO_RUN;
            }
            else if (tecla == 0x1D) { // W - cima
                if (opcao_selecionada == OPCAO_ITENS) opcao_selecionada = OPCAO_FIGHT;
                else if (opcao_selecionada == OPCAO_RUN) opcao_selecionada = OPCAO_PKMN;
            }
            else if (tecla == 0x5A) { // X, PROVISORIAMENTE ENTER
                if (opcao_selecionada == OPCAO_FIGHT) {
                    estado_atual = ESTADO_MENU_LUTAR;
                    cursor_ataque = 0;
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
            if (tecla == 0x23) {
                if (cursor_ataque == 0) cursor_ataque = 1;
                else if (cursor_ataque == 2) cursor_ataque = 3;
            }
            else if (tecla == 0x1C) {
                if (cursor_ataque == 1) cursor_ataque = 0;
                else if (cursor_ataque == 3) cursor_ataque = 2;
            }
            else if (tecla == 0x1B) {
                if (cursor_ataque == 0) cursor_ataque = 2;
                else if (cursor_ataque == 1) cursor_ataque = 3;
            }
            else if (tecla == 0x1D) {
                if (cursor_ataque == 2) cursor_ataque = 0;
                else if (cursor_ataque == 3) cursor_ataque = 1;
            }
            else if (tecla == 0x22) { // X
                printf("Usando o ataque %d!\n", cursor_ataque);
                estado_atual = ESTADO_PROCESSANDO_TURNO;
            }
            else if (tecla == 0x1A) { // Z - voltar
                estado_atual = ESTADO_MENU_PRINCIPAL;
            }
            break;

        case ESTADO_MENU_POKEMON:
        case ESTADO_MENU_ITENS:
            if (tecla == 0x1A) { // Z
                estado_atual = ESTADO_MENU_PRINCIPAL;
            }
            break;
            
        case ESTADO_PROCESSANDO_TURNO:
            if (tecla == 0x22) {
                estado_atual = ESTADO_MENU_PRINCIPAL;
            }
            break;

        case ESTADO_FIM_BATALHA:
            if (tecla == 0x22) {
                estado_atual = ESTADO_MENU_PRINCIPAL;
            }
            break;
    }
}
void desenhar_batalha(Pokemon red, Pokemon desafiante) {
    // 1. Sempre limpa a tela e desenha o fundo básico
    
    // (Desenhe os sprites dos Pokémons ativos aqui usando a função que blindamos anteriormente)
    desenhar_pokemons_batalhas(red, desafiante);

    // 2. Desenha a interface específica do estado atual
    switch (estado_atual) {

        case ESTADO_INTRO_BATALHA:
            // Substitua com os dados dinâmicos do seu Pokémon se preferir
            //escrever_texto("Wild PIDGEOT", X, Y); 
            //escrever_texto("wants to fight!", X, Y + 10);
            
            // Dica visual: Desenhar uma setinha piscando no canto inferior direito
            // para o jogador saber que precisa apertar 'X' para continuar.
            break;
        
        case ESTADO_MENU_PRINCIPAL:
            // Aqui você desenha os textos "FIGHT", "PKMN", "ITEM", "RUN"
            desenhar_setinha_menu_principal(opcao_selecionada);
            break;

        case ESTADO_MENU_LUTAR:
            // Aqui você desenha a lista de golpes do seu Pokémon ativo
            //desenhar_setinha_menu_lutar(cursor_ataque);
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
            escrever_texto(x, y, ">");
        } else {
            escrever_texto(x, y, " ");
        }
    }
}

void desenhar_tela_gameboy(const unsigned short tela[144][160]) {
    for (int y = 0; y < 144; y++) {
        for (int x = 0; x < 160; x++) {
            // cor do pixel correspondente
            unsigned short cor = tela[y][x];
            
            // coordenada final na tela
            int vga_x = OFFSET_X + x;
            int vga_y = OFFSET_Y + y;
            
            write_pixel(vga_x, vga_y, cor);
        }
    }
}

void desenhar_pokemon_frente(const unsigned short *sprite) {
    for (int y = 0; y < 40; y++) {
        for (int x = 0; x < 40; x++) {
            // posição do pixel na tela
            int vga_x = OFFSET_X + 104 + x;
            int vga_y = OFFSET_Y + 16 + y;

            // cor do pixel atual da matriz do sprite
            unsigned short cor_pixel = sprite[y * 40 + x];
            write_pixel(vga_x, vga_y, cor_pixel);
        }
    }
}

void desenhar_pokemon_costas(const unsigned short *sprite) {
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 32; x++) {
            // posição do pixel na tela
            int vga_x = OFFSET_X + 8 + (x*2);
            int vga_y = OFFSET_Y + 40 + (y*2);
                
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

void desenhar_dialogo_batalhas() {
    imprimir_caixa_dialogo();
    desenhar_caractere(8, 12, obter_coordenada_borda("canto_sup_esq"));
    desenhar_caractere(8, 17, obter_coordenada_borda("canto_inf_esq"));
    desenhar_caractere(8, 13, obter_coordenada_borda("borda_vertical"));
}

void completar_fundo_batalha() {
    // completa borda do pokemon rival
    for (int x = OFFSET_X + 96; x < OFFSET_X + 152; x++){ 
        for (int y = OFFSET_Y + 0; y < OFFSET_Y + 56; y++) {
            write_pixel(x, y, 0xF75F);
        }
    }
}

void desenhar_pokemons_batalhas(Pokemon red, Pokemon desafiante){
    desenhar_tela_gameboy(menu_batalha);

    completar_fundo_batalha();
    desenhar_pokemon_costas(red.sprites->costas);
    desenhar_pokemon_frente(desafiante.sprites->frente);
    desenhar_dialogo_batalhas();

    escrever_texto(10, 7, "          ");
    escrever_texto(10, 7, red.nome);

    escrever_texto(1, 0, "          ");
    escrever_texto(1, 0, desafiante.nome);

    escrever_texto(14, 8, ":L");
    escrever_texto(4, 1, ":L");

    char tmp[4];

    snprintf(tmp, sizeof(tmp), "%d", red.nivel);
    escrever_texto(16, 8, tmp);
    snprintf(tmp, sizeof(tmp), "%d", desafiante.nivel);
    escrever_texto(6, 1, tmp);

    snprintf(tmp, sizeof(tmp), "%d", red.hp_atual);
    escrever_texto(11, 10, "   ");
    escrever_texto(11, 10, tmp);
    snprintf(tmp, sizeof(tmp), "%d", red.hp_max);
    escrever_texto(15, 10, "   ");
    escrever_texto(15, 10, tmp);
}