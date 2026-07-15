#ifndef BATALHA_POKEMON_H
#define BATALHA_POKEMON_H
#include "pokemons.h"


#define OFFSET_X 80
#define OFFSET_Y 48
#define TELA_LARGURA_GB 160
#define TELA_ALTURA_GB  144

#define SETA_X_COL0   14   // Posição X para FIGHT e ITEM
#define SETA_X_COL1   16  // Posição X para PKMN e RUN

#define SETA_Y_LIN0   9 // Posição Y para FIGHT e PKMN
#define SETA_Y_LIN1   15

typedef enum {
    ESTADO_INTRO_BATALHA,     // <-- NOVO: "Wild [PKMN] wants to fight!"
    ESTADO_MENU_PRINCIPAL,    // Escolhendo entre: Fight, Run, PKMN, Itens
    ESTADO_MENU_LUTAR,        // Escolhendo qual dos 4 golpes usar
    ESTADO_MENU_POKEMON,      // Tela de troca de Pokémon do time
    ESTADO_MENU_ITENS,        // Tela da mochila / itens
    ESTADO_PROCESSANDO_TURNO, // Mostrando as mensagens de ataque ("Pikachu usou Tackle!")
    ESTADO_FIM_BATALHA        // Tela de vitória, derrota ou fuga concluída
} EstadoBatalha;

typedef enum {
    OPCAO_FIGHT = 0,
    OPCAO_PKMN,
    OPCAO_ITENS,
    OPCAO_RUN
} OpcaoMenu;

// Variáveis de controle globais (para que outros arquivos possam ler se necessário)
extern EstadoBatalha estado_atual;
extern OpcaoMenu opcao_selecionada;
extern int cursor_ataque; // De 0 a 3 para mapear os 4 golpes

void processar_input_batalha(unsigned char tecla);
void desenhar_batalha(Pokemon red, Pokemon desafiante);
void desenhar_setinha_menu_principal(OpcaoMenu opcao_atual);

void desenhar_tela_gameboy(const unsigned short tela[144][160]);
void desenhar_pokemon_frente(int pos_x, int pos_y, const unsigned short *sprite);
void desenhar_pokemon_costas(const unsigned short *sprite);
void desenhar_pokemons_batalhas(Pokemon red,Pokemon desafiante);

#endif