/**
 * Puzzle Generator Module Header File
 * 
 * This header declares all functions related to Sudoku puzzle generation and validation.
 * The generator module is responsible for creating valid, solvable puzzles with unique
 * solutions at specified difficulty levels.
 * 
 * Key Responsibilities:
 * - Generate complete valid Sudoku grids
 * - Remove cells strategically to create puzzles
 * - Ensure puzzles have unique solutions
 * - Adjust difficulty by controlling clue count
 * - Provide utility functions for grid manipulation
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#include "sudoku.h"

// ============================================================================
//                          MAIN GENERATION FUNCTIONS
// ============================================================================
// Core functions for creating complete Sudoku puzzles

/**
 * Generate a complete Sudoku puzzle with specified difficulty
 * Creates a full solution, then strategically removes cells to form a puzzle
 * Ensures the resulting puzzle has exactly one unique solution
 * 
 * @param grid 9x9 array to store the puzzle (with some cells removed)
 * @param solution 9x9 array to store the complete solution
 * @param given 9x9 array to mark which cells are original clues (1) vs empty (0)
 * @param difficulty Desired puzzle difficulty level affecting number of clues
 * @return 1 if puzzle generation successful, 0 if failed
 */
int generate_puzzle(int grid[9][9], int solution[9][9], int given[9][9], difficulty_t difficulty);

// ============================================================================
//                          DIFFICULTY CONFIGURATION
// ============================================================================
// Functions that control puzzle difficulty through clue management

/**
 * Determine number of cells to remove based on difficulty level
 * Higher difficulty = more cells removed = fewer clues for player
 * 
 * @param difficulty Desired puzzle difficulty level
 * @return Number of cells that should be removed from complete grid
 */
int get_cells_to_remove(difficulty_t difficulty);

// ============================================================================
//                           VALIDATION FUNCTIONS  
// ============================================================================
// Functions for checking Sudoku rule compliance and grid validity

/**
 * Check if placing a number at specific position violates Sudoku rules
 * Validates against row, column, and 3x3 box constraints
 * 
 * @param grid 9x9 Sudoku grid to check against
 * @param row Target row position (0-8)
 * @param col Target column position (0-8)
 * @param num Number to validate (1-9)
 * @return 1 if placement is valid, 0 if it violates rules
 */
int is_valid_placement(int grid[9][9], int row, int col, int num);

// ============================================================================
//                            SOLVER FUNCTIONS
// ============================================================================
// Functions for solving and analyzing Sudoku grids

/**
 * Solve a Sudoku puzzle using backtracking algorithm
 * Modifies the input grid to contain a complete solution
 * Used during generation to create complete grids
 * 
 * @param grid 9x9 Sudoku grid to solve (modified in place)
 * @return 1 if solution found, 0 if puzzle is unsolvable
 */
int solve_grid(int grid[9][9]);

/**
 * Find the first empty cell (value 0) in the grid
 * Uses row-major order scanning (left to right, top to bottom)
 * Essential utility for backtracking algorithms
 * 
 * @param grid 9x9 Sudoku grid to search
 * @param row Pointer to store found row index
 * @param col Pointer to store found column index
 * @return 1 if empty cell found, 0 if grid is complete
 */
int find_empty_cell(int grid[9][9], int *row, int *col);

// ============================================================================
//                            UTILITY FUNCTIONS
// ============================================================================
// Helper functions for randomization and array manipulation

/**
 * Shuffle an array using Fisher-Yates algorithm
 * Ensures random, unbiased permutation of array elements
 * Used to randomize number placement during grid generation
 * 
 * @param array Pointer to integer array to shuffle
 * @param num Number of elements in the array
 */
void shuffle_array(int *array, int num);

#endif

/**
 * MODULE USAGE NOTES:
 * 
 * Generation Process:
 * 1. Create complete valid grid using solve_grid() with randomization
 * 2. Strategically remove cells based on difficulty level
 * 3. Validate that puzzle maintains unique solution
 * 4. Mark remaining cells as "given" clues
 * 
 * Difficulty Levels:
 * - EASY: ~36 clues (45 cells removed)
 * - MEDIUM: ~32 clues (49 cells removed) 
 * - HARD: ~30 clues (51 cells removed)
 * - EXPERT: ~28 clues (53 cells removed)
 * 
 * Algorithm Efficiency:
 * - Uses randomized backtracking for variety
 * - Fisher-Yates shuffle ensures unbiased randomization
 * - Strategic cell removal maintains puzzle quality
 * 
 * Integration with Game:
 * - generate_puzzle() is typically called from new_puzzle() in game module
 * - Generated puzzles are guaranteed to be solvable and unique
 * - Difficulty setting persists across multiple puzzle generations
 */