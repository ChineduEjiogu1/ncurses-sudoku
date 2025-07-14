#include "../include/sudoku.h"
#include "../include/generator.h"
#include "../include/solver.h"
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

        swap(&array[i], &array[j]);
    }
}

int get_cells_to_remove(difficulty_t difficulty)
{
    switch (difficulty)
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
    // Initialize counters for cell removal process
    int removed_count = 0;
    int cells_to_remove = get_cells_to_remove(difficulty);

    // Set up attempt limiting to prevent infinite loops
    int attempts = 0;
    int max_attempts = cells_to_remove * 10;

    // Step 1: Clear the grid to start with empty puzzle
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            grid[row][col] = 0;
        }
    }

    // Step 2: Generate a complete, valid Sudoku solution
    solve_grid(grid);

    // Step 3: Store the complete solution in solution array
    // This preserves the full solution for reference (e.g., when player requests hint)
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            solution[row][col] = grid[row][col];
        }
    }

    // Step 4: Intelligently remove cells while maintaining puzzle uniqueness
    while (removed_count < cells_to_remove && attempts < max_attempts)
    {
        // Pick a random cell to potentially remove
        int row = rand() % 9;
        int col = rand() % 9;

        // Skip cells that have already been removed (are zero)
        if (grid[row][col] == 0)
        {
            attempts++;
            continue;
        }

        // Store the original value before removal attempt
        int original_value = grid[row][col];

        // Temporarily remove the cell (set to zero)
        grid[row][col] = 0;

        // Test if puzzle still has a unique solution after removal
        if (has_unique_solution(grid))
        {
            // Removal successful - puzzle still has unique solution
            removed_count++;
        }
        else
        {
            // Removal failed - would create multiple solutions or no solution
            // Restore the original value
            grid[row][col] = original_value;
        }

        // Increment attempt counter regardless of success/failure
        attempts++;
    }

    // Step 5: Create the given array to track which cells are clues
    // This helps distinguish between original clues and player-filled cells
    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            if (grid[row][col] != 0)
            {
                given[row][col] = 1; // This cell is a given clue
            }
            else
            {
                given[row][col] = 0; // This cell is empty for player to fill
            }
        }
    }

    return 1; // Success
}