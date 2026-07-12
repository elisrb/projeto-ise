#include "perifericos.h"
#include "fonte.h"
#include <stdio.h>

int main(void) {
    if (hw_init() != 0) {
        printf("Falha ao inicializar\n");
        return 1;
    }

    inicializar_double_buffering();

    int x = 160, y = 120;
    int rodando = 1;

    while (rodando) {
        unsigned char tecla = keyboard_input_filtrado();

        // Move um quadrado com as setas ou WASD -- só pra testar o teclado
        switch (tecla) {
            case 0x75: case 0x1D: y -= 4; break; // cima / W
            case 0x72: case 0x1B: y += 4; break; // baixo / S
            case 0x6B: case 0x1C: x -= 4; break; // esquerda / A
            case 0x74: case 0x23: x += 4; break; // direita / D
            case 0x76: rodando = 0; break;        // ESC sai
            default: break;
        }

        clear();
        desenhar_texto(90, 110, "ola meu lindo potoxitos s2");
        desenhar_texto(80, 118, "   TE AMO!                ");

        // desenha um quadrado 10x10 na posição atual
        for (int dy = 0; dy < 10; dy++)
            for (int dx = 0; dx < 10; dx++)
                write_pixel(x + dx, y + dy, VERDE_GRAMA);

        inverter_buffers();
        delay(16); // ~60 fps
    }

    hw_cleanup();
    return 0;
}