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
    // 1. Inicializa os componentes de vídeo/teclado (mmap na placa ou janela SDL no simulador)
    if (hw_init() != 0) {
        printf("Falha ao inicializar o hardware da DE1-SoC.\n");
        return 1;
    }
    
    // Inicializa o sistema de double buffering da VGA
    inicializar_double_buffering();

    // 2. Configura as dimensões E AS MATRIZES DE COLISÃO dos cenários
    cidade.largura = 320; 
    cidade.altura = 288;
    cidade.mapa_colisao = (const Terreno *)cidade_colisao; // <-- VINCULA A COLISÃO DA CIDADE

    rota1.largura = 320;  
    rota1.altura = 576;
    rota1.mapa_colisao = (const Terreno *)rota1_colisao;   // <-- VINCULA A COLISÃO DA ROTA 1
    
    // Se for testar as casas ou lab depois, basta fazer o mesmo:
    // casa1.largura = 128; casa1.altura = 128; casa1.mapa_colisao = (const Terreno *)casa1_colisao;

    // 3. Carrega o cenário inicial (Cidade de Pallet)
    cenario_atual = &cidade;

    // 4. Inicializa o Red no centro da tela (coordenadas do mundo) olhando para baixo
    start_player(&player, 36, 96, BAIXO);

    printf("Teste do motor de jogo com COLISÕES ativas!\n");
    printf("Use W, A, S, D ou as Setas do teclado para mover o Red.\n");

    // 5. Loop Principal do Jogo
    clear();
    while (1) {
        // A. Limpa o buffer dos comandos de desenho

        // B. Lê a tecla e processa o movimento (agora checando colisão internamente no personagem.c)
        unsigned char tecla = keyboard_input_filtrado();
        mover_jogador(&player, tecla);

        // C. Atualiza a posição da câmera para seguir o jogador de acordo com o tamanho dinâmico (160x144 ou 320x240)
        atualizar_camera(player.x, player.y);

        // D. Atualiza a lógica de animação das pernas do Red
        atualizar_animacao_jogador(&player);

        // E. Desenha a parte visível do cenário de fundo baseado na câmera e na moldura centralizada
        desenhar_cenario();

        // F. Desenha o jogador por cima do cenário aplicando o posicionamento correto da janela
        desenhar_jogador(camera_x, camera_y, &player);

        // G. Realiza a troca dos buffers da VGA sem screen tearing
        inverter_buffers();

        // H. Controla a taxa de atualização para cravar em aproximadamente 60 FPS
        delay(16);
    }

    // Código de encerramento caso o loop termine
    hw_cleanup();
    return 0;
}