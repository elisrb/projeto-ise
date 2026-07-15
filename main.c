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

    Pokemon pokemon_inimigo;
    Pokemon pokemon_red;
    gerar_pokemon(&pokemon_inimigo, 1, 5, &charmander);
    gerar_pokemon(&pokemon_red, 0, 0, &bulbasaur);

    printf("Teste de batalha iniciado.\n");

    unsigned char tecla_anterior = 0;

    while (1) {
        unsigned char tecla_atual = keyboard_input();

        // Só processa no frame em que a tecla MUDOU pra um valor novo
        // e diferente de zero -- evita repetir a ação todo frame
        // enquanto a tecla fica segurada (menu não é movimento contínuo)
        if (tecla_atual != 0 && tecla_atual != tecla_anterior) {
            processar_input_batalha(tecla_atual);
        }
        tecla_anterior = tecla_atual;

        clear();
        desenhar_batalha(pokemon_red, pokemon_inimigo);
        inverter_buffers();

        delay(16);
    }

    hw_cleanup();
    return 0;
}