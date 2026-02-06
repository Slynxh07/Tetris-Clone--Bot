#include "block.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "colors.h"

#define NUM_CELLS 4
#define CELL_SIZE 30

static const Position ORANGE_RICKY_CELLS[NUM_CELLS][NUM_CELLS] = {
    { {0,2}, {1,0}, {1,1}, {1,2} },
    { {0,1}, {1,1}, {2,1}, {2,2} },
    { {1,0}, {1,1}, {1,2}, {2,0} },
    { {0,0}, {0,1}, {1,1}, {2,1} }
};

static const Position BLUE_RICKY_CELLS[NUM_CELLS][NUM_CELLS] = {
    { {0,2}, {1,0}, {1,1}, {1,2} },
    { {0,1}, {1,1}, {2,1}, {2,2} },
    { {1,0}, {1,1}, {1,2}, {2,0} },
    { {0,0}, {0,1}, {1,1}, {2,1} }
};

static const Position CLEVELAND_Z_CELLS[NUM_CELLS][NUM_CELLS] = {
    { {0,2}, {1,0}, {1,1}, {1,2} },
    { {0,1}, {1,1}, {2,1}, {2,2} },
    { {1,0}, {1,1}, {1,2}, {2,0} },
    { {0,0}, {0,1}, {1,1}, {2,1} }
};

static const Position RHODE_ISLAND_Z_CELLS[NUM_CELLS][NUM_CELLS] = {
    { {0,2}, {1,0}, {1,1}, {1,2} },
    { {0,1}, {1,1}, {2,1}, {2,2} },
    { {1,0}, {1,1}, {1,2}, {2,0} },
    { {0,0}, {0,1}, {1,1}, {2,1} }
};

static const Position HERO_CELLS[NUM_CELLS][NUM_CELLS] = {
    { {0,2}, {1,0}, {1,1}, {1,2} },
    { {0,1}, {1,1}, {2,1}, {2,2} },
    { {1,0}, {1,1}, {1,2}, {2,0} },
    { {0,0}, {0,1}, {1,1}, {2,1} }
};

static const Position TEEWEE_CELLS[NUM_CELLS][NUM_CELLS] = {
    { {0,2}, {1,0}, {1,1}, {1,2} },
    { {0,1}, {1,1}, {2,1}, {2,2} },
    { {1,0}, {1,1}, {1,2}, {2,0} },
    { {0,0}, {0,1}, {1,1}, {2,1} }
};

static const Position SMASHBOY_CELLS[NUM_CELLS][NUM_CELLS] = {
    { {0,2}, {1,0}, {1,1}, {1,2} },
    { {0,1}, {1,1}, {2,1}, {2,2} },
    { {1,0}, {1,1}, {1,2}, {2,0} },
    { {0,0}, {0,1}, {1,1}, {2,1} }
};

typedef struct Block {
    Position cells[NUM_CELLS][NUM_CELLS];
    BLOCK_TYPE blockType;
    int rowOffset;
    int colOffset;
    int rotateState;
} Block;

void getCellPositions(const Block *b, Position out[NUM_CELLS]);

Block *createBlock(BLOCK_TYPE blockType)
{
    Block *b = malloc(sizeof(Block));
    b->blockType = blockType;
    switch (blockType)
    {
    case ORANGE_RICKY:
        memcpy(b->cells, ORANGE_RICKY_CELLS, sizeof(b->cells));
        break;
    case BLUE_RICKY:
        memcpy(b->cells, BLUE_RICKY_CELLS, sizeof(b->cells));
        break;
    case CLEVELAND_Z:
        memcpy(b->cells, CLEVELAND_Z_CELLS, sizeof(b->cells));
        break;
    case RHODE_ISLAND_Z:
        memcpy(b->cells, RHODE_ISLAND_Z_CELLS, sizeof(b->cells));
        break;
    case HERO:
        memcpy(b->cells, HERO_CELLS, sizeof(b->cells));
        break;
    case TEEWEE:
        memcpy(b->cells, TEEWEE_CELLS, sizeof(b->cells));
        break;
    case SMASHBOY:
        memcpy(b->cells, SMASHBOY_CELLS, sizeof(b->cells));
        break;
    default:
        printf("Error creating block...");
        exit(-1);
        break;
    }
    b->colOffset = 0;
    b->rowOffset = 0;
    b->rotateState = 0;
    return b;
}

BLOCK_TYPE getBlockType(const Block *b)
{
    return b->blockType;
}

void destroyBlock(Block *b)
{
    free(b);
}

void rotate(Block *b)
{
    b->rotateState++;
    if (b->rotateState >= 4) b->rotateState = 0;
}

void move(Block *b, Direction dir)
{
    switch (dir)
    {
        case DOWN:
            b->rowOffset++;
            break;
        case LEFT:
            b->colOffset--;
            break;
        case RIGHT:
            b->colOffset++;
            break;
        default:
            printf("Invlaid input, don't know how you got here...");
            exit(-1);
            break;
    }
}

void drawBlock(const Block *b, int xOffset, int yOffset)
{
    Position newPos[NUM_CELLS];
    getCellPositions(b, newPos);
    for (int i = 0; i < NUM_CELLS; i++)
    {
        DrawRectangle(newPos[i].col * CELL_SIZE + xOffset, newPos[i].row * CELL_SIZE + yOffset, CELL_SIZE - 1, CELL_SIZE - 1, brickColors[b->blockType]);
    }
}

void getCellPositions(const Block *b, Position out[NUM_CELLS])
{
    for (int i = 0; i < NUM_CELLS; i++)
    {
        Position tile = b->cells[b->rotateState][i];

        out[i].row = tile.row + b->rowOffset;
        out[i].col = tile.col + b->colOffset;
    }
}

int checkValidMove(const Block *b, const Direction dir, const Grid *g)
{
    switch (dir)
    {
        case DOWN:
            if (!isCellEmpty(g, b->rowOffset + 1, b->colOffset) && !isCellOutside(g, b->rowOffset + 1, b->colOffset)) return 0;
            break;
        case LEFT:
            if (!isCellEmpty(g, b->rowOffset, b->colOffset - 1) && !isCellOutside(g, b->rowOffset, b->colOffset - 1)) return 0;
            break;
        case RIGHT:
            if (!isCellEmpty(g, b->rowOffset, b->colOffset + 1) && !isCellOutside(g, b->rowOffset + 1, b->colOffset + 1)) return 0;
            break;
    }
    return 1;
}

void lockBlock(const Block *b, Grid *g)
{
    if (setCellValue(g, b->rowOffset, b->colOffset, b->blockType))
    {
        return;
    }
}