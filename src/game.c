#include <stdlib.h>
#include "raylib.h"
#include "game.h"
#include "colors.h"
#include "block.h"
#include <time.h>

#define HEIGHT 620
#define WIDTH 500

#define BAG_SIZE 7

Block *currentBlock;
Block *nextBlock;
Block *holdBlock;

Grid *grid;

int keyPressed;

Block *bag[BAG_SIZE];

void updateGame();
void drawGame();
void initBlocks();
void shuffle();
Block *getNextBlock();

void initGame()
{
    InitWindow(WIDTH, HEIGHT, "Teris");
    SetTargetFPS(60);
    srand(time(NULL));
    initBlocks();
    currentBlock = getNextBlock();
    nextBlock = getNextBlock();
    grid = createGrid();
}

void runGame()
{
    while (!WindowShouldClose())
    {
        updateGame();
        drawGame();
    }
}

void closeGame()
{
    //free memory
}

void updateGame()
{
    keyPressed = GetKeyPressed();
    switch (keyPressed)
    {
        case KEY_UP:
            rotate(currentBlock);
            break;
        case KEY_DOWN:
            if (checkValidMove(currentBlock, DOWN, grid))
            {
                move(currentBlock, DOWN);
            }
            else 
            {
                lockBlock(currentBlock, grid);
            }
            break;
        case KEY_RIGHT:
            if (checkValidMove(currentBlock, RIGHT, grid))
            {
                move(currentBlock, RIGHT);
            }
            break;
        case KEY_LEFT:
            if (checkValidMove(currentBlock, LEFT, grid))
            {
                move(currentBlock, LEFT);   
            }
            break; 
    }
}

void drawGame()
{
    BeginDrawing();

    ClearBackground(backGround);

    DrawRectangleRounded((Rectangle){ 320, 55, 170, 60 }, 0.3, 6, lightBlue);

    DrawRectangleRounded((Rectangle){ 320, 165, 170, 180 }, 0.3, 6, lightBlue);
    DrawRectangleRounded((Rectangle){ 320, 385, 170, 180 }, 0.3, 6, lightBlue);

    drawBlock(currentBlock, 11, 11);

    EndDrawing();
}

void initBlocks()
{
    for (int i = 0; i < BAG_SIZE; i++)
    {
        bag[i] = createBlock((BLOCK_TYPE)(i + 1));
    }
}

void shuffle()
{
    for (int i = BAG_SIZE - 1; i > 0; i--) 
    {
        int j = rand() % (i + 1);
        Block *tmp = bag[i];
        bag[i] = bag[j];
        bag[j] = tmp;
    }
}

Block *getNextBlock()
{
    Block *retBlock;
    for (int i = 0; i < BAG_SIZE; i++)
    {
        if (bag[i] == NULL) continue;
        retBlock = bag[i]; 
        bag[i] = NULL;   
        return retBlock;
    }
    initBlocks();
    return getNextBlock();
}