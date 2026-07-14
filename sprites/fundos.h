#ifndef FUNDOS_H
#define FUNDOS_H
#include "colisao.h"

typedef struct Porta Porta;
typedef struct Cenario Cenario;

struct Cenario {
    int altura;
    int largura;
    int qtd_portas;
    const Terreno *mapa_colisao;
    const unsigned short *fundo;
    const Porta *portas;
};

struct Porta {
    int x, y;
    Cenario *destino;
    int novo_x, novo_y;
};

extern const unsigned short rota1_fundo[576][320];
extern const unsigned short casa1_fundo[128][128];
extern const unsigned short casa2_fundo[128][128];
extern const unsigned short casa_rival_fundo[128][128];
extern const unsigned short lab_fundo[192][160];
extern const unsigned short cidade_fundo[288][320];

extern Cenario rota1;
extern Cenario casa1;
extern Cenario casa2;
extern Cenario casa_rival;
extern Cenario lab;
extern Cenario cidade;

#endif // FUNDOS_H