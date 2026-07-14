#ifndef POKE_SPRITES_H
#define POKE_SPRITES_H

struct SpritePokemon {
    const unsigned short frente[40][40];
    const unsigned short costas[32][32];
};

extern struct SpritePokemon charmander;
extern struct SpritePokemon bulbasaur;
extern struct SpritePokemon squirtle;
extern struct SpritePokemon pidgey;

#endif // POKE_SPRITES_H
