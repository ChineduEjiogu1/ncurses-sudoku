/**
 * Input Handling Module Header File
 * 
 * This header declares all functions related to user input processing and cursor management.
 * The input module serves as the interface between user actions (keyboard input) and
 * game state modifications, handling both movement and game commands.
 * 
 * Key Responsibilities:
 * - Process keyboard input and route to appropriate actions
 * - Manage cursor movement within grid boundaries
 * - Handle number placement and pencil mark toggling
 * - Validate input before applying changes to game state
 * - Provide unified interface for all user interactions
 */

#ifndef INPUT_H
#define INPUT_H

#include "../include/sudoku.h"

// ============================================================================
//                          MAIN INPUT PROCESSING
// ============================================================================
// High-level input handling and command routing

/**
 * Main input handler for the Sudoku game
 * Processes keyboard input and routes commands to appropriate functions
 * Handles arrow keys, number input, and game command shortcuts
 * 
 * @param game Pointer to game state structure to modify
 * @return 1 to continue game loop, 0 to quit game
 */
int handle_input(game_state_t *game);

// ============================================================================
//                          CURSOR MOVEMENT FUNCTIONS
// ============================================================================
// Functions for navigating the cursor around the Sudoku grid

/**
 * Move cursor up one row
 * Prevents moving above the top boundary (row 0)
 * 
 * @param game Pointer to game state structure containing cursor position
 */
void move_cursor_up(game_state_t *game);

/**
 * Move cursor down one row
 * Prevents moving below the bottom boundary (row 8)
 * 
 * @param game Pointer to game state structure containing cursor position
 */
void move_cursor_down(game_state_t *game);

/**
 * Move cursor left one column
 * Prevents moving past the left boundary (column 0)
 * 
 * @param game Pointer to game state structure containing cursor position
 */
void move_cursor_left(game_state_t *game);

/**
 * Move cursor right one column
 * Prevents moving past the right boundary (column 8)
 * 
 * @param game Pointer to game state structure containing cursor position
 */
void move_cursor_right(game_state_t *game);

// ============================================================================
//                          NUMBER INPUT PROCESSING
// ============================================================================
// Functions for handling number placement and pencil marks

/**
 * Process number placement in the grid
 * Validates cell editability and Sudoku rules before placement
 * Updates move counter and provides feedback for invalid moves
 * 
 * @param game Pointer to game state structure to modify
 * @param num Number to place (1-9)
 * @return 1 if number placed successfully, 0 if placement invalid
 */
int process_number_input(game_state_t *game, int num);

/**
 * Toggle pencil marks for a number at current cursor position
 * Pencil marks are candidate numbers that might go in a cell
 * Toggles mark on/off: if marked becomes unmarked, if unmarked becomes marked
 * 
 * @param game Pointer to game state structure to modify
 * @param num Number to toggle mark for (1-9)
 */
void handle_mark_input(game_state_t *game, int num);

/**
 * Unified number entry function
 * Routes to either number placement or mark toggling based on current mode
 * Automatically determines action based on game->show_marks flag
 * 
 * @param game Pointer to game state structure to modify
 * @param num Number to process (1-9)
 */
void enter_number(game_state_t *game, int num);

// ============================================================================
//                          INPUT VALIDATION FUNCTIONS
// ============================================================================
// Functions for validating input before applying changes

/**
 * Validate if cursor position is within grid boundaries
 * Ensures cursor coordinates are within valid range (0-8, 0-8)
 * 
 * @param row Row position to validate
 * @param col Column position to validate
 * @return 1 if position is valid, 0 if out of bounds
 */
int is_valid_cursor_position(int row, int col);

/**
 * Check if a cell can accept user input
 * Original puzzle clues (given cells) cannot be modified by player
 * 
 * @param game Pointer to game state structure
 * @param row Target row position (0-8)
 * @param col Target column position (0-8)
 * @return 1 if cell is editable, 0 if it's a given clue
 */
int can_enter_number(game_state_t *game, int row, int col);

#endif

/**
 * MODULE USAGE NOTES:
 * 
 * Input Flow:
 * 1. handle_input() receives raw keyboard input
 * 2. Routes to appropriate movement or action function
 * 3. Validation functions ensure safe operations
 * 4. Game state is updated if input is valid
 * 
 * Cursor Management:
 * - Cursor position is bounded within 0-8 range for both row and col
 * - Movement functions prevent cursor from going out of bounds
 * - Cursor position determines where numbers/marks are placed
 * 
 * Number Input Modes:
 * - Normal mode: enter_number() places digits directly
 * - Mark mode: enter_number() toggles pencil marks
 * - Mode determined by game->show_marks flag
 * 
 * Input Validation:
 * - can_enter_number() prevents editing given clues
 * - is_valid_cursor_position() ensures safe array access
 * - All input is validated before modifying game state
 * 
 * Key Mappings (handled in main loop):
 * - Arrow keys: cursor movement
 * - 1-9: number input (mode-dependent)
 * - x: delete number
 * - m: toggle mark mode
 * - q/ESC: quit game
 * - n: new puzzle
 * - s: solve puzzle
 */