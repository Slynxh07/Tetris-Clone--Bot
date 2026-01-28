#include "raylib.h"
#include "game.h"
#include "colors.h"
#include "block.h"

#define HEIGHT 620
#define WIDTH 500

Block *currentBlock;
Block *nextBlock;
Block *holdBlock;

void updateGame();
void drawGame();

void initGame()
{
    InitWindow(WIDTH, HEIGHT, "Teris");
    SetTargetFPS(60);
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

}

void drawGame()
{
    BeginDrawing();

    ClearBackground(backGround);

    DrawRectangleRounded((Rectangle){ 320, 55, 170, 60 }, 0.3, 6, lightBlue);

    DrawRectangleRounded((Rectangle){ 320, 165, 170, 180 }, 0.3, 6, lightBlue);
    DrawRectangleRounded((Rectangle){ 320, 385, 170, 180 }, 0.3, 6, lightBlue);

    EndDrawing();
}