#ifndef BLOCK_H
#define BLOCK_H

#define SWAP_BLOCK_PTR(a, b) do { Block *tmp = (a); (a) = (b); (b) = tmp; } while (0)

typedef struct Block Block;

typedef enum BLOCK_TYPE {
    ORANGE_RICKY = 0,
    BLUE_RICKY,
    CLEVELAND_Z,
    RHODE_ISLAND_Z,
    HERO,
    TEEWEE,
    SMASHBOY
} BLOCK_TYPE;

typedef struct Posistion
{
    int x, y;
} Posistion;

Block *createBlock(BLOCK_TYPE blockType);
void destroyBlock(Block *b);

#endif