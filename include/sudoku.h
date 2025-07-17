/**
 * Sudoku Game Header File
 * 
 * This header file contains all the core data structures, constants, and type definitions
 * used throughout the Sudoku game. It serves as the central include for all modules
 * and defines the fundamental game state structure.
 * 
 * Key Components:
 * - Game constants (grid size, box size)
 * - Difficulty enumeration
 * - Complete game state structure
 * - Standard library includes for all modules
 */

#ifndef SUDOKU_H
#define SUDOKU_H

// ============================================================================
//                             STANDARD INCLUDES
// ============================================================================
// Essential libraries used across multiple game modules

#include <ncurses/ncurses.h>    // Terminal UI library for display and input
#include <stdlib.h>             // Standard library (malloc, rand, etc.)
#include <string.h>             // String manipulation functions
#include <time.h>               // Time functions for game timer
#include <stdbool.h>            // Boolean type support
#include <stdint.h>             // Fixed-width integer types
#include <stdio.h>              // Standard I/O operations
#include <limits.h>             // System limits and constants

// ============================================================================
//                              GAME CONSTANTS
// ============================================================================
// Core mathematical constants that define Sudoku game structure

#define GRID_SIZE 9             // Standard Sudoku grid is 9x9
#define BOX_SIZE 3              // Each 3x3 box within the grid

// ============================================================================
//                            DIFFICULTY ENUMERATION
// ============================================================================
// Defines the available difficulty levels for puzzle generation

typedef enum
{
    EASY = 0,                   // Easiest difficulty - more clues provided
    MEDIUM,                     // Moderate difficulty - balanced clue count
    HARD,                       // Challenging difficulty - fewer clues
    EXPERT                      // Hardest difficulty - minimal clues
} difficulty_t;

// ============================================================================
//                           MAIN GAME STATE STRUCTURE
// ============================================================================
// Central data structure containing all game information and state

typedef struct
{
    // ========================================================================
    //                              GRID DATA
    // ========================================================================
    
    int grid[GRID_SIZE][GRID_SIZE];              // Current puzzle state (player's progress)
    int solution[GRID_SIZE][GRID_SIZE];          // Complete solution to the puzzle
    int given[GRID_SIZE][GRID_SIZE];             // Marks which cells are original clues (1) vs player-filled (0)
    int marks[GRID_SIZE][GRID_SIZE][GRID_SIZE];  // Pencil marks: [row][col][number-1] = 1 if marked
    
    // ========================================================================
    //                            DISPLAY STATE
    // ========================================================================
    
    int show_marks;                              // Flag: 1 = mark mode, 0 = number entry mode
    int cursor_row, cursor_col;                  // Current cursor position (0-8, 0-8)
    
    // ========================================================================
    //                             GAME SETTINGS
    // ========================================================================
    
    difficulty_t difficulty;                     // Current puzzle difficulty level
    int moves;                                   // Count of player moves (for statistics)
    
    // ========================================================================
    //                              TIMER DATA
    // ========================================================================
    
    time_t start_time;                           // When the current puzzle was started
    time_t pause_time;                           // When pause was initiated (if paused)
    time_t completion_time;                      // When puzzle was completed (0 if not finished)
    
    // ========================================================================
    //                             STATUS FLAGS
    // ========================================================================
    
    int is_paused;                               // Flag: 1 = game paused, 0 = running
    int is_completed;                            // Flag: 1 = puzzle solved, 0 = in progress
    
} game_state_t;

#endif

/**
 * USAGE NOTES:
 * 
 * Grid Indexing:
 * - All grids use 0-8 indexing (not 1-9)
 * - grid[row][col] where row=0 is top, col=0 is left
 * 
 * Marks Array:
 * - marks[row][col][number-1] corresponds to pencil mark for number (1-9)
 * - Example: marks[0][0][4] = 1 means "5" is marked in top-left cell
 * 
 * Given Array:
 * - given[row][col] = 1 means cell is original clue (cannot be edited)
 * - given[row][col] = 0 means cell is empty/editable
 * 
 * Timer System:
 * - start_time: absolute time when puzzle began
 * - pause_time: absolute time when pause started (only valid if is_paused = 1)
 * - completion_time: absolute time when puzzle was solved
 * - Elapsed time = current_time - start_time (adjusted for pause duration)
 */