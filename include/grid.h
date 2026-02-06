#ifndef GRID_H
#define GRID_H

#define NUM_ROWS 20
#define NUM_COLS 10

typedef struct Grid
{
    int grid[NUM_ROWS][NUM_COLS];
} Grid;

Grid *createGrid();
void destroyGrid(Grid *g);
int isCellEmpty(const Grid *g, const int row, const int col);
int isCellOutside(const Grid *g, const int row, const int col);
void setCellValue(Grid *g, const int row, const int col, const int val);

#endif