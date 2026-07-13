#include <stdio.h>
#include "perifericos.h"
#include "fonte.h"
#include "personagem.h"
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

    // 2. Configura as dimensões dos cenários (caso não estejam pré-configuradas no fundos.c)
    // Se o seu fundos.c já define esses tamanhos, você pode remover estas linhas:
    cidade.largura = 320; 
    cidade.altura = 288;
    rota1.largura = 320;  
    rota1.altura = 576;
    // (Adicione as dimensões dos outros cenários se for testá-los)

    // 3. Carrega o cenário inicial (Cidade de Pallet)
    carregar_cenario(&cidade, (const unsigned short *)cidade_fundo);

    // 4. Inicializa o Red no centro da tela (coordenadas do mundo) olhando para baixo
    // Como a cidade tem 320x288, iniciar em (160, 120) é perfeito
    start_player(&player, 160, 120, BAIXO);

    printf("Teste do motor de jogo iniciado com sucesso!\n");
    printf("Use W, A, S, D ou as Setas do teclado para mover o Red.\n");

    // 5. Loop Principal do Jogo
    while (1) {
        // A. Limpa o buffer dos comandos de desenho (opcional, já que o cenário cobre a tela toda)
        clear();

        // B. Lê a tecla pressionada e processa o movimento do jogador (respeitando os limites do mapa)
        unsigned char tecla = keyboard_input_filtrado();
        mover_jogador(&player, tecla);

        // C. Atualiza a posição da câmera para seguir o jogador e travar nas bordas do mapa
        atualizar_camera(player.x, player.y);

        // D. Atualiza a lógica de animação das pernas do Red
        atualizar_animacao_jogador(&player);

        // E. Desenha a parte visível do cenário de fundo baseado na posição atual da câmera
        desenhar_cenario();

        // F. Desenha o jogador por cima do cenário (passando a câmera calculada)
        desenhar_jogador(camera_x, camera_y, &player);

        // G. Realiza a troca dos buffers da VGA para mostrar o frame renderizado sem screen tearing
        inverter_buffers();

        // H. Controla a taxa de atualização para cravar em aproximadamente 60 FPS
        delay(16);
    }

    // Código de encerramento caso o loop termine (boa prática de segurança)
    hw_cleanup();
    return 0;
}