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
    if (g != NULL) free(g);
}

void printGrid(const Grid *g)
{
    for (int row = 0; row < NUM_ROWS; row++)
    {
        for (int col = 0; col < NUM_COLS; col++)
        {
            printf("%d ", g->grid[row][col]);
        }
        printf("\n");
    }
}

void drawTGrid(const Grid *g)
{
    for (int row = 0; row < NUM_ROWS; row++)
    {
        for (int col = 0; col < NUM_COLS; col++)
        {
            int cellValue = g->grid[row][col];
            DrawRectangle(col * CELL_SIZE + 11, row * CELL_SIZE + 11, CELL_SIZE - 1, CELL_SIZE - 1, brickColors[cellValue]);
        }
    }
}

int isCellEmpty(const Grid *g, const int row, const int col)
{
    return g->grid[row][col] == 0;
}

int isCellOutside(const int row, const int col)
{
    if (row < 0 || row >= NUM_ROWS || col < 0 || col >= NUM_COLS) return 1;
    return 0;
}

int isRowFull(const Grid *g, int row)
{
    for (int i = 0; i < NUM_COLS; i++)
    {
        if (g->grid[row][i] == 0) return 0;
    }
    return 1;
}

void clearRow(Grid *g, int row)
{
    for (int i = 0; i < NUM_COLS; i++)
    {
        g->grid[row][i] = 0;
    }
}

void moveRowDown(Grid *g, int row, int numRows)
{
	for (int i = 0; i < NUM_COLS; i++)
	{
		g->grid[row + numRows][i] = g->grid[row][i];
		g->grid[row][i] = 0;
	}
}

int clearFullRows(Grid *g)
{
    int clearRows = 0;
	for (int i = NUM_ROWS - 1; i >= 0; i--)
	{
		if (isRowFull(g, i))
		{
			clearRow(g, i);
			clearRows++;
		}
		else if (clearRows > 0)
		{
			moveRowDown(g, i, clearRows);
		}
	}
	return clearRows;
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

void setCellValue(Grid *g, const int row, const int col, const int val)
{
    if (isCellOutside(row, col)) return;
    g->grid[row][col] = val;
}

int countHoles(Grid *g)
{
    int holes = 0;

    for (int col = 0; col < NUM_COLS; col++)
    {
        int blockFound = 0;

        for (int row = 0; row < NUM_ROWS; row++)
        {
            if (g->grid[row][col])
            {
                blockFound = 1;
            }
            else if (blockFound)
            {
                holes++;
            }
        }
    }

    return holes;
}

int getMaxHeight(Grid *g)
{
    int maxHeight = 0;

    for (int col = 0; col < NUM_COLS; col++)
    {
        for (int row = 0; row < NUM_ROWS; row++)
        {
            if (g->grid[row][col])
            {
                int height = NUM_ROWS - row;
                if (height > maxHeight) maxHeight = height;
                break;
            }
        }
    }

    return maxHeight;
}

int getAggHeight(Grid *g)
{
    int sum = 0;

    for (int col = 0; col < NUM_COLS; col++)
    {

        int height = 0;

        for (int row = 0; row < NUM_ROWS; row++)
        {
            if (g->grid[row][col])
            {
                height = NUM_ROWS - row;
                break;
            }
        }

        sum += height;
    }

    return sum;
}

int getBumpiness(Grid *g)
{
    int heights[NUM_COLS] = {0};

    for (int col = 0; col < NUM_COLS; col++)
    {
        for (int row = 0; row < NUM_ROWS; row++)
        {
            if (g->grid[row][col])
            {
                heights[col] = NUM_ROWS - row;
                break;
            }
        }
    }

    int bumpiness = 0;
    for (int col = 0; col < NUM_COLS - 1; col++)
    {
        int diff = heights[col] - heights[col + 1];
        if (diff < 0) diff = -diff;
        bumpiness += diff;
    }

    return bumpiness;
}

int getWells(Grid *g)
{
    int heights[NUM_COLS] = {0};

    for (int col = 0; col < NUM_COLS; col++)
    {
        for (int row = 0; row < NUM_ROWS; row++)
        {
            if (g->grid[row][col])
            {
                heights[col] = NUM_ROWS - row;
                break;
            }
        }
    }

    int wells = 0;

    for (int col = 0; col < NUM_COLS; col++)
    {
        int left = (col == 0) ? heights[col] + 1 : heights[col - 1];
        int right = (col == NUM_COLS - 1) ? heights[col] + 1 : heights[col + 1];

        if (left > heights[col] && right > heights[col])
        {
            int depth = (left < right ? left : right) - heights[col];
            wells += depth;
        }
    }

    return wells;
}