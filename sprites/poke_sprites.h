#ifndef POKE_SPRITES_H
#define POKE_SPRITES_H

#define TAM_FRENTE 40
#define TAM_COSTAS 32

typedef struct {
    const unsigned short *frente; 
    const unsigned short *costas; 
} SpritePokemon;

extern SpritePokemon charmander;
extern SpritePokemon bulbasaur;
extern SpritePokemon squirtle;
extern SpritePokemon pidgey;

#endif // POKE_SPRITES_H
