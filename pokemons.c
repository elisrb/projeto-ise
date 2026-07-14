#include "pokemons.h"
#include <string.h>  // Para usar strcpy nativo do C
#include <stdlib.h>  // Caso precise de funções como rand() no futuro
#include <stdio.h>
// ============================================================================
// BANCO DE DADOS ESTÁTICO (Escondido dentro do .c para proteção de dados)
// ============================================================================

typedef struct {
    char nome[20];
    int dano_base;
    int pp_max;
} InfoAtaque;

// Tabela de propriedades estáticas dos ataques
const InfoAtaque banco_ataques[] = {
    [ATK_TACKLE]       = {"TACKLE",       40, 35},
    [ATK_GROWL]        = {"GROWL",         0, 40}, // Golpe de efeito/status
    [ATK_SCRATCH]      = {"SCRATCH",      40, 35},
    [ATK_EMBER]        = {"EMBER",        40, 25},
    [ATK_THUNDERSHOCK] = {"THUNDERSHOCK", 40, 30},
    [ATK_GUST]         = {"GUST",         40, 35},
    [ATK_QUICK_ATTACK] = {"QUICK ATTACK", 40, 30}
};

typedef struct {
    char nome[20];
    int base_hp;
    int base_atk;
    int base_def;
    int base_vel;
    int ataques_iniciais[2]; // IDs dos ataques que ele já carrega ao nascer
} EspecieBase;

// Tabela com os Status Base originais do Game Boy (RGB)
const EspecieBase banco_especies[] = {
    [PKMN_BULBASAUR]  = {"BULBASAUR",  45, 49, 49, 45, {ATK_TACKLE, ATK_GROWL}},
    [PKMN_CHARMANDER] = {"CHARMANDER", 39, 52, 43, 65, {ATK_SCRATCH, ATK_GROWL}},
    [PKMN_SQUIRTLE]   = {"SQUIRTLE",   44, 48, 65, 43, {ATK_TACKLE, ATK_GROWL}},
    [PKMN_PIKACHU]    = {"PIKACHU",    35, 55, 40, 90, {ATK_THUNDERSHOCK, ATK_GROWL}},
    
    // Espécies nativas da Rota 1
    [PKMN_PIDGEY]     = {"PIDGEY",     40, 45, 40, 56, {ATK_TACKLE, ATK_GUST}},
    [PKMN_RATTATA]    = {"RATTATA",    30, 56, 35, 72, {ATK_TACKLE, ATK_QUICK_ATTACK}}
};

// ============================================================================
// FUNÇÕES DE LOGICA DO SISTEMA
// ============================================================================

// Função auxiliar interna para ensinar os golpes pelo ID da tabela
static void aprender_ataque_por_id(Pokemon *pkmn, int id_ataque) {
    if (pkmn->qtd_golpes >= 4) return; // Limite padrão de 4 movimentos
    
    int slot = pkmn->qtd_golpes;
    InfoAtaque info = banco_ataques[id_ataque];
    
    strcpy(pkmn->golpes[slot].nome, info.nome);
    pkmn->golpes[slot].dano_base = info.dano_base;
    pkmn->golpes[slot].pp_max = info.pp_max;
    pkmn->golpes[slot].pp_atual = info.pp_max; // Nasce com carga máxima de PP
    
    pkmn->qtd_golpes++;
}

void gerar_pokemon(Pokemon *pkmn, int id_especie, int nivel, const unsigned short *sp_frente, const unsigned short *sp_costas) {
    EspecieBase base = banco_especies[id_especie];
    
    pkmn->id_especie = id_especie;
    pkmn->nivel = nivel;
    
    // Fórmulas matemáticas originais para balanceamento por nível
    pkmn->hp_max    = ((base.base_hp * 2 * nivel) / 100) + nivel + 10;
    pkmn->ataque    = ((base.base_atk * 2 * nivel) / 100) + 5;
    pkmn->defesa    = ((base.base_def * 2 * nivel) / 100) + 5;
    pkmn->velocidade = ((base.base_vel * 2 * nivel) / 100) + 5;
    
    pkmn->hp_atual = pkmn->hp_max; // Preenche a vida inteira na criação
    pkmn->qtd_golpes = 0;
    strcpy(pkmn->nome, base.nome);
    
    pkmn->sprite_frente = sp_frente;
    pkmn->sprite_costas = sp_costas;
    
    // Atribui os dois golpes iniciais da espécie automaticamente
    aprender_ataque_por_id(pkmn, base.ataques_iniciais[0]);
    aprender_ataque_por_id(pkmn, base.ataques_iniciais[1]);
}

void subir_nivel(Pokemon *pkmn) {
    pkmn->nivel++;
    
    int hp_max_antigo = pkmn->hp_max;
    EspecieBase base = banco_especies[pkmn->id_especie];
    
    // Recalcula tudo com o novo nível modificado
    pkmn->hp_max    = ((base.base_hp * 2 * pkmn->nivel) / 100) + pkmn->nivel + 10;
    pkmn->ataque    = ((base.base_atk * 2 * pkmn->nivel) / 100) + 5;
    pkmn->defesa    = ((base.base_def * 2 * pkmn->nivel) / 100) + 5;
    pkmn->velocidade = ((base.base_vel * 2 * pkmn->nivel) / 100) + 5;
    
    // Regra do Game Boy: Incrementa a vida ganha sem curar o dano que ele já tinha sofrido
    int ganho_de_hp = pkmn->hp_max - hp_max_antigo;
    pkmn->hp_atual += ganho_de_hp;
    
    if (pkmn->hp_atual > pkmn->hp_max) {
        pkmn->hp_atual = pkmn->hp_max;
    }
}

int calcular_dano(const Pokemon *atacante, const Pokemon *defensor, int indice_ataque) {
    Ataque golpe = atacante->golpes[indice_ataque];
    
    // Se for um golpe de status (como Growl), não causa dano numérico direto
    if (golpe.dano_base == 0) return 0;
    
    // Fórmula oficial simplificada de Pokémon (Geração 1):
    // Dano = (((((2 * Nivel) / 5) + 2) * PoderAtaque * (AtkAtacante / DefDefensor)) / 50) + 2
    int parte1 = ((2 * atacante->nivel) / 5) + 2;
    int parte2 = parte1 * golpe.dano_base * atacante->ataque;
    int parte3 = parte2 / (defensor->defesa > 0 ? defensor->defesa : 1); // Evita divisão por zero
    int dano_final = (parte3 / 50) + 2;
    
    return dano_final;
}


// Função auxiliar interna para aplicar o dano de um ataque
static void executar_ataque(Pokemon *atacante, Pokemon *defensor, int slot_ataque) {
    if (atacante->hp_atual <= 0) return; // Se desmaiou antes, não ataca!

    Ataque *golpe = &atacante->golpes[slot_ataque];
    
    // Verifica se ainda tem PP disponível
    if (golpe->pp_atual <= 0) {
        printf("%s tentou usar %s, mas nao tem PP!\n", atacante->nome, golpe->nome);
        return;
    }

    golpe->pp_atual--; // Gasta 1 de PP

    // Calcula e aplica o dano
    int dano = calcular_dano(atacante, defensor, slot_ataque);
    defensor->hp_atual -= dano;
    
    // Garante que o HP não fique negativo
    if (defensor->hp_atual < 0) defensor->hp_atual = 0;

    printf("%s usou %s! Causou %d de dano.\n", atacante->nome, golpe->nome, dano);
    
    if (defensor->hp_atual == 0) {
        printf("%s desmaiou!\n", defensor->nome);
    }
}

// FUNÇÃO PRINCIPAL: PROCESSA O TURNO INTEIRO
void processar_turno_batalha(Pokemon *jogador, AcaoBatalha acao_jogador, Pokemon *selvagem, AcaoBatalha acao_selvagem) {
    
    // 1. Tratamento de Fuga (Fugir tem prioridade máxima antes dos ataques)
    if (acao_jogador == ACAO_FUGIR) {
        // Regra simples: se o jogador for mais rápido ou tiver sorte, foge
        if (jogador->velocidade >= selvagem->velocidade) {
            printf("Voce fugiu com sucesso!\n");
            // Aqui no seu loop principal você setaria o estado do jogo de volta para MAPA
        } else {
            printf("Nao conseguiu fugir!\n");
            // Como o jogador falhou em fugir, o selvagem ataca livremente
            if (acao_selvagem <= ACAO_ATACAR_SLOT_3) {
                executar_ataque(selvagem, jogador, (int)acao_selvagem);
            }
        }
        return; 
    }

    // 2. Determinar Prioridade de Ataque
    int jogador_ataca_primeiro = 0;

    // Checa se alguém usou o Quick Attack (ID 6 que configuramos no banco)
    int jogador_usou_prioridade = (acao_jogador <= ACAO_ATACAR_SLOT_3 && 
                                   strcmp(jogador->golpes[acao_jogador].nome, "QUICK ATTACK") == 0);
    int selvagem_usou_prioridade = (acao_selvagem <= ACAO_ATACAR_SLOT_3 && 
                                    strcmp(selvagem->golpes[acao_selvagem].nome, "QUICK ATTACK") == 0);

    if (jogador_usou_prioridade && !selvagem_usou_prioridade) {
        jogador_ataca_primeiro = 1; // Prioridade por golpe
    } else if (!jogador_usou_prioridade && selvagem_usou_prioridade) {
        jogador_ataca_primeiro = 0; // Prioridade por golpe para o inimigo
    } else {
        // Se nenhum ou ambos usaram golpe de prioridade, decide na Velocidade pura
        if (jogador->velocidade > selvagem->velocidade) {
            jogador_ataca_primeiro = 1;
        } else if (jogador->velocidade < selvagem->velocidade) {
            jogador_ataca_primeiro = 0;
        } else {
            // Empate perfeito de velocidade? Sorteia quem vai! (rand() % 2)
            jogador_ataca_primeiro = rand() % 2;
        }
    }

    // 3. Execução das Ações na ordem correta
    if (jogador_ataca_primeiro) {
        // Jogador bate primeiro
        if (acao_jogador <= ACAO_ATACAR_SLOT_3) executar_ataque(jogador, selvagem, (int)acao_jogador);
        // Selvagem só revida se ainda estiver vivo!
        if (selvagem->hp_atual > 0 && acao_selvagem <= ACAO_ATACAR_SLOT_3) {
            executar_ataque(selvagem, jogador, (int)acao_selvagem);
        }
    } else {
        // Selvagem bate primeiro
        if (acao_selvagem <= ACAO_ATACAR_SLOT_3) executar_ataque(selvagem, jogador, (int)acao_selvagem);
        // Jogador só revida se sobreviver!
        if (jogador->hp_atual > 0 && acao_jogador <= ACAO_ATACAR_SLOT_3) {
            executar_ataque(jogador, selvagem, (int)acao_jogador);
        }
    }
}