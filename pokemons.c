#include "pokemons.h"
#include "personagem.h"
#include <string.h>  
#include <stdlib.h>  
#include <stdio.h>

// ============================================================================
// BANCO DE DADOS ESTÁTICO (Escondido dentro do .c para proteção de dados)
// ============================================================================


const InfoAtaque banco_ataques[] = {
    
    [ATK_TACKLE]       = {"TACKLE",       "NORMAL",   40, 35},
    [ATK_GROWL]        = {"GROWL",        "NORMAL",    0, 40}, 
    [ATK_SCRATCH]      = {"SCRATCH",      "NORMAL",   40, 35}, 
    [ATK_EMBER]        = {"EMBER",        "FIRE",     40, 25}, 
    [ATK_THUNDERSHOCK] = {"THUNDERSHOCK", "ELECTRIC", 40, 30}, 
    [ATK_GUST]         = {"GUST",         "FLYING",   40, 35}, 
    [ATK_QUICK_ATTACK] = {"QUICK ATTACK", "NORMAL",   40, 30} 
};


const EspecieBase banco_especies[] = {
    [PKMN_BULBASAUR]  = {"BULBASAUR",  45, 49, 49, 45, {ATK_TACKLE, ATK_GROWL}},
    [PKMN_CHARMANDER] = {"CHARMANDER", 39, 52, 43, 65, {ATK_SCRATCH, ATK_GROWL}},
    [PKMN_SQUIRTLE]   = {"SQUIRTLE",   44, 48, 65, 43, {ATK_TACKLE, ATK_GROWL}},
    [PKMN_PIKACHU]    = {"PIKACHU",    35, 55, 40, 90, {ATK_THUNDERSHOCK, ATK_GROWL}},
    
    
    [PKMN_PIDGEY]     = {"PIDGEY",     40, 45, 40, 56, {ATK_TACKLE, ATK_GUST}},
    [PKMN_RATTATA]    = {"RATTATA",    30, 56, 35, 72, {ATK_TACKLE, ATK_QUICK_ATTACK}}
};

// ============================================================================
// FUNÇÕES DE LÓGICA DO SISTEMA
// ============================================================================


static void aprender_ataque_por_id(Pokemon *pkmn, int id_ataque) {
    if (pkmn->qtd_golpes >= 4) return; 
    int slot = pkmn->qtd_golpes;
    InfoAtaque info = banco_ataques[id_ataque];
    
    strcpy(pkmn->golpes[slot].nome, info.nome);
    pkmn->golpes[slot].dano_base = info.dano_base;
    pkmn->golpes[slot].pp_max = info.pp_max;
    pkmn->golpes[slot].pp_atual = info.pp_max; 
    strcpy(pkmn->golpes[slot].tipo, info.tipo); 
    pkmn->qtd_golpes++;
}

void gerar_pokemon(Pokemon *pkmn, int id_especie, int nivel, SpritePokemon *sprite) {
    
    memset(pkmn, 0, sizeof(Pokemon));

    EspecieBase base = banco_especies[id_especie];
    
    pkmn->id_especie = id_especie;
    pkmn->nivel = nivel;
    
    
    pkmn->hp_max    = ((base.base_hp * 2 * nivel) / 100) + nivel + 10;
    pkmn->ataque    = ((base.base_atk * 2 * nivel) / 100) + 5;
    pkmn->defesa    = ((base.base_def * 2 * nivel) / 100) + 5;
    pkmn->velocidade = ((base.base_vel * 2 * nivel) / 100) + 5;
    
    pkmn->hp_atual = pkmn->hp_max; 
    pkmn->qtd_golpes = 0;
    strcpy(pkmn->nome, base.nome);
    
    pkmn->sprites = sprite;
    
    
    aprender_ataque_por_id(pkmn, base.ataques_iniciais[0]);
    aprender_ataque_por_id(pkmn, base.ataques_iniciais[1]);
}

void subir_nivel(Pokemon *pkmn) {
    pkmn->nivel++;
    
    int hp_max_antigo = pkmn->hp_max;
    EspecieBase base = banco_especies[pkmn->id_especie];
    
    
    pkmn->hp_max    = ((base.base_hp * 2 * pkmn->nivel) / 100) + pkmn->nivel + 10;
    pkmn->ataque    = ((base.base_atk * 2 * pkmn->nivel) / 100) + 5;
    pkmn->defesa    = ((base.base_def * 2 * pkmn->nivel) / 100) + 5;
    pkmn->velocidade = ((base.base_vel * 2 * pkmn->nivel) / 100) + 5;
    
    
    int ganho_de_hp = pkmn->hp_max - hp_max_antigo;
    pkmn->hp_atual += ganho_de_hp;
    
    if (pkmn->hp_atual > pkmn->hp_max) {
        pkmn->hp_atual = pkmn->hp_max;
    }
}

int calcular_dano(const Pokemon *atacante, const Pokemon *defensor, int indice_ataque) {
    
    if (indice_ataque < 0 || indice_ataque >= atacante->qtd_golpes) return 0;

    Ataque golpe = atacante->golpes[indice_ataque];
    
    
    if (golpe.dano_base == 0) return 0;
    
    
    int parte1 = ((2 * atacante->nivel) / 5) + 2;
    int parte2 = parte1 * golpe.dano_base * atacante->ataque;
    int parte3 = parte2 / (defensor->defesa > 0 ? defensor->defesa : 1); 
    int dano_final = (parte3 / 50) + 2;
    
    return dano_final;
}


static void executar_ataque(Pokemon *atacante, Pokemon *defensor, int slot_ataque) {
    if (atacante->hp_atual <= 0){ 
        batalha_on = 3;
        return; 
    }
    
    
    if (slot_ataque < 0 || slot_ataque >= atacante->qtd_golpes) {
        return;
    }

    Ataque *golpe = &atacante->golpes[slot_ataque];
    
    
    if (golpe->pp_atual <= 0) {
        printf("%s tentou usar %s, mas nao tem PP!\n", atacante->nome, golpe->nome);
        return;
    }

    golpe->pp_atual--; 
    
    int dano = calcular_dano(atacante, defensor, slot_ataque);
    defensor->hp_atual -= dano;
    
    
    if (defensor->hp_atual < 0) batalha_on = 3;

    printf("%s usou %s! Causou %d de dano.\n", atacante->nome, golpe->nome, dano);
    
    if (defensor->hp_atual == 0) {
        batalha_on = 3;
        printf("%s desmaiou!\n", defensor->nome);
    }
}


char* processar_turno_batalha(Pokemon *jogador, AcaoBatalha acao_jogador, Pokemon *selvagem, AcaoBatalha acao_selvagem) {
    
    
    if (acao_jogador == ACAO_FUGIR) {
        
        if (jogador->velocidade >= selvagem->velocidade) {
            printf("Voce fugiu com sucesso!\n");
            batalha_on = 3;
            return("Voce fugiu!");
        } else {
            printf("Nao conseguiu fugir!\n");
            
            if (acao_selvagem <= ACAO_ATACAR_SLOT_3) {
                executar_ataque(selvagem, jogador, (int)acao_selvagem);
            }
            return("Voce nao fugiu!");
        }
    }
    
    
    int jogador_ataca_primeiro = 0;

    
    int jogador_usou_prioridade = (acao_jogador <= ACAO_ATACAR_SLOT_3 && 
                                   acao_jogador < jogador->qtd_golpes &&
                                   strcmp(jogador->golpes[acao_jogador].nome, "QUICK ATTACK") == 0);
                                   
    int selvagem_usou_prioridade = (acao_selvagem <= ACAO_ATACAR_SLOT_3 && 
                                    acao_selvagem < selvagem->qtd_golpes &&
                                    strcmp(selvagem->golpes[acao_selvagem].nome, "QUICK ATTACK") == 0);

    if (jogador_usou_prioridade && !selvagem_usou_prioridade) {
        jogador_ataca_primeiro = 1; 
    } else if (!jogador_usou_prioridade && selvagem_usou_prioridade) {
        jogador_ataca_primeiro = 0; 
    } else {
        
        if (jogador->velocidade > selvagem->velocidade) {
            jogador_ataca_primeiro = 1;
        } else if (jogador->velocidade < selvagem->velocidade) {
            jogador_ataca_primeiro = 0;
        } else {
            
            jogador_ataca_primeiro = rand() % 2;
        }
    }

    
    if (jogador_ataca_primeiro) {
        
        if (acao_jogador <= ACAO_ATACAR_SLOT_3) executar_ataque(jogador, selvagem, (int)acao_jogador);
        
        if (selvagem->hp_atual > 0 && acao_selvagem <= ACAO_ATACAR_SLOT_3) {
            executar_ataque(selvagem, jogador, (int)acao_selvagem);
        }
        return("Voce ataca antes!");
    } else {
        
        if (acao_selvagem <= ACAO_ATACAR_SLOT_3) executar_ataque(selvagem, jogador, (int)acao_selvagem);
        
        if (jogador->hp_atual > 0 && acao_jogador <= ACAO_ATACAR_SLOT_3) {
            executar_ataque(jogador, selvagem, (int)acao_jogador);
        }
        return("Voce ataca depois!");
    }
}