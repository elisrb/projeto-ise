#include <stdio.h>
#include "perifericos_sdl.h"
#include "fonte.h"
#include "personagem.h"
#include "sprites/sprites.h"
#include "sprites/fundos.h"
#include "sprites/colisao.h"

int main() {
    Jogador player;
    // Inicializa os periféricos (placa ou simulador)
    if (hw_init() != 0) {
        printf("Falha ao inicializar o hardware da DE1-SoC.\n");
        return 1;
    }
    
    // Inicializa o sistema de double buffering da VGA
    inicializar_double_buffering();

    // Carrega o cenário inicial (Cidade de Pallet)
    carregar_cenario(&cidade);

    // Inicializa o Red no meio da tela olhando para baixo
    start_player(&player, 36, 96, BAIXO);

    printf("Teste do motor de jogo com COLISÕES ativas!\n");
    printf("Use W, A, S, D ou as Setas do teclado para mover o Red.\n");

    // Loop Principal do Jogo
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