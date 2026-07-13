#ifndef COLISAO_H
#define COLISAO_H

enum Terreno {
    LIVRE,
    OBSTACULO,
    GRAMA
};

/* Mapas de colisão: dimensões [altura/16][largura/16] de cada fundo */

extern enum Terreno rota1_colisao[36][20];
extern enum Terreno casa1_colisao[8][8];
extern enum Terreno casa2_colisao[8][8];
extern enum Terreno casa_rival_colisao[8][8];
extern enum Terreno lab_colisao[12][10];
extern enum Terreno cidade_colisao[18][20];

#endif /* COLISAO_H */