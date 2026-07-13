// arquivo temporário para testar o fundo
#include "perifericos.h"
#include "fundos.h"
#include <stdio.h>

int main(void) {
    if (hw_init() != 0) {
        printf("Falha ao inicializar\n");
        return 1;
    }

    inicializar_double_buffering();
    clear();

    // parâmetro 
    int lin_max = 576;

    int x = 160, y = 120;
    int rodando = 1;

    while (rodando) {
        // printa o fundo
        for(int i = lin_max - 240; i < lin_max; i++) {
            for(int j = 0; j < 320; j++) {
                write_pixel(j, (i - lin_max + 240), rota1_fundo[i][j]);
            }
        }

        // isso já tava no main
        unsigned char tecla = keyboard_input_filtrado();

        switch (tecla) {
            case 0x75: case 0x1D: lin_max -= 16; break; // cima / W
            case 0x72: case 0x1B: lin_max += 16; break; // baixo / S
            case 0x76: rodando = 0; break;        // ESC sai
            default: break;
        }

        if(lin_max < 240) lin_max = 240;
        if(lin_max > 576) lin_max = 576;

        inverter_buffers();
        delay(16); // ~60 fps
    }

    hw_cleanup();
    return 0;
}
