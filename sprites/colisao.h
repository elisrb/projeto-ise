#ifndef COLISAO_H
#define COLISAO_H

typedef enum {
    LIVRE,
    OBSTACULO,
    GRAMA
} Terreno;

/* Mapas de colisão: dimensões [altura/16][largura/16] de cada fundo */

extern Terreno rota1_colisao[36][20];
extern Terreno casa1_colisao[8][8];
extern Terreno casa2_colisao[8][8];
extern Terreno casa_rival_colisao[8][8];
extern Terreno lab_colisao[12][10];
extern Terreno cidade_colisao[18][20];

#endif /* COLISAO_H */