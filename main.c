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

    // Carrega o cenário inicial (Cidade de Pallet)
    cenario_atual = &cidade;

    // Inicializa o Red no meio da tela olhando para baixo
    start_player(&player, 36, 96, BAIXO);

    inicializar_double_buffering();

    clear();
    Pokemon pokemon_inimigo;
    Pokemon pokemon_red;
    gerar_pokemon(&pokemon_inimigo, 1, 5, &charmander);
    gerar_pokemon(&pokemon_red, 0, 0, &bulbasaur);    
    
    while (1) {
    // 1. Lê os controles
    unsigned char tecla = keyboard_input_filtrado();

    // 2. Limpa o que está no fundo
    clear();

    // 3. Desenha o cenário de fundo
    desenhar_pokemons_batalhas(pokemon_red, pokemon_inimigo);

    // 4. Desenha o texto por cima (Toda vez, em todos os frames!)
    //escrever_texto(14, 1, "                 ");
    //escrever_texto(15, 1, "                 ");
    //escrever_texto(16, 1, "                 ");
    
    
    // 5. Joga o que desenhou para a tela
    inverter_buffers();

    // 6. 60 FPS
    delay(16);
}

    hw_cleanup();

    return 0;
}