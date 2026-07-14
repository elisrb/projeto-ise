#define OFFSET_X 80
#define OFFSET_Y 48

#include "batalha.h"
#include "perifericos_sdl.h"

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