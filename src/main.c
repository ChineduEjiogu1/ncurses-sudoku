#include "../include/sudoku.h"
#include "../include/display.h"
#include "../include/input.h"
#include "../include/game.h"
#include <ncurses/ncurses.h>

// int main()
// {
//     game_state_t game = {0};

//     // Initialize ncurses
//     initscr();
//     raw();
//     noecho();
//     keypad(stdscr, TRUE);
//     curs_set(0); // Optional: hide blinking cursor

//     // Initialize the game state with a new puzzle
//     new_puzzle(&game);

//     // Draw initial screen
//     redraw_screen(&game);

//     // Main game loop
//     while (handle_input(&game))
//     {
//         redraw_screen(&game); // Refresh visual after every input
//     }

//     // Cleanup
//     endwin();
//     return 0;
// }

// int main() {
//     // Initialize ncurses
//     initscr();
//     raw();
//     noecho();
//     keypad(stdscr, TRUE);
    
//     // Create minimal game state for testing
//     game_state_t test_game = {0};
//     test_game.cursor_row = 4;  // Start in middle
//     test_game.cursor_col = 4;
//     test_game.show_marks = 0;  // Start in number mode
    
//     // Set up some test given cells
//     test_game.given[0][0] = 1;  // Make corner cell a given
//     test_game.grid[0][0] = 5;
    
//     printw("Input Test - ESC to quit\n");
//     printw("Cursor: (%d,%d) Mode: %s\n", 
//            test_game.cursor_row, test_game.cursor_col,
//            test_game.show_marks ? "MARK" : "NUMBER");
    
//     while(handle_input(&test_game)) {
//         clear();
//         printw("Cursor: (%d,%d) Mode: %s\n", 
//                test_game.cursor_row, test_game.cursor_col,
//                test_game.show_marks ? "MARK" : "NUMBER");
//         refresh();
//     }
    
//     endwin();
//     return 0;
// }

// int main(int argc, char *argv[])
// {
//     game_state_t game;

//     // Initialize ncurses
//     init_ncurses();

//     // Initialize game with default difficulty
//     init_game(&game, EASY);

//     // Main game loop
//     game_loop(&game);

//     // Cleanup and exit
//     cleanup_ncurses();
//     return 0;
// }

// void init_ncurses(void)
// {
//     initscr();            // Initialize ncurses
//     cbreak();             // Disable line buffering
//     noecho();             // Don't echo pressed keys
//     keypad(stdscr, TRUE); // Enable arrow keys
//     curs_set(0);          // Hide cursor

//     // Initialize colors if terminal supports them
//     if (has_colors())
//     {
//         start_color();
//         init_colors(); // Call your color initialization function
//     }
// }

// void cleanup_ncurses(void)
// {
//     endwin(); // End ncurses mode
// }

// void game_loop(game_state_t *game)
// {
//     while (1)
//     {
//         // Clear screen and draw everything
//         clear();
//         draw_game(game);
//         refresh();

//         // Handle input (returns 0 when user wants to quit)
//         if (!handle_input(game))
//         {
//             break;
//         }
//     }
// }