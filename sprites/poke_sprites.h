#ifndef POKE_SPRITES_H
#define POKE_SPRITES_H

#define TAM_FRENTE 40
#define TAM_COSTAS 32

typedef struct {
    const unsigned short *frente; 
    const unsigned short *costas; 
} SpritePokemon;

extern const unsigned short charmander_frente[40][40];
extern const unsigned short charmander_costas[32][32];
extern const unsigned short bulbasaur_frente[40][40];
extern const unsigned short bulbasaur_costas[32][32];
extern const unsigned short squirtle_frente[40][40];
extern const unsigned short squirtle_costas[32][32];
extern const unsigned short pidgey_frente[40][40];
extern const unsigned short pidgey_costas[32][32];

extern SpritePokemon charmander;
extern SpritePokemon bulbasaur;
extern SpritePokemon squirtle;
extern SpritePokemon pidgey;

#endif // POKE_SPRITES_H
