#ifndef POKEMON_H
#define POKEMON_H

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
#define PKMN_PIDGEY      4  // Rota 1
#define PKMN_RATTATA     5  // Rota 1

// ==========================================
// ESTRUTURA DOS ATAQUES (MOVIMENTOS)
// ==========================================
typedef struct {
    char nome[20];       // Ex: "TACKLE"
    int dano_base;       // Poder do golpe
    int pp_max;          // Limite máximo de usos
    int pp_atual;        // Usos restantes na batalha
} Ataque;

// ==========================================
// ESTRUTURA PRINCIPAL DO POKÉMON
// ==========================================
typedef struct {
    char nome[20];       // Nome do monstrinho
    int id_especie;      // Guarda o ID (ex: PKMN_CHARMANDER) para evoluções/level up
    int nivel;           // Nível atual (1 a 100)
    
    // Status de combate atualizados
    int hp_max;          
    int hp_atual;        // Vida atual (se chegar a 0, desmaiou)
    int ataque;          
    int defesa;          
    int velocidade;      // Quem tiver a maior, ataca primeiro no turno
    
    // Sistema de golpes (Máximo de 4)
    Ataque golpes[4];
    int qtd_golpes;      
    
    // Ponteiros para os sprites na tela de batalha da placa DE1-SoC
    const unsigned short *sprite_frente; 
    const unsigned short *sprite_costas; 
} Pokemon;

typedef enum {
    ACAO_ATACAR_SLOT_0 = 0,
    ACAO_ATACAR_SLOT_1 = 1,
    ACAO_ATACAR_SLOT_2 = 2,
    ACAO_ATACAR_SLOT_3 = 3,
    ACAO_FUGIR,
    ACAO_USAR_ITEM
} AcaoBatalha;

// ==========================================
// PROTÓTIPO DAS FUNÇÕES
// ==========================================

// Inicializa e escala um Pokémon do zero baseado no banco de dados e nível
void gerar_pokemon(Pokemon *pkmn, int id_especie, int nivel, const unsigned short *sp_frente, const unsigned short *sp_costas);

// Sobe 1 nível do Pokémon e recalcula os status mantendo a proporção de dano sofrido
void subir_nivel(Pokemon *pkmn);

// Calcula o dano de forma simplificada baseada na fórmula oficial de Gen 1
int calcular_dano(const Pokemon *atacante, const Pokemon *defensor, int indice_ataque);

void processar_turno_batalha(Pokemon *jogador, AcaoBatalha acao_jogador, Pokemon *selvagem, AcaoBatalha acao_selvagem);

#endif // POKEMON_H