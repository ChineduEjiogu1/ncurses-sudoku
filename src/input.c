#include "../include/sudoku.h"
#include "../include/display.h"
#include "../include/input.h"
#include "../include/game.h"
#include "../include/solver.h"
#include <ncurses/ncurses.h>


/**
 * Move cursor up one row
 * Prevents moving above top boundary
 */
void move_cursor_up(game_state_t *game)
{
    if (game->cursor_row > 0)
    {
        game->cursor_row--;
    }
}

/**
 * Move cursor down one row
 * Prevents moving below bottom boundary
 */
void move_cursor_down(game_state_t *game)
{
    if (game->cursor_row < GRID_SIZE - 1)
    {
        game->cursor_row++;
    }
}

/**
 * Move cursor left one column
 * Prevents moving past left boundary
 */
void move_cursor_left(game_state_t *game)
{
    if (game->cursor_col > 0)
    {
        game->cursor_col--;
    }
}

/**
 * Move cursor right one column
 * Prevents moving past right boundary
 */
void move_cursor_right(game_state_t *game)
{
    if (game->cursor_col < GRID_SIZE - 1)
    {
        game->cursor_col++;
    }
}

/**
 * Main input handler for the Sudoku game
 * Processes keyboard input and routes to appropriate functions
 * Returns: 1 to continue game loop, 0 to quit
 */
int handle_input(game_state_t *game)
{
    int ch = getch(); // Get single character input without Enter

    switch (ch)
    {
        // Arrow key movement controls
        case KEY_UP:
            move_cursor_up(game);
            break;
        case KEY_DOWN:
            move_cursor_down(game);
            break;
        case KEY_LEFT:
            move_cursor_left(game);
            break;
        case KEY_RIGHT:
            move_cursor_right(game);
            break;

        // Number input (1-9) - enters numbers or toggles marks
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            enter_number(game, ch - '0'); // Convert char to int
            break;

        // Game command shortcuts
        case 'c':
            check_solution(game);         // Check if current solution is correct
            break;
        case 'n':
            new_puzzle(game);            // Generate new puzzle
            break;
        case 'm':
            toggle_marks(game);          // Switch between number/mark mode
            break;
        case 'r':
            redraw_screen(game);         // Refresh display
            break;
        case 's':
            solve_puzzle(game);          // Auto-solve puzzle
            break;
        case 'x':
            delete_number(game);         // Clear current cell
            break;

        // Exit game
        case 'q':
        case 27: // ESC key
            return 0; // Signal to quit game loop

        // Ignore unrecognized input
        default:
            break;
    }

    return 1; // Continue game loop
}

/**
 * Process actual number placement in grid
 * Validates both editability and Sudoku rules
 * Returns: 1 if number placed successfully, 0 if invalid
 */
int process_number_input(game_state_t *game, int num)
{
    // Check if cell can be edited AND number follows Sudoku rules
    if (can_enter_number(game, game->cursor_row, game->cursor_col) && 
        is_valid_placement(game->grid, game->cursor_row, game->cursor_col, num))
    {
        game->grid[game->cursor_row][game->cursor_col] = num; // Place number
        return 1; // Success
    }
    else
    {
        return 0; // Invalid move
    }
}

/**
 * Toggle pencil marks for a number at current cursor position
 * Pencil marks are candidate numbers that might go in a cell
 */
void handle_mark_input(game_state_t *game, int num)
{
    // Only allow marks in editable cells
    if (can_enter_number(game, game->cursor_row, game->cursor_col))
    {
        // Toggle mark: if 0 becomes 1, if 1 becomes 0
        // num-1 converts 1-9 to 0-8 array index
        game->marks[game->cursor_row][game->cursor_col][num - 1] = 
            !game->marks[game->cursor_row][game->cursor_col][num - 1];
    }
}

/**
 * Validate if cursor position is within grid boundaries
 * Returns: 1 if valid position, 0 if out of bounds
 */
int is_valid_cursor_position(int row, int col)
{
    if (row >= 0 && row <= 8 && col >= 0 && col <= 8)
    {
        return 1; // Valid position
    }
    else
    {
        return 0; // Out of bounds
    }
}

/**
 * Check if a cell can accept user input
 * Given/clue cells cannot be modified
 * Returns: 1 if editable, 0 if given/clue cell
 */
int can_enter_number(game_state_t *game, int row, int col)
{
    return game->given[row][col] == 0; // 0 = editable, 1 = given/clue
}

/**
 * Unified number entry function
 * Routes to either number placement or mark toggling based on current mode
 */
void enter_number(game_state_t *game, int num)
{
    if (game->show_marks)
    {
        handle_mark_input(game, num);    // Mark mode: toggle pencil mark
    }
    else
    {
        process_number_input(game, num); // Number mode: place number
    }
}