#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include "sprites/fundos.h"

// --- CONFIGURAÇÕES DO JOGO E SPRITES ---
#define SPRITE_TAMANHO 16
#define VELOCIDADE     1
#define TELA_LARGURA   320
#define TELA_ALTURA    240

// --- BASTA MUDAR ESTES DOIS VALORES PARA MUDAR O TAMANHO DA TELA DO JOGO ---
#define CAM_LARGURA    160  // Largura da janela do jogo
#define CAM_ALTURA     144  // Altura da janela do jogo

// Centralização automática baseada na janela escolhida acima
#define OFFSET_X       ((TELA_LARGURA - CAM_LARGURA) / 2)
#define OFFSET_Y       ((TELA_ALTURA - CAM_ALTURA) / 2)

// --- ENUMERADOS ---
typedef enum {
    CIMA = 0,
    DIREITA = 1,
    BAIXO = 2,
    ESQUERDA = 3
} Direcao;

// --- ESTRUTURA DO JOGADOR ---
typedef struct {
    int x;                 // Posição X no mundo gigante (pixels)
    int y;                 // Posição Y no mundo gigante (pixels)
    Direcao direcao;       // Direção atual (CIMA, DIREITA, BAIXO, ESQUERDA)
    int movendo;           // Flag: 1 se estiver andando, 0 se parado
    int movendo_anterior;  // Flag do frame anterior para o gatilho visual imediato
    int frame_atual;       // Coluna atual na matriz de sprites (0, 1 ou 2)
    int timer_animacao;    // Contador para controlar a taxa de frames das pernas
    int timer_movimento;   // Contador auxiliar para controle de passo (se necessário)
} Jogador;

// --- VARIÁVEIS GLOBAIS DA CÂMERA E CENÁRIO ---
// Declaradas como extern para que o main.c também possa ler os valores delas
extern int camera_x;
extern int camera_y;
extern struct Cenario *cenario_atual;
extern const unsigned short *pixels_cenario_atual;

// --- ASSINATURAS DAS FUNÇÕES ---

// Inicializa os atributos do jogador no começo do jogo
void start_player(Jogador *player, int dx, int dy, int dir);

// Lê a entrada do teclado, muda a direção e altera a coordenada (x, y) do mundo
void mover_jogador(Jogador *player, unsigned char tecla);

// Controla o tempo e atualiza qual frame da matriz deve ser mostrado nas pernas
void atualizar_animacao_jogador(Jogador *player);

// Converte a posição do mundo para posição da tela e plota o sprite na VGA
void desenhar_jogador(int camera_x, int camera_y, const Jogador *player);

// Define qual é o cenário atual e seus limites de pixels
void carregar_cenario(struct Cenario *novo_cenario, const unsigned short *novos_pixels);

// Calcula para onde a câmera deve ir para focar no Red, respeitando as bordas
void atualizar_camera(int jogador_x, int jogador_y);

// Varre a tela de 320x240 e desenha a parte visível da imagem do cenário de fundo
void desenhar_cenario();

#endif // PERSONAGEM_H