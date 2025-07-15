#include "../include/sudoku.h"
#include "../include/generator.h"

int find_empty_cell(int grid[9][9], int *row, int *col)
{
    for (int x = 0; x < GRID_SIZE; x++)
    {
        for (int y = 0; y < GRID_SIZE; y++)
        {
            if (grid[x][y] == 0)
            {
                *row = x;
                *col = y;
                return 1;
            }
        }
    }

    return 0;
}

int count_solutions_helper(int grid[9][9], int *counter)
{
    int row = 0, col = 0;

    if (!(find_empty_cell(grid, &row, &col)))
    {
        (*counter)++;

        if (*counter >= 2)
        {
            return 1;
        }

        return 0;
    }

    for (int i = 0; i < GRID_SIZE; i++)
    {
        int guess = i + 1;

        if (is_valid_placement(grid, row, col, guess))
        {
            grid[row][col] = guess;
            if (count_solutions_helper(grid, counter) == 1)
            {
                return 1;
            }

            grid[row][col] = 0;
        }
    }

    return 0;
}

int solve_grid(int grid[9][9])
{
    int row = 0, col = 0;

    if (!find_empty_cell(grid, &row, &col))
    {
        return 1;
    }

    for (int i = 0; i < GRID_SIZE; i++)
    {
        int guess = i + 1;

        if (is_valid_placement(grid, row, col, guess))
        {
            grid[row][col] = guess;
            if (solve_grid(grid))
                return 1;
            grid[row][col] = 0;
        }
    }

    return 0;
}

int is_valid_placement(int grid[9][9], int row, int col, int num)
{
    for (int x = 0; x < GRID_SIZE; x++)
    {
        if (grid[x][col] == num)
        {
            return 0;
        }
    }

    for (int y = 0; y < GRID_SIZE; y++)
    {
        if (grid[row][y] == num)
        {
            return 0;
        }
    }

    int start_row = (row / 3) * 3;
    int start_col = (col / 3) * 3;

    for (int x = start_row; x < start_row + BOX_SIZE; x++)
    {
        for (int y = start_col; y < start_col + BOX_SIZE; y++)
        {
            if (grid[x][y] == num)
            {
                return 0;
            }
        }
    }

    return 1;
}

int has_unique_solution(int grid[9][9])
{
    int grid_copy[9][9];

    for (int i = 0; i < 9; i++) 
    {
        for (int j = 0; j < 9; j++) 
        {
            grid_copy[i][j] = grid[i][j];
        }
    }
    
    int counter = 0;
    count_solutions_helper(grid_copy, &counter);
    
    if (counter == 1) {
        return 1;
    } else {
        return 0;
    }
}

int count_solutions(int grid[9][9])
{
    int counter = 0;
    count_solutions_helper(grid, &counter);
    return counter;
}

int is_grid_valid(int grid[9][9])
{
    for (int x = 0; x < GRID_SIZE; x++)
    {
        for (int y = 0; y < GRID_SIZE; y++)
        {
            if (grid[x][y] == 0)
                continue;

            for (int col_idx = 0; col_idx < GRID_SIZE; col_idx++)
            {
                if (col_idx != y && grid[x][col_idx] == grid[x][y])
                {
                    return 0;
                }
            }

            for (int row_idx = 0; row_idx < GRID_SIZE; row_idx++)
            {
                if (row_idx != x && grid[row_idx][y] == grid[x][y])
                {
                    return 0;
                }
            }

            int start_row = (x / 3) * 3;
            int start_col = (y / 3) * 3;

            for (int box_row = start_row; box_row < start_row + BOX_SIZE; box_row++)
            {
                for (int box_col = start_col; box_col < start_col + BOX_SIZE; box_col++)
                {
                    if ((box_row != x || box_col != y) && grid[box_row][box_col] == grid[x][y])
                    {
                        return 0;
                    }
                }
            }
        }
    }

    return 1;
}

int is_grid_complete(int grid[9][9])
{
    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            if (grid[row][col] == 0)
            {
                return 0;
            }
        }
    }

    return 1;
}