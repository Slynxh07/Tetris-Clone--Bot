#include "grid.h"
#include <stdlib.h>
#include "raylib.h"
#include "colors.h"
#include <stdio.h>

#define CELL_SIZE 30

Grid *createGrid()
{
    Grid *g = malloc(sizeof(Grid));
    for (int i = 0; i < NUM_ROWS; i++)
    {
        for(int j = 0; j < NUM_COLS; j++)
        {
            g->grid[i][j] = 0;
        }
    }
    return g;
}

void destroyGrid(Grid *g)
{
    free(g);
}

int isCellEmpty(const Grid *g, const int row, const int col)
{
    return g->grid[row][col] == 0;
}

int isCellOutside(const Grid *g, const int row, const int col)
{
    if (row < 0 || row >= NUM_ROWS || col < 0 || col >= NUM_COLS) return 1;
    return 0;
}

void drawGrid(const Grid *g)
{
    for (int i = 0; i < NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_COLS; j++)
        {
            DrawRectangle(CELL_SIZE * j, CELL_SIZE * i, CELL_SIZE, CELL_SIZE, brickColors[g->grid[i][j]]);
        }
    }
}

void printGrid(Grid *g)
{
    for (int i = 0; i < NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_COLS; j++)
        {
            printf("%d ", g->grid[i][j]);
        }
        printf("\n");
    }
}

void setCellValue(Grid *g, const int row, const int col, const int val)
{
    g->grid[row][col] = val;
    printGrid(g);
}
