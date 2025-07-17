/**
 * Sudoku Solver Module Header File
 * 
 * This header declares all functions related to Sudoku puzzle solving and validation.
 * The solver module provides the core algorithms for solving puzzles, validating moves,
 * and analyzing puzzle properties like solution uniqueness and completeness.
 * 
 * Key Responsibilities:
 * - Solve Sudoku puzzles using backtracking algorithms
 * - Validate individual moves against Sudoku rules
 * - Analyze puzzle properties (uniqueness, completeness, validity)
 * - Support puzzle generation by ensuring quality constraints
 * - Provide utility functions for grid analysis
 */

#ifndef SOLVER_H
#define SOLVER_H

#include "../include/sudoku.h"

// ============================================================================
//                          CORE SOLVING FUNCTIONS
// ============================================================================
// Primary algorithms for solving Sudoku puzzles

/**
 * Solve a Sudoku puzzle using recursive backtracking algorithm
 * Modifies the input grid to contain a complete solution if solvable
 * Uses systematic approach: try numbers 1-9 in each empty cell
 * 
 * @param grid 9x9 Sudoku grid to solve (modified in place)
 * @return 1 if solution found and grid solved, 0 if puzzle is unsolvable
 */
int solve_grid(int grid[9][9]);

/**
 * Find the first empty cell (value 0) in the grid
 * Scans grid in row-major order (left to right, top to bottom)
 * Essential utility function for backtracking algorithms
 * 
 * @param grid 9x9 Sudoku grid to search
 * @param row Pointer to store the row index of found empty cell
 * @param col Pointer to store the column index of found empty cell
 * @return 1 if empty cell found, 0 if grid is completely filled
 */
int find_empty_cell(int grid[9][9], int *row, int *col);

// ============================================================================
//                          RULE VALIDATION FUNCTIONS
// ============================================================================
// Functions for checking Sudoku rule compliance

/**
 * Check if placing a number at specific position violates Sudoku rules
 * Validates against three core constraints:
 * - No duplicate in same row
 * - No duplicate in same column  
 * - No duplicate in same 3x3 box
 * 
 * @param grid 9x9 Sudoku grid to check against
 * @param row Target row position (0-8)
 * @param col Target column position (0-8)
 * @param num Number to validate (1-9)
 * @return 1 if placement follows all Sudoku rules, 0 if rules violated
 */
int is_valid_placement(int grid[9][9], int row, int col, int num);

/**
 * Validate if current state of Sudoku grid is legal
 * Checks all filled cells for rule violations without requiring completion
 * Useful for validating partially filled grids during gameplay
 * 
 * @param grid 9x9 Sudoku grid to validate
 * @return 1 if current state follows all rules, 0 if violations found
 */
int is_grid_valid(int grid[9][9]);

/**
 * Check if the Sudoku grid is completely filled
 * Only checks for completeness, not correctness of solution
 * Used to detect when all cells have been filled by player
 * 
 * @param grid 9x9 Sudoku grid to check
 * @return 1 if all cells contain numbers (1-9), 0 if empty cells remain
 */
int is_grid_complete(int grid[9][9]);

// ============================================================================
//                          SOLUTION ANALYSIS FUNCTIONS
// ============================================================================
// Advanced functions for analyzing puzzle solution properties

/**
 * Check if a Sudoku puzzle has exactly one unique solution
 * Critical for puzzle quality - good puzzles should have unique solutions
 * Uses modified solving algorithm to count all possible solutions
 * 
 * @param grid 9x9 Sudoku grid to analyze (not modified)
 * @return 1 if puzzle has exactly one solution, 0 if multiple or no solutions
 */
int has_unique_solution(int grid[9][9]);

/**
 * Count the total number of solutions for a Sudoku puzzle
 * More comprehensive than has_unique_solution() but potentially slower
 * Useful for puzzle analysis and debugging generation algorithms
 * 
 * @param grid 9x9 Sudoku grid to analyze (not modified)
 * @return Total number of valid solutions found
 */
int count_solutions(int grid[9][9]);

#endif

/**
 * MODULE USAGE NOTES:
 * 
 * Algorithm Overview:
 * - solve_grid() uses recursive backtracking with constraint checking
 * - find_empty_cell() provides systematic cell selection for backtracking
 * - is_valid_placement() enforces Sudoku rules at each step
 * 
 * Performance Characteristics:
 * - solve_grid(): O(9^(empty_cells)) worst case, much faster in practice
 * - has_unique_solution(): More expensive as it must check all possibilities
 * - count_solutions(): Most expensive, only use when necessary
 * 
 * Integration with Other Modules:
 * - Generator uses solve_grid() to create complete grids
 * - Generator uses has_unique_solution() to validate puzzle quality
 * - Game module uses is_valid_placement() for move validation
 * - Display module can use these for highlighting invalid moves
 * 
 * Rule Validation:
 * - Row constraint: no duplicates in same horizontal line
 * - Column constraint: no duplicates in same vertical line
 * - Box constraint: no duplicates in same 3x3 subgrid
 * - All three constraints must be satisfied simultaneously
 * 
 * Grid State Analysis:
 * - is_grid_valid(): checks rules compliance (allows empty cells)
 * - is_grid_complete(): checks if all cells filled (ignores correctness)
 * - Combine both to verify a complete, correct solution
 */