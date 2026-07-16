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
    // inicializa o jogador, pokemons e itens iniciais
    Jogador player;
    Pokemon pokemon_inimigo;
    Pokemon pokemon_red;
    Pokemon pokemon_selvagem;
    Item pocao = {"POTION", 1};
    Item poke_b = {"POKE BALL", 10};

    // o jogo começa na casa do jogador
    cenario_atual = &casa2;
    start_player(&player, 32, 32, BAIXO);
    
    // inicializa os pokemons em jogo: o do jogador, o do rival, e o selvagem
    gerar_pokemon(&pokemon_red, PKMN_BULBASAUR, 5, &bulbasaur);

    if(pokemon_red.id_especie == PKMN_BULBASAUR) {
        gerar_pokemon(&pokemon_inimigo, PKMN_CHARMANDER, 5, &charmander);
    } else if(pokemon_red.id_especie == PKMN_CHARMANDER) {
        gerar_pokemon(&pokemon_inimigo, PKMN_SQUIRTLE, 5, &squirtle);
    } else if(pokemon_red.id_especie == PKMN_SQUIRTLE) {
        gerar_pokemon(&pokemon_inimigo, PKMN_BULBASAUR, 5, &bulbasaur);
    }

    gerar_pokemon(&pokemon_selvagem, PKMN_PIDGEY, 3, &pidgey);

    // inicializa os itens do jogador
    capturar_pokemon(&player, &pokemon_red);
    pegar_item(&player, &pocao);
    pegar_item(&player, &poke_b);
    
    // Inicializa os periféricos (placa ou simulador)
    if (hw_init() != 0) {
        printf("Falha ao inicializar periféricos.\n");
        return 1;
    }
    
    // Inicializa o sistema de double buffering da VGA
    inicializar_double_buffering();

    batalha_on = 0; // controla se está em batalha ou não
    // 0 = não está em batalha, 1 = batalha vai começar, 2 = batalha em andamento, 3 = batalha terminou

    unsigned char tecla_anterior = 0;
    unsigned char tecla_atual = 0;

    // menu inicial
    clear();
    while (tecla_atual != 0x5A) { // 0x5A = Enter
        completar_fundo_batalha();
        imprimir_caixa_dialogo();
        escrever_texto(1, 13, "Pressione Enter");
        escrever_texto(1, 14, "para jogar");
        tecla_atual = keyboard_input_filtrado();
        inverter_buffers();
        delay(16);
    }
    clear();
    while (1) {
        tecla_atual = keyboard_input_filtrado();

        switch (batalha_on) {
            case 0: // não está em batalha: jogador andando
                mover_jogador(&player, tecla_atual);
                atualizar_camera(player.x, player.y);
                atualizar_animacao_jogador(&player);
                desenhar_jogador(camera_x, camera_y, &player);
                desenhar_cenario();
                desenhar_jogador(camera_x, camera_y, &player);
                break;

            case 1: // batalha vai começar: inicializa o estado da batalha
                estado_atual = ESTADO_INTRO_BATALHA;
                opcao_selecionada = OPCAO_FIGHT;
                cursor = 0;
                batalha_on = 2;
                break;

            case 2: // batalha em andamento 
                if (tecla_atual != 0 && tecla_atual != tecla_anterior) {
                    if (cenario_atual == &lab) {
                        processar_input_batalha(tecla_atual, &pokemon_red, &pokemon_inimigo, &player);
                    } else {
                        processar_input_batalha(tecla_atual, &pokemon_red, &pokemon_selvagem, &player);
                    }
                }
                
                if (cenario_atual == &lab) {
                    desenhar_batalha(pokemon_red, pokemon_inimigo, player);
                } else {
                    desenhar_batalha(pokemon_red, pokemon_selvagem, player);
                }
                break;

            case 3: // batalha terminou
                batalha_on = 0;
                pokemon_red.hp_atual = pokemon_red.hp_max;
                pokemon_inimigo.hp_atual = pokemon_inimigo.hp_max;
                pokemon_selvagem.hp_atual = pokemon_selvagem.hp_max;
                break;
        }

        tecla_anterior = tecla_atual;

        inverter_buffers(); // Joga o buffer desenhado (e limpo) para a tela
        delay(32);
    }

    hw_cleanup();
    return 0;
}