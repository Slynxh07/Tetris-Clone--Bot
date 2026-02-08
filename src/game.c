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
double lastUpdateTime = 0;

Block *bag[BAG_SIZE];

void updateGame();
void drawGame();
void initBlocks();
void shuffle();
void endMove();
Block *getNextBlock();

int eventTriggered(double interval)
{
	double currentTime = GetTime();
	if (currentTime - lastUpdateTime >= interval)
	{
		lastUpdateTime = currentTime;
		return 1;
	}
	return 0;
}

void initGame()
{
    InitWindow(WIDTH, HEIGHT, "Teris");
    SetTargetFPS(60);
    srand(time(NULL));
    initBlocks();
    shuffle();
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
    for (int i = 0; i < BAG_SIZE; i++)
    {
        destroyBlock(bag[i]);
    }
    destroyBlock(holdBlock);
    destroyBlock(nextBlock);
    destroyBlock(currentBlock);
    destroyGrid(grid);
}

void updateGame()
{
    if (eventTriggered(1))
    {
        if (checkValidMove(currentBlock, DOWN, grid))
        {
            move(currentBlock, DOWN);
        }
        else 
        {
            endMove();
        }
    }

    keyPressed = GetKeyPressed();
    switch (keyPressed)
    {
        case KEY_UP:
            if(checkValidRotation(currentBlock, grid))
            {
                rotate(currentBlock);
            }
            break;
        case KEY_DOWN:
            if (checkValidMove(currentBlock, DOWN, grid))
            {
                move(currentBlock, DOWN);
            }
            else 
            {
                endMove();
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

    drawTGrid(grid);
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
    shuffle();
    return getNextBlock();
}

void endMove()
{
    lockBlock(currentBlock, grid);
    destroyBlock(currentBlock);
    currentBlock = nextBlock;
    nextBlock = getNextBlock();
    int rowsCleared = clearFullRows(grid);
}