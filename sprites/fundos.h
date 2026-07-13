#ifndef FUNDOS_H
#define FUNDOS_H

struct Cenario {
    int altura;
    int largura;
};

extern struct Cenario rota1;
extern struct Cenario casa1;
extern struct Cenario casa2;
extern struct Cenario casa_rival;
extern struct Cenario lab;
extern struct Cenario cidade;

extern const unsigned short rota1_fundo[576][320];
extern const unsigned short casa1_fundo[128][128];
extern const unsigned short casa2_fundo[128][128];
extern const unsigned short casa_rival_fundo[128][128];
extern const unsigned short lab_fundo[192][160];
extern const unsigned short cidade_fundo[288][320];

#endif // FUNDOS_H