#ifndef POKE_SPRITES_H
#define POKE_SPRITES_H

typedef struct {
    const unsigned short frente[40][40];
    const unsigned short costas[32][32];
} SpritePokemon;

extern SpritePokemon charmander;
extern SpritePokemon bulbasaur;
extern SpritePokemon squirtle;
extern SpritePokemon pidgey;

#endif // POKE_SPRITES_H
