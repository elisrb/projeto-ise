#ifndef FONTE_POKEMON_H
#define FONTE_POKEMON_H

#define OFFSET_X 80
#define OFFSET_Y 48

#define MAX_COLUNAS 20
#define MAX_LINHAS  18

// Estrutura para guardar a coordenada de um caractere no grid da imagem
typedef struct {
    int coluna;
    int linha;
} CoordenadaGrid;

// Protótipos das funções implementadas no seu .c
CoordenadaGrid obter_coordenada_borda(char* str);
CoordenadaGrid obter_coordenada_caractere(char c);
void desenhar_caractere(int tela_x, int tela_y, CoordenadaGrid coord);
void escrever_texto(int caixa_x, int caixa_y, const char *texto);
void escrever_texto_progressivo(int caixa_x,int caixa_y,const char *texto,int tempo);
void imprimir_caixa_dialogo();

#endif
