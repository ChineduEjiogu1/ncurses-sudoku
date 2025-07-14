#include "sudoku.h"
#include "generator.h"
#include <time.h>

void swap(int *a, int *b) 
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void shuffle_array(int *array, int num)
{
    srand(time(NULL));

    for (int i = num - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);

        swap(&array[i], array[j]);
    }
}

int find_empty_cell(int grid[9][9], int *row, int *col)
{
    for (int x = 0; x < 9; x++)
    {
        for (int y = 0; y < 9; y++)
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

int solve_grid(int grid[9][9])
{
    int numbers[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    int row, col;

    if (!find_empty_cell(grid, &row, &col)) 
    {
        return 1;
    }

    shuffle_array(numbers, 9);

    for (int i = 0; i < 9; i++)
    {
        int guess = numbers[i];
        
        if (is_valid_placement(grid, row, col, guess)) 
        {
            grid[row][col] = guess;
            if(solve_grid(grid)) return 1;
            grid[row][col] = 0;
        }
    }

    return 0;
}

int is_valid_placement(int grid[9][9], int row, int col, int num)
{
    for (int x = 0; x < 9; x++)
    {
        if (grid[x][col] == num)
        {
            return false;
        }
    }

    for (int y = 0; y < 9; y++)
    {
        if (grid[row][y] == num)
        {
            return false;
        }
    }

    int start_row = (row / 3) * 3;
    int start_col = (col / 3) * 3;

    for (int x = start_row; x < start_row + 3; x++)
    {
        for (int y = start_col; y < start_col + 3; y++)
        {
            if (grid[x][y] == num)
            {
                return false;
            }
        }
    }

    return true;
}


int get_cells_to_remove(difficulty_t difficulty)
{
    switch(difficulty) 
    {
        case EASY:
            return 45;
        case MEDIUM:
            return 49;
        case HARD:
            return 51;
        case EXPERT:
            return 53;
        default:
            return 45;
    }
}

int generate_puzzle(int grid[9][9], int solution[9][9], int given[9][9], difficulty_t difficulty)
{
    for (int row = 0; row < 9; row++)
    {
        for(int col = 0; col < 9; col++)
        {
            grid[row][col] = 0;
        }
    }
    
    solve_grid(grid);

    for (int row = 0; row < 9; row++)
    {
        for(int col = 0; col < 9; col++)
        {
            solution[row][col] = grid[row][col];
        }
    }
    
    int cells_to_remove = get_cells_to_remove(difficulty);

    for (int i = 0; i < cells_to_remove; i++) 
    {
        int row, col;

        do {
            row = rand() % 9;
            col = rand() % 9;
        } while (grid[row][col] == 0);  
        
        grid[row][col] = 0; 
    }

    for (int row = 0; row < 9; row++) 
    {
        for (int col = 0; col < 9; col++) 
        {
            if (grid[row][col] != 0) 
            {
                given[row][col] = 1;  
            }
            else 
            {
                given[row][col] = 0;
            }
        }
    }

    return 1;
}