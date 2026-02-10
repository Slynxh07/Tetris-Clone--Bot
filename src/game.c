#include <stdlib.h>
#include "raylib.h"
#include "game.h"
#include "colors.h"
#include "block.h"
#include <time.h>
#include <stdio.h>
#include "levels.h"

#define HEIGHT 620
#define WIDTH 500

#define BAG_SIZE 7

Block *currentBlock;
Block *nextBlock;
Block *holdBlock;
Block *ghostBlock;

Grid *grid;

int keyPressed;
int canHold;
int holding;
double lastUpdateTime = 0;
int score;
int gameOver;
int b2bTetris;
int level;
int totalLinesCleared;
Font font;
Sound music;

BLOCK_TYPE bag[BAG_SIZE];

void updateGame();
void drawGame();
void initBlocks();
void shuffle();
void hold();
void endMove();
void updateScore(int linesCleared, int moveDownPoints);
Block *getNextBlock();
void resetGame();
void changeGhostBlock(BLOCK_TYPE t);

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
    InitAudioDevice();
    SetTargetFPS(60);
    font = LoadFont("Assets/Font/monogram.ttf");
    music = LoadSound("Assets/Sound/Music.wav");
    SetSoundVolume(music, 0.5f);
    PlaySound(music);
    srand(time(NULL));
    initBlocks();
    shuffle();
    currentBlock = getNextBlock();
    nextBlock = getNextBlock();
    ghostBlock = createBlock(getBlockType(currentBlock));
    holdBlock = NULL;
    grid = createGrid();
    score = 0;
    gameOver = 0;
    holding = 0;
    canHold = 1;
    b2bTetris = 0;
    level = 1;
    totalLinesCleared = 0;
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
    destroyBlock(holdBlock);
    destroyBlock(nextBlock);
    destroyBlock(currentBlock);
    destroyBlock(ghostBlock);
    destroyGrid(grid);
    UnloadFont(font);
    UnloadSound(music);
    CloseWindow();
}

void updateGame()
{
    if (!IsSoundPlaying(music))
    {
        PlaySound(music);
    }

    keyPressed = GetKeyPressed();
    resetGhostBlockRow(ghostBlock, currentBlock);
    level = calcCurrentLevel(totalLinesCleared);
    double intervel = getIntervel(level);
    if (!gameOver)
    {

        if (eventTriggered(intervel))
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
        
        switch (keyPressed)
        {
            case KEY_UP:
                if(checkValidRotation(currentBlock, grid))
                {
                    rotate(currentBlock);
                    rotate(ghostBlock);
                }
                break;
            case KEY_DOWN:
                if (checkValidMove(currentBlock, DOWN, grid))
                {
                    move(currentBlock, DOWN);
                    updateScore(0, 1);
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
                    move(ghostBlock, RIGHT);
                }
                break;
            case KEY_LEFT:
                if (checkValidMove(currentBlock, LEFT, grid))
                {
                    move(currentBlock, LEFT);
                    move(ghostBlock, LEFT);
                }
                break; 
            case KEY_SPACE:
                updateScore(0, hardDrop(currentBlock, grid) * 2);
                endMove();
                break;
            case KEY_C:
                if(canHold) hold();
                break;
        }

        hardDrop(ghostBlock, grid);
    }
    else 
    {
        if (keyPressed == KEY_ENTER)
        {
            resetGame();
        }
    }
}

void drawGame()
{
    BeginDrawing();

    ClearBackground(backGround);

    if (gameOver)
	{
		DrawTextEx(font, "GAME OVER", (Vector2) { 320, 570 }, 38, 2, WHITE);
	}

    DrawTextEx(font, "Score", (Vector2) { 365, 15 }, 38, 2, WHITE);
    DrawTextEx(font, "Next", (Vector2) { 370, 125 }, 38, 2, WHITE);
    DrawTextEx(font, "Held", (Vector2) { 370, 345 }, 38, 2, WHITE);
    
    DrawRectangleRounded((Rectangle){ 320, 55, 170, 60 }, 0.3, 6, lightBlue);

    char scoreText[10];
    snprintf(scoreText, sizeof(scoreText), "%d", score);
    Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);

    DrawTextEx(font, scoreText, (Vector2) { 320 + (170 - textSize.x) / 2, 65}, 38, 2, WHITE);

    DrawRectangleRounded((Rectangle){ 320, 165, 170, 180 }, 0.3, 6, lightBlue);
    DrawRectangleRounded((Rectangle){ 320, 385, 170, 180 }, 0.3, 6, lightBlue);

    drawTGrid(grid);
    drawBlock(currentBlock, 11, 11);
    drawGhostBlock(ghostBlock, 11, 11);

    switch (getBlockType(nextBlock))
	{
        case HERO:
            drawBlock(nextBlock, 255, 240);
            break;
        case SMASHBOY:
            drawBlock(nextBlock, 255, 230);
            break;
        default:
            drawBlock(nextBlock, 270, 220);
            break;
	}

    if (holding)
	{
		switch (getBlockType(holdBlock))
		{
            case HERO:
                drawBlock(holdBlock, 255, 460);
                break;
            case SMASHBOY:
                drawBlock(holdBlock, 255, 450);
                break;
            default:
                drawBlock(holdBlock, 270, 440);
                break;
		}
    }
    EndDrawing();
}

void initBlocks()
{
    for (int i = 0; i < BAG_SIZE; i++)
    {
        bag[i] = (BLOCK_TYPE)(i + 1);
    }
}

void shuffle()
{
    for (int i = BAG_SIZE - 1; i > 0; i--) 
    {
        int j = rand() % (i + 1);
        BLOCK_TYPE temp = bag[i];
        bag[i] = bag[j];
        bag[j] = temp;
    }
}

void hold()
{
    canHold = 0;
    resetBlock(currentBlock);
    if (holding)
    {
        SWAP_BLOCK_PTR(holdBlock, currentBlock);
    }
    else
    {
        holdBlock = currentBlock;
        currentBlock = nextBlock;
        nextBlock = getNextBlock();
    }
    changeGhostBlock(getBlockType(currentBlock));
    holding = 1;
}

Block *getNextBlock()
{
    for (int i = 0; i < BAG_SIZE; i++)
    {
        if (bag[i] == 0) continue;
        BLOCK_TYPE t = bag[i];
        bag[i] = 0;
        return createBlock(t);
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
    changeGhostBlock(getBlockType(currentBlock));
    if (!checkValidMove(currentBlock, 0, grid))
    {
        gameOver = 1;
    }
    nextBlock = getNextBlock();
    int rowsCleared = clearFullRows(grid);
    updateScore(rowsCleared, 0);
    totalLinesCleared += rowsCleared;
    canHold = 1;
}

void updateScore(int linesCleared, int moveDownPoints)
{
    int multiplier = 1.5;
    switch (linesCleared)
    {
        case 1:
            score += 100 * level;
            b2bTetris = 0;
            break;
        case 2:
            score += 300 * level;
            b2bTetris = 0;
            break;
        case 3:
            score += 500 * level;
            b2bTetris = 0;
            break;
        case 4:
            if (b2bTetris)
            {
                score += (800 * level) * multiplier;
            }
            else
            {
                score += 800 * level;
                b2bTetris = 1;
            }
            break;
    }
    score += moveDownPoints;
}

void resetGame()
{
    destroyBlock(holdBlock);
    destroyBlock(nextBlock);
    destroyBlock(ghostBlock);
    destroyBlock(currentBlock);
    destroyGrid(grid);
    initBlocks();
    shuffle();
    currentBlock = getNextBlock();
    ghostBlock = createBlock(getBlockType(currentBlock));
    nextBlock = getNextBlock();
    holdBlock = NULL;
    grid = createGrid();
    score = 0;
    holding = 0;
    canHold = 1;
    gameOver = 0;
    b2bTetris = 0;
    level = 1;
    totalLinesCleared = 0;
}

void changeGhostBlock(BLOCK_TYPE t)
{
    destroyBlock(ghostBlock);
    ghostBlock = createBlock(t);
}