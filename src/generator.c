#include "../include/sudoku.h"
#include "../include/generator.h"
#include "../include/solver.h"
#include <time.h>

/**
 * Swap two integer values using pointers
 * Utility function for array shuffling
 * 
 * Parameters:
 *   a - pointer to first integer
 *   b - pointer to second integer
 */
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Shuffle an array using Fisher-Yates algorithm
 * Ensures random, unbiased permutation of array elements
 * 
 * Parameters:
 *   array - array to shuffle
 *   num   - number of elements in array
 */
void shuffle_array(int *array, int num)
{
    // Iterate backwards through array
    for (int i = num - 1; i > 0; i--)
    {
        // Pick random element from remaining unshuffled portion
        int j = rand() % (i + 1);

        // Swap current element with randomly selected element
        swap(&array[i], &array[j]);
    }
}

/**
 * Generate a complete, valid Sudoku grid using randomized backtracking
 * Creates a fully solved 9x9 grid that satisfies all Sudoku rules
 * Uses shuffled number arrays to ensure random grid generation
 * 
 * Parameters:
 *   grid - 9x9 array to fill with complete solution
 * 
 * Returns: 1 if successful generation, 0 if failed
 */
int generate_complete_grid(int grid[9][9])
{
    int row = 0, col = 0;

    // Create array of numbers 1-9 and shuffle for randomness
    int numbers[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    shuffle_array(numbers, 9);

    // Base case: if no empty cells found, grid is complete
    if (!find_empty_cell(grid, &row, &col))
    {
        return 1; // Success - grid is fully generated
    }

    // Try each shuffled number in the current empty cell
    for (int i = 0; i < GRID_SIZE; i++) 
    {
        int guess = numbers[i]; // Use shuffled order for randomness

        // Check if this number placement follows Sudoku rules
        if (is_valid_placement(grid, row, col, guess))
        {
            grid[row][col] = guess; // Place the number
            
            // Recursively fill the rest of the grid
            if (generate_complete_grid(grid))
                return 1; // Successfully completed grid
                
            grid[row][col] = 0; // Backtrack - remove number and try next
        }
    }

    return 0; // No valid number worked - backtrack further
}

/**
 * Determine number of cells to remove based on difficulty level
 * More removed cells = harder puzzle (fewer clues to work with)
 * 
 * Parameters:
 *   difficulty - enum value indicating desired puzzle difficulty
 * 
 * Returns: number of cells that should be removed from complete grid
 */
int get_cells_to_remove(difficulty_t difficulty)
{
    switch (difficulty)
    {
        case EASY:
            return 45; // Remove 45 cells, leaving 36 clues
        case MEDIUM:
            return 49; // Remove 49 cells, leaving 32 clues
        case HARD:
            return 51; // Remove 51 cells, leaving 30 clues
        case EXPERT:
            return 53; // Remove 53 cells, leaving 28 clues
        default:
            return 45; // Default to easy if unknown difficulty
    }
}

/**
 * Generate a complete Sudoku puzzle with specified difficulty
 * Creates puzzle grid, solution grid, and given array
 * Ensures puzzle has exactly one unique solution
 * 
 * Parameters:
 *   grid       - 9x9 array for the puzzle (with cells removed)
 *   solution   - 9x9 array storing the complete solution
 *   given      - 9x9 array marking which cells are original clues
 *   difficulty - desired puzzle difficulty level
 * 
 * Returns: 1 if successful generation, 0 if failed
 */
int generate_puzzle(int grid[9][9], int solution[9][9], int given[9][9], difficulty_t difficulty)
{
    srand(time(NULL)); // Seed random number generator with current time

    // Initialize counters for cell removal process
    int removed_count = 0;
    int cells_to_remove = get_cells_to_remove(difficulty);

    // Set up attempt limiting to prevent infinite loops
    int attempts = 0;
    int max_attempts = cells_to_remove * 10; // Allow reasonable retry limit

    // Step 1: Clear the grid to start with empty puzzle
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            grid[row][col] = 0; // Initialize all cells to empty
        }
    }

    // Step 2: Generate a complete, valid Sudoku solution
    generate_complete_grid(grid);

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
            continue; // Try a different cell
        }

        // Store the original value before removal attempt
        int original_value = grid[row][col];

        // Temporarily remove the cell (set to zero)
        grid[row][col] = 0;

        // Test if puzzle still has a unique solution after removal
        if (has_unique_solution(grid))
        {
            // Removal successful - puzzle still has unique solution
            removed_count++; // Keep it removed
        }
        else
        {
            // Removal failed - would create multiple solutions
            grid[row][col] = original_value; // Put the number back
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
                given[row][col] = 1; // This cell is a given clue (unchangeable)
            }
            else
            {
                given[row][col] = 0; // This cell is empty for player to fill
            }
        }
    }

    return 1; // Success - puzzle generation complete
}