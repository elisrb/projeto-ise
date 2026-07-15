#include "batalha.h"
#include "perifericos_sdl.h"
#include "pokemons.h"
#include "sprites/telas_batalha.h"
#include "fonte.h"
#include <stdio.h>

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

    escrever_texto(7, 10, "          ");
    escrever_texto(7, 10, red.nome);

    escrever_texto(0, 1, "          ");
    escrever_texto(0, 1, desafiante.nome);

    escrever_texto(8, 14, ":L");
    escrever_texto(1, 4, ":L");

    char tmp[4];

    snprintf(tmp, sizeof(tmp), "%d", red.nivel);
    escrever_texto(8, 16, tmp);
    snprintf(tmp, sizeof(tmp), "%d", desafiante.nivel);
    escrever_texto(1, 6, tmp);

    snprintf(tmp, sizeof(tmp), "%d", red.hp_atual);
    escrever_texto(10, 11, "   ");
    escrever_texto(10, 11, tmp);
    snprintf(tmp, sizeof(tmp), "%d", red.hp_max);
    escrever_texto(10, 15, "   ");
    escrever_texto(10, 15, tmp);
}