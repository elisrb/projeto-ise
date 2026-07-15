
#include "batalha.h"
#include "perifericos_sdl.h"
#include "pokemons.h"
#include "sprites/telas_batalha.h"
#include "fonte.h"

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