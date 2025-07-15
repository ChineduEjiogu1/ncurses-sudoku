// #include "../include/sudoku.h"
// #include "../include/generator.h"
// #include "../include/solver.h"
// #include <stdio.h>

// #define SIZE 9
// #define GRID_SIZE 9  // Needed for puzzle, solution, given arrays

// void print_grid(int grid[SIZE][SIZE])
// {
//     printf("-------------------------\n");
//     for (int row = 0; row < SIZE; row++)
//     {
//         for (int col = 0; col < SIZE; col++)
//         {
//             if (col % 3 == 0)
//                 printf("| ");
//             if (grid[row][col] == 0)
//                 printf(". ");
//             else
//                 printf("%d ", grid[row][col]);
//         }
//         printf("|\n");
//         if ((row + 1) % 3 == 0)
//         {
//             printf("-------------------------\n");
//         }
//     }
// }

// int main()
// {
//     printf("=== Testing Sudoku Functions ===\n");

//     // Test 1: Sample partially filled grid
//     int test_grid[9][9] = {
//         {5, 3, 0, 0, 7, 0, 0, 0, 0},
//         {6, 0, 0, 1, 9, 5, 0, 0, 0},
//         {0, 9, 8, 0, 0, 0, 0, 6, 0},
//         {8, 0, 0, 0, 6, 0, 0, 0, 3},
//         {4, 0, 0, 8, 0, 3, 0, 0, 1},
//         {7, 0, 0, 0, 2, 0, 0, 0, 6},
//         {0, 6, 0, 0, 0, 0, 2, 8, 0},
//         {0, 0, 0, 4, 1, 9, 0, 0, 5},
//         {0, 0, 0, 0, 8, 0, 0, 7, 9}
//     };

//     printf("\nOriginal Grid:\n");
//     print_grid(test_grid);

//     // Test 2: Try is_valid_placement
//     int test_row = 0;
//     int test_col = 2;
//     int test_num = 2;

//     printf("\nTest is_valid_placement(%d at [%d][%d]):\n", test_num, test_row, test_col);
//     if (is_valid_placement(test_grid, test_row, test_col, test_num))
//         printf("✅ It is valid to place %d at (%d, %d)\n", test_num, test_row, test_col);
//     else
//         printf("❌ It is NOT valid to place %d at (%d, %d)\n", test_num, test_row, test_col);

//     // Test 3: Try solve_grid
//     printf("\nSolving the puzzle...\n");
//     if (solve_grid(test_grid))
//     {
//         printf("✅ Solved Puzzle:\n");
//         print_grid(test_grid);
//     }
//     else
//     {
//         printf("❌ No solution found.\n");
//     }

//     // Test 4: Try generate_puzzle
//     int puzzle[GRID_SIZE][GRID_SIZE];
//     int solution[GRID_SIZE][GRID_SIZE];
//     int given[GRID_SIZE][GRID_SIZE];

//     printf("\nGenerating new puzzle (Medium difficulty)...\n");
//     if (generate_puzzle(puzzle, solution, given, MEDIUM))
//     {
//         printf("✅ Generated Puzzle:\n");
//         print_grid(puzzle);

//         printf("\nPuzzle Solution:\n");
//         print_grid(solution);
//     }
//     else
//     {
//         printf("❌ Failed to generate puzzle.\n");
//     }

//     return 0;
// }
