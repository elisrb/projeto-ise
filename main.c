#include <stdio.h>

#include "perifericos_sdl.h"
#include "fonte.h"
#include "batalha.h"
#include "pokemons.h"
#include "personagem.h"
#include "sprites/telas_batalha.h"
#include "sprites/poke_sprites.h"
#include "sprites/sprites.h"
#include "sprites/fundos.h"
#include "sprites/colisao.h"

int main() {
    Jogador player;
    
    // Inicializa os periféricos (placa ou simulador)
    if (hw_init() != 0) {
        printf("Falha ao inicializar periféricos.\n");
        return 1;
    }
    
    // Inicializa o sistema de double buffering da VGA
    inicializar_double_buffering();

    // === CORREÇÃO CRÍTICA AQUI ===
    // Inicializa o player com valores padrão (posição 0,0, olhando para baixo)
    start_player(&player, 0, 0, 1); // Garante que numero_itens e numero_pokemons virem 0!

    Pokemon pokemon_inimigo;
    Pokemon pokemon_red;
    Item pocao = {"POTION", 1}; // Dica: Use "POTION" em vez de "POCAO" para casar com o seu usar_item!
    
    gerar_pokemon(&pokemon_inimigo, 1, 5, &charmander);
    gerar_pokemon(&pokemon_red, 0, 0, &bulbasaur);

    // Agora é seguro capturar e dar itens, pois os contadores são zero!
    capturar_pokemon(&player, &pokemon_red);
    pegar_item(&player, &pocao);

    printf("Teste de batalha iniciado.\n");

    unsigned char tecla_anterior = 0;

    while (1) {
        unsigned char tecla_atual = keyboard_input();

        if (tecla_atual != 0 && tecla_atual != tecla_anterior) {
            processar_input_batalha(tecla_atual, pokemon_red, player);
        }
        tecla_anterior = tecla_atual;

        clear();
        desenhar_batalha(pokemon_red, pokemon_inimigo, player);
        inverter_buffers();

        delay(16);
    }

    hw_cleanup();
    return 0;
}