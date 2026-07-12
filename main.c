#include "perifericos.h"
#include "fonte.h"
#include <stdio.h>
#include "personagem.h"
#include "sprites/sprites.h"

// --- VARIAVEIS DE TESTE ---
Jogador player;
int camera_x = 0; // Camera estatica no topo esquerdo para o teste
int camera_y = 0;

int main() {
    // 1. Inicializa os componentes de video/teclado (mmap na placa,
    // janela SDL no simulador -- depende de qual perifericos.c foi linkado)
    if (hw_init() != 0) {
        printf("Falha ao inicializar hardware\n");
        return 1;
    }
    inicializar_double_buffering();

    // 2. Inicializa o nosso personagem
    start_player(&player, 160, 120, BAIXO); // centralizado, olhando pra frente

    printf("Iniciando teste do sprite do Red...\n");

    // 3. Loop Principal do Jogo
    while (1) {
        // 4. Limpa a tela antes de desenhar o frame novo
        clear();

        // 5. Le a tecla pressionada e move o jogador
        unsigned char tecla = keyboard_input();
        mover_jogador(&player, tecla);

        // 6. Atualiza a logica da animacao
        atualizar_animacao_jogador(&player);

        // 7. Desenha o jogador na tela passando a camera
        desenhar_jogador(camera_x, camera_y, &player);

        // 8. Mostra o frame na tela e processa eventos de teclado/janela
        inverter_buffers();

        // 9. Controla os FPS (~60fps)
        delay(16);
    }

    hw_cleanup();
    return 0;
}