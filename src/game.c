#include "../include/sudoku.h"
#include "../include/game.h"
#include "../include/input.h"
#include "../include/generator.h"
#include "../include/solver.h"
#include "../include/display.h"  // Add this line
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
    game->difficulty = difficulty; // Store difficulty setting
    game->is_paused = 0;           // Game starts unpaused
    game->start_time = 0;          // Timer not started yet
    game->show_marks = 0;          // Start in number entry mode
    game->completion_time = 0;     // No completion time yet

    new_puzzle(game); // Generate initial puzzle
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
    game->completion_time = 0; // Clear any previous completion time

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
            game->moves++; // Increment move counter
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
    check_solution(game);      // Update completion status
    return game->is_completed; // Return the result
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
    game->is_paused = 0;       // Unpause game
    game->start_time = 0;      // Clear timer
    game->completion_time = 0; // Clear completion time
    game->show_marks = 0;      // Back to number mode
    game->cursor_row = 0;      // Reset cursor position
    game->cursor_col = 0;
    game->moves = 0;        // Clear move counter
    game->is_completed = 0; // Mark as incomplete
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
    game->start_time = time(NULL); // Record start time
    game->pause_time = 0;          // Clear any previous pause time
    game->is_paused = 0;           // Ensure timer is running
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
    if (!game->is_paused) // Only pause if not already paused
    {
        game->pause_time = time(NULL); // Record when pause started
        game->is_paused = 1;           // Mark as paused
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
    if (game->is_paused) // Only resume if currently paused
    {
        time_t now = time(NULL);
        // Shift start_time forward to account for pause duration
        // This keeps elapsed time calculation correct
        game->start_time += (now - game->pause_time);
        game->pause_time = 0; // Clear pause time
        game->is_paused = 0;  // Mark as running
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

/**
 * Hint System Implementation for Sudoku Game
 * Add these functions to your game.c file
 */

/**
 * Main hint function - finds the best hint using multiple strategies
 * Tries different hint techniques in order of difficulty
 *
 * @param game Pointer to game state
 * @param hint_row Pointer to store hint row position
 * @param hint_col Pointer to store hint column position
 * @param hint_value Pointer to store hint value
 * @return 1 if hint found, 0 if no hint available
 */
int get_hint(game_state_t *game, int *hint_row, int *hint_col, int *hint_value)
{
    // Strategy 1: Look for naked singles (cells with only one possible value)
    if (find_naked_single(game, hint_row, hint_col, hint_value))
    {
        return 1;
    }

    // Strategy 2: Look for hidden singles (only cell in row/col/box for a number)
    if (find_hidden_single(game, hint_row, hint_col, hint_value))
    {
        return 1;
    }

    // Strategy 3: If no logical hint found, give first empty cell from solution
    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            // Check if cell is empty AND not a given clue
            if (game->grid[row][col] == 0 && !game->given[row][col])
            {
                *hint_row = row;
                *hint_col = col;
                *hint_value = game->solution[row][col];
                return 1;
            }
        }
    }

    return 0; // No hint available (puzzle complete)
}

/**
 * Find naked singles - empty cells that can only contain one possible number
 * This is the most basic Sudoku solving technique
 *
 * @param game Pointer to game state
 * @param hint_row Pointer to store hint row position
 * @param hint_col Pointer to store hint column position
 * @param hint_value Pointer to store hint value
 * @return 1 if naked single found, 0 otherwise
 */
int find_naked_single(game_state_t *game, int *hint_row, int *hint_col, int *hint_value)
{
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            // Skip filled cells
            if (game->grid[row][col] != 0) continue;
            
            int count = 0;
            int last_possible = 0;
            
            // Check which numbers (1-9) are possible in this cell
            for (int num = 1; num <= GRID_SIZE; num++) {
                if (is_valid_placement(game->grid, row, col, num)) {
                    count++;
                    last_possible = num;
                }
            }
            
            // If only one number is possible, it's a naked single
            if (count == 1) {
                *hint_row = row;
                *hint_col = col;
                *hint_value = last_possible;
                return 1;
            }
        }
    }
    
    return 0; // No naked singles found
}

/**
 * Find hidden singles - numbers that can only go in one cell within a region
 * Checks rows, columns, and 3x3 boxes for numbers with only one valid position
 *
 * @param game Pointer to game state
 * @param hint_row Pointer to store hint row position
 * @param hint_col Pointer to store hint column position
 * @param hint_value Pointer to store hint value
 * @return 1 if hidden single found, 0 otherwise
 */
int find_hidden_single(game_state_t *game, int *hint_row, int *hint_col, int *hint_value)
{
    // Check each number (1-9)
    for (int num = 1; num <= GRID_SIZE; num++)
    {

        // Check rows for hidden singles
        for (int row = 0; row < GRID_SIZE; row++)
        {
            int possible_cols[GRID_SIZE];
            int possible_count = 0;

            // Find all possible positions for this number in this row
            for (int col = 0; col < GRID_SIZE; col++)
            {
                if (game->grid[row][col] == 0 &&
                    is_valid_placement(game->grid, row, col, num))
                {
                    possible_cols[possible_count] = col;
                    possible_count++;
                }
            }

            // If only one position possible, it's a hidden single
            if (possible_count == 1)
            {
                *hint_row = row;
                *hint_col = possible_cols[0];
                *hint_value = num;
                return 1;
            }
        }

        // Check columns for hidden singles
        for (int col = 0; col < GRID_SIZE; col++)
        {
            int possible_rows[GRID_SIZE];
            int possible_count = 0;

            // Find all possible positions for this number in this column
            for (int row = 0; row < GRID_SIZE; row++)
            {
                if (game->grid[row][col] == 0 &&
                    is_valid_placement(game->grid, row, col, num))
                {
                    possible_rows[possible_count] = row;
                    possible_count++;
                }
            }

            // If only one position possible, it's a hidden single
            if (possible_count == 1)
            {
                *hint_row = possible_rows[0];
                *hint_col = col;
                *hint_value = num;
                return 1;
            }
        }

        // Check 3x3 boxes for hidden singles
        for (int box_row = 0; box_row < 3; box_row++)
        {
            for (int box_col = 0; box_col < 3; box_col++)
            {
                int possible_positions[9][2]; // Store [row, col] pairs
                int possible_count = 0;

                int start_row = box_row * 3;
                int start_col = box_col * 3;

                // Find all possible positions for this number in this 3x3 box
                for (int r = start_row; r < start_row + 3; r++)
                {
                    for (int c = start_col; c < start_col + 3; c++)
                    {
                        if (game->grid[r][c] == 0 &&
                            is_valid_placement(game->grid, r, c, num))
                        {
                            possible_positions[possible_count][0] = r;
                            possible_positions[possible_count][1] = c;
                            possible_count++;
                        }
                    }
                }

                // If only one position possible, it's a hidden single
                if (possible_count == 1)
                {
                    *hint_row = possible_positions[0][0];
                    *hint_col = possible_positions[0][1];
                    *hint_value = num;
                    return 1;
                }
            }
        }
    }

    return 0; // No hidden singles found
}

/**
 * Display a hint message to the player
 * Shows where to place the hint and optionally explains the reasoning
 *
 * @param game Pointer to game state
 * @param row Hint row position
 * @param col Hint column position
 * @param value Hint value
 */
void show_hint_message(game_state_t *game, int row, int col)
{
    // Move cursor to hint position
    game->cursor_row = row;
    game->cursor_col = col;
}