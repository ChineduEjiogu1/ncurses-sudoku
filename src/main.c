/**
 * Sudoku Game Main Entry Point
 * 
 * This file contains the main game loop and handles the overall program flow.
 * It initializes ncurses, sets up the game state, handles user input, and
 * manages the display updates including smooth timer functionality.
 * 
 * Key Features:
 * - Non-blocking input with timeout for smooth timer updates
 * - Efficient redrawing (grid-only for cursor movement, full for game changes)
 * - Real-time timer display without screen flickering
 * - Complete input handling for all game commands
 */

#include "../include/sudoku.h"
#include "../include/display.h"
#include "../include/input.h"
#include "../include/game.h"
#include "../include/generator.h"
#include <ncurses/curses.h>

/**
 * Main program entry point
 * Initializes the game environment, runs the main game loop, and handles cleanup
 * 
 * @return 0 on successful program completion
 */
int main(void)
{
    game_state_t game;  // Game state structure to hold all game data

    // ========================================================================
    //                           NCURSES INITIALIZATION
    // ========================================================================
    
    initscr();              // Initialize ncurses screen
    raw();                  // Disable line buffering - get keys immediately
    noecho();               // Don't echo typed characters to screen
    keypad(stdscr, TRUE);   // Enable arrow keys and function keys
    curs_set(0);            // Hide the terminal cursor
    timeout(250);           // Set 250ms timeout for getch() - enables smooth timer updates

    // ========================================================================
    //                              GAME SETUP
    // ========================================================================
    
    init_colors();                  // Set up color pairs for the interface
    init_game(&game, MEDIUM);       // Initialize game state with medium difficulty
    start_timer(&game);             // Begin timing the game session
    
    draw_game(&game);               // Draw the initial game screen

    // ========================================================================
    //                          MAIN GAME LOOP SETUP
    // ========================================================================
    
    int last_time = -1;             // Track last displayed time to avoid redundant updates
    int continue_game = 1;          // Flag to control main game loop
    
    // ========================================================================
    //                             MAIN GAME LOOP
    // ========================================================================
    
    while (continue_game)
    {
        int ch = getch();           // Get character input (blocks for up to 250ms)
        
        if (ch == ERR) {
            // ================================================================
            //                          TIMEOUT HANDLING
            // ================================================================
            // No key was pressed within timeout period - update timer display
            
            int current_time = get_elapsed_time(&game);
            if (current_time != last_time) {
                // Timer has incremented - update only the timer display
                attron(COLOR_PAIR(9));                          // Activate cyan color
                mvprintw(6, 50, "Time: %02d:%02d",              // Update timer at fixed position
                         current_time / 60, current_time % 60); // Format as MM:SS
                attroff(COLOR_PAIR(9));                         // Deactivate color
                refresh();                                      // Apply changes to screen
                last_time = current_time;                       // Remember this time
            }
        } else {
            // ================================================================
            //                           INPUT HANDLING
            // ================================================================
            // A key was pressed - process the input
            
            switch (ch)
            {
                // ============================================================
                //                    CURSOR MOVEMENT CONTROLS
                // ============================================================
                // Arrow keys move cursor and redraw only the grid (efficient)
                
                case KEY_UP:
                    move_cursor_up(&game);      // Move cursor up one row
                    draw_grid(&game);           // Redraw grid to show new cursor position
                    break;
                case KEY_DOWN:
                    move_cursor_down(&game);    // Move cursor down one row
                    draw_grid(&game);           // Redraw grid to show new cursor position
                    break;
                case KEY_LEFT:
                    move_cursor_left(&game);    // Move cursor left one column
                    draw_grid(&game);           // Redraw grid to show new cursor position
                    break;
                case KEY_RIGHT:
                    move_cursor_right(&game);   // Move cursor right one column
                    draw_grid(&game);           // Redraw grid to show new cursor position
                    break;

                // ============================================================
                //                      NUMBER INPUT (1-9)
                // ============================================================
                // Number keys place digits or toggle pencil marks
                
                case '1': case '2': case '3': case '4': case '5':
                case '6': case '7': case '8': case '9':
                    enter_number(&game, ch - '0');  // Convert char to int and place number
                    draw_game(&game);               // Full redraw (numbers can change highlighting)
                    break;

                // ============================================================
                //                       GAME COMMANDS
                // ============================================================
                // Letter keys for various game functions
                
                case 'c': case 'n': case 'm': case 'r': case 's': case 'x':
                    // Process the specific command
                    if (ch == 'c') 
                        check_solution(&game);              // Check if puzzle is solved correctly
                    else if (ch == 'n') { 
                        new_puzzle(&game);                  // Generate new puzzle
                        start_timer(&game);                 // Restart timer for new puzzle
                    }
                    else if (ch == 'm') 
                        toggle_marks(&game);                // Switch between number/mark mode
                    else if (ch == 's') 
                        solve_puzzle(&game);                // Auto-solve the current puzzle
                    else if (ch == 'x') 
                        delete_number(&game);               // Clear current cell
                    // Note: 'r' (redraw) is handled by the draw_game() call below
                    
                    draw_game(&game);                       // Full redraw for all commands
                    break;

                // ============================================================
                //                         QUIT COMMANDS
                // ============================================================
                // Exit the game
                
                case 'q':                       // 'q' key to quit
                case 27:                        // ESC key to quit
                    continue_game = 0;          // Set flag to exit main loop
                    break;
            }
            
            // Note: We intentionally DON'T update last_time here
            // This allows the timeout mechanism to handle timer updates smoothly
        }
        
        // ====================================================================
        //                          COMPLETION CHECK
        // ====================================================================
        // Check if puzzle was just completed and show completion message
        
        if (is_game_complete(&game) && game.completion_time == 0) {
            game.completion_time = time(NULL);      // Record completion time
            draw_completion_message(&game);         // Display congratulations message
        }
    }

    // ========================================================================
    //                               CLEANUP
    // ========================================================================
    
    endwin();               // Restore terminal to normal mode
    return 0;               // Successful program termination
}