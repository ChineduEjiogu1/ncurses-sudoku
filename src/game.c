#include "../include/sudoku.h"
#include "../include/game.h"
#include "../include/input.h"
#include "../include/generator.h"
#include "../include/solver.h"
#include <time.h>

/*
 * IMPORTANT NOTE FOR MAIN GAME LOOP:
 * 
 * When implementing your main game loop, you must handle completion timing:
 * 
 * if (is_game_complete(game) && game->completion_time == 0) {
 *     game->completion_time = time(NULL);  // Record completion time
 *     // Handle completion event (show message, pause timer, etc.)
 * }
 * 
 * Also consider incrementing game->moves in your input handling when 
 * valid number placements occur (not just deletions).
 */

/**
 * Initialize a new Sudoku game with specified difficulty
 * Sets up game state and generates the initial puzzle
 * 
 * Parameters:
 *   game       - pointer to game state structure
 *   difficulty - desired puzzle difficulty level
 */
void init_game(game_state_t *game, difficulty_t difficulty)
{
    game->difficulty = difficulty;   // Store difficulty setting
    game->is_paused = 0;            // Game starts unpaused
    game->start_time = 0;           // Timer not started yet
    game->show_marks = 0;           // Start in number entry mode
    game->completion_time = 0;      // No completion time yet

    new_puzzle(game);               // Generate initial puzzle
}

/**
 * Check if current player solution matches the stored solution
 * Compares every cell and updates completion status
 * 
 * Parameters:
 *   game - pointer to game state structure
 */
void check_solution(game_state_t *game)
{
    // Compare every cell in player's grid to the solution
    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            if (game->grid[row][col] != game->solution[row][col])
            {
                game->is_completed = 0; // Found mismatch - not complete
                return;                 // Exit early on first mismatch
            }
        }
    }

    game->is_completed = 1; // All cells match - puzzle complete
}

/**
 * Generate a new puzzle and reset game state
 * Creates fresh puzzle, solution, and given arrays
 * Resets cursor, moves, completion status, and marks
 * 
 * Parameters:
 *   game - pointer to game state structure
 */
void new_puzzle(game_state_t *game)
{
    // Generate new puzzle with current difficulty setting
    generate_puzzle(game->grid, game->solution, game->given, game->difficulty);

    // Reset cursor to top-left corner
    game->cursor_row = 0;
    game->cursor_col = 0;
    
    // Reset game progress tracking
    game->moves = 0;
    game->is_completed = 0;
    game->completion_time = 0;      // Clear any previous completion time

    // Clear all pencil marks for fresh start
    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            for (int marks = 0; marks < GRID_SIZE; marks++)
            {
                game->marks[row][col][marks] = 0;
            }
        }
    }
}

/**
 * Auto-solve the current puzzle by copying solution to grid
 * Fills in all empty cells with correct numbers
 * 
 * Parameters:
 *   game - pointer to game state structure
 */
void solve_puzzle(game_state_t *game)
{
    // Copy complete solution to player's grid
    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            game->grid[row][col] = game->solution[row][col];
        }
    }

    game->is_completed = 1; // Mark as complete since we filled it
}

/**
 * Delete number and marks from current cursor position
 * Only works on editable cells (not given clues)
 * Increments move counter when successful deletion occurs
 * 
 * Parameters:
 *   game - pointer to game state structure
 */
void delete_number(game_state_t *game)
{
    // Check if current cell can be edited (not a given clue)
    if (can_enter_number(game, game->cursor_row, game->cursor_col))
    {
        // Only count as move if cell actually had a number
        if (game->grid[game->cursor_row][game->cursor_col] != 0)
        {
            game->moves++;          // Increment move counter
        }
        
        // Clear the number from current cell
        game->grid[game->cursor_row][game->cursor_col] = 0;

        // Clear all pencil marks for this cell
        for (int i = 0; i < GRID_SIZE; i++)
        {
            game->marks[game->cursor_row][game->cursor_col][i] = 0;
        }
    }
}

/**
 * Toggle between number entry mode and pencil mark mode
 * Affects behavior of number input (place number vs toggle mark)
 * 
 * Parameters:
 *   game - pointer to game state structure
 */
void toggle_marks(game_state_t *game)
{
    game->show_marks = !game->show_marks; // Flip the boolean flag
}

/**
 * Check if placing a number at given position is valid
 * Wrapper around solver's validation function
 * 
 * Parameters:
 *   game - pointer to game state structure
 *   row  - target row (0-8)
 *   col  - target column (0-8)
 *   num  - number to validate (1-9)
 * 
 * Returns: 1 if valid placement, 0 if violates Sudoku rules
 */
int is_valid_move(game_state_t *game, int row, int col, int num)
{
    return is_valid_placement(game->grid, row, col, num);
}

/**
 * Check if the game is complete (all cells filled correctly)
 * Validates solution and returns completion status
 * 
 * Parameters:
 *   game - pointer to game state structure
 * 
 * Returns: 1 if game complete, 0 if not complete
 */
int is_game_complete(game_state_t *game)
{
    check_solution(game);           // Update completion status
    return game->is_completed;      // Return the result
}

/**
 * Check if a specific cell contains a given clue
 * Used to determine if cell is editable
 * 
 * Parameters:
 *   game - pointer to game state structure
 *   row  - target row (0-8)
 *   col  - target column (0-8)
 * 
 * Returns: 1 if cell is given clue, 0 if editable
 */
int is_cell_given(game_state_t *game, int row, int col)
{
    return game->given[row][col];
}

/**
 * Reset game to original puzzle state
 * Restores initial clues and clears player entries
 * Resets all game state except difficulty
 * 
 * Parameters:
 *   game - pointer to game state structure
 */
void reset_game(game_state_t *game)
{
    // Restore original puzzle state
    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            if (game->given[row][col] == 1)
            {
                // Restore original clue from solution
                game->grid[row][col] = game->solution[row][col];
            }
            else
            {
                // Clear player's entry
                game->grid[row][col] = 0;
            }

            // Clear all pencil marks for this cell
            for (int i = 0; i < GRID_SIZE; i++)
            {
                game->marks[row][col][i] = 0;
            }
        }
    }

    // Reset all game state variables
    game->is_paused = 0;            // Unpause game
    game->start_time = 0;           // Clear timer
    game->completion_time = 0;      // Clear completion time
    game->show_marks = 0;           // Back to number mode
    game->cursor_row = 0;           // Reset cursor position
    game->cursor_col = 0;
    game->moves = 0;                // Clear move counter
    game->is_completed = 0;         // Mark as incomplete
}

// ============================================================================
//                               TIMER FUNCTIONS
// ============================================================================

/**
 * Start the game timer
 * Records current time and ensures timer is unpaused
 * 
 * Parameters:
 *   game - pointer to game state structure
 */
void start_timer(game_state_t *game)
{
    game->start_time = time(NULL);  // Record start time
    game->pause_time = 0;           // Clear any previous pause time
    game->is_paused = 0;            // Ensure timer is running
}

/**
 * Pause the game timer
 * Records pause time but only if not already paused
 * 
 * Parameters:
 *   game - pointer to game state structure
 */
void pause_timer(game_state_t *game)
{
    if (!game->is_paused)           // Only pause if not already paused
    {
        game->pause_time = time(NULL);  // Record when pause started
        game->is_paused = 1;            // Mark as paused
    }
}

/**
 * Resume the game timer after pause
 * Adjusts start time to account for pause duration
 * 
 * Parameters:
 *   game - pointer to game state structure
 */
void resume_timer(game_state_t *game)
{
    if (game->is_paused)            // Only resume if currently paused
    {
        time_t now = time(NULL);
        // Shift start_time forward to account for pause duration
        // This keeps elapsed time calculation correct
        game->start_time += (now - game->pause_time);
        game->pause_time = 0;       // Clear pause time
        game->is_paused = 0;        // Mark as running
    }
}

/**
 * Calculate elapsed game time in seconds
 * Handles different timer states (running, paused, completed)
 * 
 * Parameters:
 *   game - pointer to game state structure
 * 
 * Returns: elapsed time in seconds, or 0 if timer not started
 */
int get_elapsed_time(game_state_t *game)
{
    // Return 0 if timer never started
    if (game->start_time == 0)
        return 0;

    // If game completed, return time when it was finished
    if (game->completion_time && game->completion_time != 0)
    {
        return (int)(game->completion_time - game->start_time);
    }

    // If paused, return time up to when pause started
    if (game->is_paused)
    {
        return (int)(game->pause_time - game->start_time);
    }
    else
    {
        // Running normally - return current elapsed time
        return (int)(time(NULL) - game->start_time);
    }
}