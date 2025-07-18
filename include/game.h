/**
 * Game Module Header File
 * 
 * This header declares all functions related to core game mechanics and state management.
 * The game module handles high-level game operations like initialization, puzzle management,
 * solution checking, and timer functionality.
 * 
 * Key Responsibilities:
 * - Game initialization and setup
 * - Puzzle generation and management
 * - Solution validation and completion checking
 * - Player move processing
 * - Timer system for tracking elapsed time
 * - Game state persistence and reset functionality
 */

#ifndef GAME_H
#define GAME_H

#include "../include/sudoku.h"

// ============================================================================
//                          GAME LIFECYCLE FUNCTIONS
// ============================================================================
// Functions that manage the overall game state and lifecycle

/**
 * Initialize a new game with specified difficulty
 * Sets up all game state variables and generates initial puzzle
 * 
 * @param game Pointer to game state structure to initialize
 * @param difficulty Desired puzzle difficulty level (EASY, MEDIUM, HARD, EXPERT)
 */
void init_game(game_state_t *game, difficulty_t difficulty);

/**
 * Generate a completely new puzzle and reset game state
 * Creates fresh puzzle, solution, and given arrays while preserving difficulty setting
 * Resets cursor position, move counter, and completion status
 * 
 * @param game Pointer to game state structure to update
 */
void new_puzzle(game_state_t *game);

/**
 * Reset current game to original puzzle state
 * Restores initial clues and clears all player entries
 * Useful for starting over on the same puzzle
 * 
 * @param game Pointer to game state structure to reset
 */
void reset_game(game_state_t *game);

// ============================================================================
//                          SOLUTION MANAGEMENT
// ============================================================================
// Functions for checking and managing puzzle solutions

/**
 * Check if current player solution matches the stored complete solution
 * Compares every cell and updates game completion status
 * Should be called after each player move to detect completion
 * 
 * @param game Pointer to game state structure to validate
 */
void check_solution(game_state_t *game);

/**
 * Auto-solve the current puzzle by revealing the complete solution
 * Fills all empty cells with correct numbers from the solution array
 * Immediately marks the game as completed
 * 
 * @param game Pointer to game state structure to solve
 */
void solve_puzzle(game_state_t *game);

/**
 * Check if the current game is complete (all cells filled correctly)
 * Validates the solution and returns completion status
 * 
 * @param game Pointer to game state structure to check
 * @return 1 if game is complete and correct, 0 if incomplete or incorrect
 */
int is_game_complete(game_state_t *game);

// ============================================================================
//                           PLAYER INPUT FUNCTIONS
// ============================================================================
// Functions that handle player actions and moves

/**
 * Delete number and marks from the current cursor position
 * Only works on editable cells (not original clues)
 * Increments move counter when successful deletion occurs
 * 
 * @param game Pointer to game state structure to modify
 */
void delete_number(game_state_t *game);

/**
 * Toggle between number entry mode and pencil mark mode
 * Changes how number input is interpreted (place number vs toggle mark)
 * 
 * @param game Pointer to game state structure to modify
 */
void toggle_marks(game_state_t *game);

// ============================================================================
//                          VALIDATION FUNCTIONS
// ============================================================================
// Functions for validating moves and checking game state

/**
 * Check if placing a number at given position would be valid
 * Validates against Sudoku rules (no duplicates in row/column/box)
 * 
 * @param game Pointer to game state structure
 * @param row Target row position (0-8)
 * @param col Target column position (0-8)  
 * @param num Number to validate (1-9)
 * @return 1 if move is valid, 0 if it violates Sudoku rules
 */
int is_valid_move(game_state_t *game, int row, int col, int num);

/**
 * Check if a specific cell contains an original puzzle clue
 * Determines whether a cell can be edited by the player
 * 
 * @param game Pointer to game state structure
 * @param row Target row position (0-8)
 * @param col Target column position (0-8)
 * @return 1 if cell is an original clue (unchangeable), 0 if editable
 */
int is_cell_given(game_state_t *game, int row, int col);

// ============================================================================
//                            TIMER FUNCTIONS
// ============================================================================
// Functions for managing game timing and elapsed time tracking

/**
 * Start the game timer
 * Records current time as start time and ensures timer is unpaused
 * Should be called when beginning a new puzzle
 * 
 * @param game Pointer to game state structure to modify
 */
void start_timer(game_state_t *game);

/**
 * Pause the game timer
 * Records pause start time for accurate elapsed time calculation
 * Only pauses if timer is currently running
 * 
 * @param game Pointer to game state structure to modify
 */
void pause_timer(game_state_t *game);

/**
 * Resume the game timer after pause
 * Adjusts start time to account for pause duration
 * Maintains accurate elapsed time calculation
 * 
 * @param game Pointer to game state structure to modify
 */
void resume_timer(game_state_t *game);

/**
 * Calculate total elapsed game time in seconds
 * Handles different timer states (running, paused, completed)
 * Accounts for pause duration in calculations
 * 
 * @param game Pointer to game state structure
 * @return Elapsed time in seconds, or 0 if timer not started
 */
int get_elapsed_time(game_state_t *game);

// Add these to your game.h header file

// Hint system functions
int get_hint(game_state_t *game, int *hint_row, int *hint_col, int *hint_value);
int find_naked_single(game_state_t *game, int *hint_row, int *hint_col, int *hint_value);
int find_hidden_single(game_state_t *game, int *hint_row, int *hint_col, int *hint_value);
void show_hint_message(game_state_t *game, int row, int col);

#endif

/**
 * MODULE USAGE NOTES:
 * 
 * Typical Game Flow:
 * 1. init_game() - Set up new game
 * 2. start_timer() - Begin timing
 * 3. Player makes moves (validated with is_valid_move())
 * 4. check_solution() called after each move
 * 5. is_game_complete() detects completion
 * 
 * Timer Management:
 * - Timer automatically handles pause/resume logic
 * - get_elapsed_time() works correctly in all states
 * - Timer stops automatically when puzzle is completed
 * 
 * Move Validation:
 * - Always check is_cell_given() before allowing edits
 * - Use is_valid_move() for rule validation
 * - delete_number() safely handles given cell protection
 */