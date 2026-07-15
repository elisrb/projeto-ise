#include <stdio.h>

#include "perifericos_sdl.h"
#include "fonte.h"
#include "sprites/telas_batalha.h"
#include "batalha.h"
#include "pokemons.h"
#include "sprites/poke_sprites.h"



int main(void)
{
    if (hw_init() != 0)
    {
        printf("Erro ao iniciar SDL.\n");
        return 1;
    }

    inicializar_double_buffering();

    clear();
    Pokemon pokemon_inimigo;
    Pokemon pokemon_red;
    gerar_pokemon(&pokemon_inimigo, 1, 5, &charmander);
    gerar_pokemon(&pokemon_red, 0, 0, &bulbasaur);
    desenhar_sprite_rpg(10, 10, TAM_FRENTE, TAM_FRENTE, pokemon_inimigo.sprites->frente);
    

    escrever_texto(1, 1, "ola mundo");
    
    
    while (1) {
    // 1. Lê os controles
    unsigned char tecla = keyboard_input_filtrado();

    // 2. LIMPA o buffer que está no fundo
    clear();

    // 3. DESENHA o cenário de fundo
    desenhar_pokemons_batalhas(pokemon_red, pokemon_inimigo);

    // 4. DESENHA o texto por cima (Toda vez, em todos os frames!)
    //escrever_texto(14, 1, "                 ");
    //escrever_texto(15, 1, "                 ");
    //escrever_texto(16, 1, "                 ");
    
    
    // 5. APENAS AGORA joga o que desenhou para a tela
    inverter_buffers();

    // 6. Espera o tempo do frame (60 FPS)
    delay(16);
}

    hw_cleanup();

    return 0;
}