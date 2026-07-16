#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include "sprites/fundos.h"
#include "pokemons.h"

// --- CONFIGURAÇÕES DO JOGO E SPRITES ---
#define SPRITE_TAMANHO 16
#define VELOCIDADE     2
#define TELA_LARGURA   320
#define TELA_ALTURA    240

// --- BASTA MUDAR ESTES DOIS VALORES PARA MUDAR O TAMANHO DA TELA DO JOGO ---
#define CAM_LARGURA    160  // Largura da janela do jogo
#define CAM_ALTURA     144  // Altura da janela do jogo

// Centralização automática baseada na janela escolhida acima
#define OFFSET_X       ((TELA_LARGURA - CAM_LARGURA) / 2)
#define OFFSET_Y       ((TELA_ALTURA - CAM_ALTURA) / 2)

#define OFFSET_VISUAL_Y -4
#define TILE 16

// --- ENUMERADOS ---
typedef enum {
    CIMA = 0,
    DIREITA = 1,
    BAIXO = 2,
    ESQUERDA = 3
} Direcao;

// --- ESTRUTURA DO JOGADOR ---
typedef struct {
    int x;
    int y;
 
    int destino_x; 
    int destino_y;
 
    Direcao direcao;
    int movendo;

    int frame_atual;     
    int timer_animacao;  
 
    int timer_movimento; 
    int movendo_anterior; 

    Pokemon pokemons[5];
    int numero_pokemons;

    Item bolsa[10];
    int numero_itens;
} Jogador;

// --- VARIÁVEIS GLOBAIS ---

extern int camera_x;
extern int camera_y;
extern Cenario *cenario_atual;
extern int batalha_on;

// --- ASSINATURAS DAS FUNÇÕES ---

// Inicializa os atributos do jogador no começo do jogo
void start_player(Jogador *player, int dx, int dy, int dir);

// Lê a entrada do teclado, muda a direção e altera a coordenada (x, y) do mundo
void mover_jogador(Jogador *player, unsigned char tecla);

// Controla o tempo e atualiza qual frame da matriz deve ser mostrado nas pernas
void atualizar_animacao_jogador(Jogador *player);

// Converte a posição do mundo para posição da tela e plota o sprite na VGA
void desenhar_jogador(int camera_x, int camera_y, const Jogador *player);

// Calcula para onde a câmera deve ir para focar no Red, respeitando as bordas
void atualizar_camera(int jogador_x, int jogador_y);

// Varre a tela de 320x240 e desenha a parte visível da imagem do cenário de fundo
void desenhar_cenario();

int checar_colisao(int prox_x, int prox_y);

void capturar_pokemon(Jogador *red, Pokemon *pokemon_capturado);
void pegar_item(Jogador *red, Item *item);
char *usar_item(Jogador *red, int indice_item, Pokemon *pokemon_alvo);

#endif // PERSONAGEM_H