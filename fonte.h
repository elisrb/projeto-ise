#ifndef FONTE_POKEMON_H
#define FONTE_POKEMON_H

// Estrutura para guardar a coordenada de um caractere no grid da imagem
typedef struct {
    int coluna;
    int linha;
} CoordenadaGrid;

// Protótipos das funções implementadas no seu .c
CoordenadaGrid obter_coordenada_borda(char* str);
CoordenadaGrid obter_coordenada_caractere(char c);
void desenhar_caractere(int tela_x, int tela_y, char caractere);
void desenhar_texto(int tela_x, int tela_y, const char *texto);

#endif // FONTE_POKEMON_H
