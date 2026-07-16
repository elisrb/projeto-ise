#include "batalha.h"
#include "perifericos_sdl.h"
#include "sprites/telas_batalha.h"
#include "fonte.h"
#include <stdio.h>
#include <stdlib.h>

EstadoBatalha estado_atual = ESTADO_INTRO_BATALHA;
OpcaoMenu opcao_selecionada = OPCAO_FIGHT;
int cursor = 0;
char tmp[6];
char *mensagem_turno = NULL;

void processar_input_batalha(unsigned char tecla, Pokemon *red, Pokemon *desafiante, Jogador *player){
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
                    cursor = 0;
                } 
                else if (opcao_selecionada == OPCAO_PKMN) {
                    estado_atual = ESTADO_MENU_POKEMON;
                    cursor = 0;
                } 
                else if (opcao_selecionada == OPCAO_ITENS) {
                    estado_atual = ESTADO_MENU_ITENS;
                    cursor = 0;
                } 
                else if (opcao_selecionada == OPCAO_RUN) {
                    mensagem_turno = processar_turno_batalha(red, cursor, desafiante, ACAO_FUGIR);
                    printf("Correndo da batalha...\n");

                    estado_atual = ESTADO_PROCESSANDO_TURNO;
                }
            }
            break;

        case ESTADO_MENU_LUTAR:
            if (tecla == 0x1B) { // S - baixo
                cursor = (cursor + 1) % red->qtd_golpes;
            }
            else if (tecla == 0x1D) { // W - cima
                cursor = (cursor + red->qtd_golpes - 1) % red->qtd_golpes; // Cicla entre 0 e red.qtd_golpes-1 (equivalente a -1 mod red.qtd_golpes)
            }
            else if (tecla == 0x5A) { // X
                printf("Usando o ataque %d!\n", cursor);
                AcaoBatalha acao_selvagem = (AcaoBatalha)(rand() % desafiante->qtd_golpes);
                mensagem_turno = processar_turno_batalha(red, cursor, desafiante, acao_selvagem);
                estado_atual = ESTADO_PROCESSANDO_TURNO;
            }
            else if (tecla == 0x1A) { // Z - voltar
                estado_atual = ESTADO_MENU_PRINCIPAL;
            }
            break;

        case ESTADO_MENU_POKEMON:
            if (tecla == 0x1B) { // S - baixo
                cursor = (cursor + 1) % player->numero_pokemons; 
            }
            else if (tecla == 0x1D) { // W - cima
                cursor = (cursor + player->numero_pokemons - 1) % player->numero_pokemons; 
            }
            else if (tecla == 0x1A) { // Z
                estado_atual = ESTADO_MENU_PRINCIPAL;
            }
            else if (tecla == 0x5A) { // X
                // chamar função de trocar para o pokemon [cursor]
                estado_atual = ESTADO_PROCESSANDO_TURNO;
            }
            break;
        
        case ESTADO_MENU_ITENS:
            if (tecla == 0x1B) { // S - baixo
                cursor = (cursor + 1) % player->numero_itens; 
            }
            else if (tecla == 0x1D) { // W - cima
                cursor = (cursor + player->numero_itens - 1) % player->numero_itens; 
            }
            else if (tecla == 0x1A) { // Z
                estado_atual = ESTADO_MENU_PRINCIPAL;
            }
            else if (tecla == 0x5A) { // X
                mensagem_turno = usar_item(player,cursor,red);
                estado_atual = ESTADO_PROCESSANDO_TURNO;
            }
            break;
            
        case ESTADO_PROCESSANDO_TURNO:
            if (tecla == 0x5A) { // X
                estado_atual = ESTADO_MENU_PRINCIPAL;
            }
            break;

        case ESTADO_FIM_BATALHA:
            batalha_on = 3;
            break;
    }
}
void desenhar_batalha(Pokemon red, Pokemon desafiante, Jogador player) {
    // Limpa a tela e desenha o fundo básico
    desenhar_pokemons_batalhas(red, desafiante);

    // Desenha a interface específica do estado atual
    switch (estado_atual) {
        case ESTADO_INTRO_BATALHA:
            desenhar_tela_gameboy(wants_to_fight);
            escrever_texto(11, 10, "       ");
            break;
        
        case ESTADO_MENU_PRINCIPAL:
            desenhar_tela_gameboy(menu_batalha);
            desenhar_setinha_menu_principal(opcao_selecionada);

            desenhar_caractere(8, 12, obter_coordenada_borda("canto_sup_esq"));
            desenhar_caractere(8, 17, obter_coordenada_borda("canto_inf_esq"));
            desenhar_caractere(8, 13, obter_coordenada_borda("borda_vertical"));
            break;

        case ESTADO_MENU_LUTAR:
            desenhar_tela_gameboy(menu_fight);

            escrever_texto(2, 10, "        ");
            escrever_texto(5, 11, "     ");

            if(cursor < red.qtd_golpes) {
                escrever_texto(2, 10, red.golpes[cursor].tipo); 
                snprintf(tmp, sizeof(tmp), "%d/%d", red.golpes[cursor].pp_atual, red.golpes[cursor].pp_max);
                escrever_texto(5, 11, tmp);
            }
            
            escrever_texto(5, 13, "             ");
            escrever_texto(5, 14, "             ");
            escrever_texto(5, 15, "             ");
            escrever_texto(5, 16, "             ");

            for (int i = 0; i < red.qtd_golpes; i++) {
                escrever_texto(6, 13 + i, red.golpes[i].nome);
            }

            desenhar_setinha_menu_luta(cursor);

            break;

        case ESTADO_MENU_POKEMON:
            desenhar_tela_gameboy(menu_pkmn);

            escrever_texto(5, 4, "             ");
            escrever_texto(5, 5, "             ");
            escrever_texto(5, 6, "             ");
            escrever_texto(5, 7, "             ");
            escrever_texto(5, 8, "             ");
            escrever_texto(5, 9, "             ");
            escrever_texto(5, 10, "             ");

            for (int i = 0; i < player.numero_pokemons; i++) { 
                escrever_texto(6, 4 + i, player.pokemons[i].nome);
            }

            desenhar_setinha_menu_pkmn(cursor);
            break;

        case ESTADO_MENU_ITENS:
            desenhar_tela_gameboy(menu_pkmn);

            escrever_texto(5, 4, "             ");
            escrever_texto(5, 5, "             ");
            escrever_texto(5, 6, "             ");
            escrever_texto(5, 7, "             ");
            escrever_texto(5, 8, "             ");
            escrever_texto(5, 9, "             ");
            escrever_texto(5, 10, "             ");

            for (int i = 0; i < player.numero_itens; i++) { 
                escrever_texto(6, 4 + i*2, player.bolsa[i].nome);
                snprintf(tmp, sizeof(tmp), "x %d", player.bolsa[i].quantidade); 
                escrever_texto(14, 4 + i*2 + 1, tmp);
            }

            desenhar_setinha_menu_itens(cursor);
            break;

        case ESTADO_PROCESSANDO_TURNO:
            escrever_texto(11, 10, "       ");
            if (mensagem_turno != NULL) {
                escrever_texto(1, 13, mensagem_turno);
                delay(500);
            }

            break;

        case ESTADO_FIM_BATALHA:
            // Desenhe a tela de vitória, XP ou mensagem de fuga
            break;
    }
}

void desenhar_setinha_menu_luta(int cursor) {
    int x, y;

    for (int i = 0; i < 4; i++) {
    
        x = 5; y = 13 + i; 
        
        if (i == cursor) {
            escrever_texto(x, y, ">");
        } else {
            escrever_texto(x, y, " ");
        }
    }
}

void desenhar_setinha_menu_pkmn(int cursor) {
    int x, y;

    for (int i = 0; i < 6; i++) {
    
        x = 5; y = 4 + i; 

        if (i == cursor) {
            escrever_texto(x, y, ">");
        } else {
            escrever_texto(x, y, " ");
        }
    }
}

void desenhar_setinha_menu_itens(int cursor) {
    int x, y;

    for (int i = 0; i < 3; i++) {
    
        x = 5; y = 4 + i*2; 

        if (i == cursor) {
            escrever_texto(x, y, ">");
        } else {
            escrever_texto(x, y, " ");
        }
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
            
            if (cor != 0xA004 && cor != 0xF802) { // cor transparente
                // coordenada final na tela
                int vga_x = OFFSET_X + x;
                int vga_y = OFFSET_Y + y;
            
            write_pixel(vga_x, vga_y, cor);
                continue;
            }
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

void completar_fundo_batalha() {
    // completa as transparências
    for (int x = OFFSET_X; x < OFFSET_X + TELA_LARGURA_GB; x++){ 
        for (int y = OFFSET_Y; y < OFFSET_Y + TELA_ALTURA_GB; y++) {
            write_pixel(x, y, 0xF75F);
        }
    }
}

void desenhar_pokemons_batalhas(Pokemon red, Pokemon desafiante){
    completar_fundo_batalha();
    desenhar_pokemon_costas(red.sprites->costas);
    desenhar_pokemon_frente(desafiante.sprites->frente);
    imprimir_caixa_dialogo();

    escrever_texto(10, 7, "          ");
    escrever_texto(10, 7, red.nome);

    escrever_texto(1, 0, "          ");
    escrever_texto(1, 0, desafiante.nome);

    escrever_texto(14, 8, ":L");
    escrever_texto(4, 1, ":L");

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