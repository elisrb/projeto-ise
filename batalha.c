
#include "batalha.h"
#include "perifericos_sdl.h"
#include "pokemons.h"
#include "sprites/telas_batalha.h"

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

void desenhar_sprite_rpg(int pos_x, int pos_y, int largura, int altura, const unsigned short *sprite) {
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            // Calcula a posição real do pixel na tela VGA
            int vga_x = OFFSET_X + pos_x + x;
            int vga_y = OFFSET_Y + pos_y + y;

            // Garante que não vai desenhar fora dos limites físicos do Game Boy (160x144)
            if (pos_x + x >= 0 && pos_x + x < TELA_LARGURA_GB &&
                pos_y + y >= 0 && pos_y + y < TELA_ALTURA_GB) {
                
                // Pega a cor do pixel atual da matriz do sprite
                unsigned short cor_pixel = sprite[y * largura + x];

                // 0xFBE5 é a cor rosa usada geralmente como fundo transparente.
                // Se o seu sprite usar outra cor de transparência (como verde puro 0x07E0 ou preto 0x0000), altere aqui.
                if (cor_pixel != 0xFBE5) {
                    write_pixel(vga_x, vga_y, cor_pixel);
                }
            }
        }
    }
}

void desenhar_pokemons_batalhas(Pokemon red,Pokemon desafiante){
    desenhar_tela_gameboy(menu_batalha);
    desenhar_sprite_rpg(10, 90, TAM_COSTAS, TAM_COSTAS, red.sprites->costas);
    desenhar_sprite_rpg(90, 10, TAM_FRENTE, TAM_FRENTE, desafiante.sprites->frente);
}