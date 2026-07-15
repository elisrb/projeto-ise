
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

void desenhar_pokemon_frente(const unsigned short *sprite) {
    for (int y = 0; y < 40; y++) {
        for (int x = 0; x < 40; x++) {
            // posição do pixel na tela
            int vga_x = OFFSET_X + 100 + x;
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
    for (int x = OFFSET_X + 96; x < OFFSET_X + 152; x++){
        for (int y = OFFSET_Y + 0; y < OFFSET_Y + 56; y++) {
            write_pixel(x, y, 0xF75F);
        }
    }
}

void desenhar_pokemons_batalhas(Pokemon red,Pokemon desafiante){
    desenhar_tela_gameboy(menu_batalha);
    completar_fundo_batalha();
    desenhar_pokemon_costas(red.sprites->costas);
    desenhar_pokemon_frente(desafiante.sprites->frente);
    desenhar_dialogo_batalhas();
}

// A 0 a 56 fundo
// A 16 a 56 = sprite
// L 96 a 100 fundo
// L 100 a 140 = sprite
// L 140 a 152 fundo