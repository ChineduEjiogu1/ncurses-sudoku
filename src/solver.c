#include "../include/sudoku.h"
#include "../include/generator.h"
#include <ncursesw/ncurses.h>

/**
 * Find the first empty cell (value 0) in the grid
 * Uses row-major order (left to right, top to bottom)
 * 
 * Parameters:
 *   grid - 9x9 Sudoku grid
 *   row  - pointer to store found row index
 *   col  - pointer to store found column index
 * 
 * Returns: 1 if empty cell found, 0 if grid is complete
 */
int find_empty_cell(int grid[9][9], int *row, int *col)
{
    for (int x = 0; x < GRID_SIZE; x++)
    {
        for (int y = 0; y < GRID_SIZE; y++)
        {
            if (grid[x][y] == 0) // Found empty cell
            {
                *row = x; // Store row position
                *col = y; // Store column position
                return 1; // Success
            }
        }
    }

    return 0; // No empty cells found - grid is complete
}

/**
 * Recursive helper function to count all possible solutions
 * Used to determine if a puzzle has a unique solution
 * Stops counting after finding 2+ solutions for efficiency
 * 
 * Parameters:
 *   grid    - 9x9 Sudoku grid (modified during recursion)
 *   counter - pointer to solution count
 * 
 * Returns: 1 if 2+ solutions found (early termination), 0 otherwise
 */
int count_solutions_helper(int grid[9][9], int *counter)
{
    int row = 0, col = 0;

    // Base case: no empty cells means we found a complete solution
    if (!(find_empty_cell(grid, &row, &col)))
    {
        (*counter)++; // Increment solution count

        // Early termination: if we found 2+ solutions, puzzle isn't unique
        if (*counter >= 2)
        {
            return 1; // Signal early termination
        }

        return 0; // Continue searching for more solutions
    }

    // Try all possible numbers (1-9) in the current empty cell
    for (int i = 0; i < GRID_SIZE; i++)
    {
        int guess = i + 1; // Convert 0-8 to 1-9

        // Check if this number is valid at current position
        if (is_valid_placement(grid, row, col, guess))
        {
            grid[row][col] = guess; // Place the number
            
            // Recursively solve with this number placed
            if (count_solutions_helper(grid, counter) == 1)
            {
                return 1; // Early termination signal received
            }

            grid[row][col] = 0; // Backtrack - remove the number
        }
    }

    return 0; // No early termination needed
}

/**
 * Solve a Sudoku puzzle using backtracking algorithm
 * Modifies the input grid to contain the solution
 * 
 * Parameters:
 *   grid - 9x9 Sudoku grid to solve
 * 
 * Returns: 1 if solution found, 0 if unsolvable
 */
int solve_grid(int grid[9][9])
{
    int row = 0, col = 0;

    // Base case: no empty cells means puzzle is solved
    if (!find_empty_cell(grid, &row, &col))
    {
        return 1; // Success - puzzle is complete
    }

    // Try each number 1-9 in the current empty cell
    for (int i = 0; i < GRID_SIZE; i++)
    {
        int guess = i + 1; // Convert 0-8 to 1-9

        // Check if this number placement is valid
        if (is_valid_placement(grid, row, col, guess))
        {
            grid[row][col] = guess; // Place the number
            
            // Recursively solve the rest of the puzzle
            if (solve_grid(grid))
                return 1; // Solution found in deeper recursion
                
            grid[row][col] = 0; // Backtrack - this path didn't work
        }
    }

    return 0; // No valid solution found from this state
}

/**
 * Check if placing a number at a specific position violates Sudoku rules
 * Validates row, column, and 3x3 box constraints
 * 
 * Parameters:
 *   grid - 9x9 Sudoku grid
 *   row  - target row (0-8)
 *   col  - target column (0-8)
 *   num  - number to place (1-9)
 * 
 * Returns: 1 if placement is valid, 0 if it violates rules
 */
int is_valid_placement(int grid[9][9], int row, int col, int num)
{
    // Check column constraint: no duplicate in same column (excluding current cell)
    for (int x = 0; x < GRID_SIZE; x++)
    {
        if (x != row && grid[x][col] == num)  // Added: x != row
        {
            return 0; // Found duplicate in column
        }
    }

    // Check row constraint: no duplicate in same row (excluding current cell)
    for (int y = 0; y < GRID_SIZE; y++)
    {
        if (y != col && grid[row][y] == num)  // Added: y != col
        {
            return 0; // Found duplicate in row
        }
    }

    // Check 3x3 box constraint: no duplicate in same box (excluding current cell)
    int start_row = (row / 3) * 3; // Top-left corner of box
    int start_col = (col / 3) * 3; // Top-left corner of box

    for (int x = start_row; x < start_row + BOX_SIZE; x++)
    {
        for (int y = start_col; y < start_col + BOX_SIZE; y++)
        {
            if ((x != row || y != col) && grid[x][y] == num)  // Added: (x != row || y != col)
            {
                return 0; // Found duplicate in 3x3 box
            }
        }
    }

    return 1; // All constraints satisfied - valid placement
}

/**
 * Check if a Sudoku puzzle has exactly one unique solution
 * Makes a copy of the grid to avoid modifying the original
 * 
 * Parameters:
 *   grid - 9x9 Sudoku grid to test
 * 
 * Returns: 1 if unique solution exists, 0 otherwise
 */
int has_unique_solution(int grid[9][9])
{
    int grid_copy[9][9];

    // Create a copy to avoid modifying original grid
    for (int i = 0; i < 9; i++) 
    {
        for (int j = 0; j < 9; j++) 
        {
            grid_copy[i][j] = grid[i][j];
        }
    }
    
    // Count solutions using the copy
    int counter = 0;
    count_solutions_helper(grid_copy, &counter);
    
    // Return true only if exactly one solution exists
    if (counter == 1) {
        return 1; // Unique solution
    } else {
        return 0; // Multiple solutions or no solution
    }
}

/**
 * Count the total number of solutions for a Sudoku puzzle
 * Primarily used for puzzle validation and generation
 * 
 * Parameters:
 *   grid - 9x9 Sudoku grid to analyze
 * 
 * Returns: number of possible solutions
 */
int count_solutions(int grid[9][9])
{
    int counter = 0;
    count_solutions_helper(grid, &counter);
    return counter;
}

/**
 * Validate if the current state of a Sudoku grid is legal
 * Checks all filled cells for rule violations
 * Does NOT require the grid to be complete
 * 
 * Parameters:
 *   grid - 9x9 Sudoku grid to validate
 * 
 * Returns: 1 if current state is valid, 0 if rules are violated
 */
int is_grid_valid(int grid[9][9])
{
    // Check every filled cell for violations
    for (int x = 0; x < GRID_SIZE; x++)
    {
        for (int y = 0; y < GRID_SIZE; y++)
        {
            if (grid[x][y] == 0) // Skip empty cells
                continue;

            // Check for duplicates in the same row
            for (int col_idx = 0; col_idx < GRID_SIZE; col_idx++)
            {
                if (col_idx != y && grid[x][col_idx] == grid[x][y])
                {
                    return 0; // Duplicate found in row
                }
            }

            // Check for duplicates in the same column
            for (int row_idx = 0; row_idx < GRID_SIZE; row_idx++)
            {
                if (row_idx != x && grid[row_idx][y] == grid[x][y])
                {
                    return 0; // Duplicate found in column
                }
            }

            // Check for duplicates in the same 3x3 box
            int start_row = (x / 3) * 3;
            int start_col = (y / 3) * 3;

            for (int box_row = start_row; box_row < start_row + BOX_SIZE; box_row++)
            {
                for (int box_col = start_col; box_col < start_col + BOX_SIZE; box_col++)
                {
                    // Skip the cell we're currently checking
                    if ((box_row != x || box_col != y) && grid[box_row][box_col] == grid[x][y])
                    {
                        return 0; // Duplicate found in 3x3 box
                    }
                }
            }
        }
    }

    return 1; // No violations found - grid state is valid
}

/**
 * Check if the Sudoku grid is completely filled
 * Does NOT validate if the solution is correct - only checks completeness
 * 
 * Parameters:
 *   grid - 9x9 Sudoku grid to check
 * 
 * Returns: 1 if all cells are filled, 0 if empty cells remain
 */
int is_grid_complete(int grid[9][9])
{
    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            if (grid[row][col] == 0) // Found empty cell
            {
                return 0; // Grid is not complete
            }
        }
    }

    return 1; // All cells are filled
}