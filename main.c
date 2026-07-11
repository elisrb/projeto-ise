#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "perifericos.h"

int main(){
    if (hw_init() != 0) {
        printf("Falha ao inicializar\n");
        return 1;
    }

    for(int y = 0; y < 240; y++){
        for(int x = 0; x < 320; x++){
            write_pixel(x, y, COR_BRANCO);
        }
    }
    inverter_buffers();
    delay(16);
    hw_cleanup();
    return 0;
}