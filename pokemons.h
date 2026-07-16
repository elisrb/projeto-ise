#ifndef POKEMON_H
#define POKEMON_H
#include "sprites/poke_sprites.h"
// ==========================================
// CONSTANTES E IDs DE IDENTIFICAÇÃO
// ==========================================
#define ATK_TACKLE        0
#define ATK_GROWL         1
#define ATK_SCRATCH       2
#define ATK_EMBER         3
#define ATK_THUNDERSHOCK  4
#define ATK_GUST          5
#define ATK_QUICK_ATTACK  6

#define PKMN_BULBASAUR   0
#define PKMN_CHARMANDER  1
#define PKMN_SQUIRTLE    2
#define PKMN_PIKACHU     3
#define PKMN_PIDGEY      4  
#define PKMN_RATTATA     5  

// ==========================================
// ESTRUTURA DOS ATAQUES (MOVIMENTOS)
// ==========================================
typedef struct {
    char nome[20];
    char tipo[20];   
    int dano_base;
    int pp_max;
} InfoAtaque;

typedef struct {
    char nome[20];
    char tipo[20];      
    int dano_base;       
    int pp_max;          
    int pp_atual;        
} Ataque;

typedef struct {
    char nome[20];
    int base_hp;
    int base_atk;
    int base_def;
    int base_vel;
    int ataques_iniciais[2]; 
} EspecieBase;

// ==========================================
// ESTRUTURA PRINCIPAL DO POKÉMON
// ==========================================
typedef struct {
    char nome[20];       
    int id_especie;      
    int nivel;           
    
    
    int hp_max;          
    int hp_atual;        
    int ataque;          
    int defesa;          
    int velocidade;      
    
    
    Ataque golpes[4];
    int qtd_golpes;      
    
    
    SpritePokemon *sprites;
} Pokemon;

typedef enum {
    ACAO_ATACAR_SLOT_0 = 0,
    ACAO_ATACAR_SLOT_1 = 1,
    ACAO_ATACAR_SLOT_2 = 2,
    ACAO_ATACAR_SLOT_3 = 3,
    ACAO_FUGIR,
    ACAO_USAR_ITEM
} AcaoBatalha;

typedef struct{
    char nome[20];
    int quantidade;
}Item;

extern const InfoAtaque banco_ataques[];
extern const EspecieBase banco_especies[];

// ==========================================
// PROTÓTIPO DAS FUNÇÕES
// ==========================================


void gerar_pokemon(Pokemon *pkmn, int id_especie, int nivel, SpritePokemon *sprite);


void subir_nivel(Pokemon *pkmn);


int calcular_dano(const Pokemon *atacante, const Pokemon *defensor, int indice_ataque);

char *processar_turno_batalha(Pokemon *jogador, AcaoBatalha acao_jogador, Pokemon *selvagem, AcaoBatalha acao_selvagem);

#endif // POKEMON_H