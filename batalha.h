#ifndef BATALHA_POKEMON_H
#define BATALHA_POKEMON_H
#include "pokemons.h"


#define OFFSET_X 80
#define OFFSET_Y 48
#define TELA_LARGURA_GB 160
#define TELA_ALTURA_GB  144

void desenhar_tela_gameboy(const unsigned short tela[144][160]);
void desenhar_pokemons_batalhas(Pokemon red,Pokemon desafiante);

#endif