/**
 * Sudoku Display Module
 * 
 * This module handles all visual display aspects of the Sudoku game using ncurses.
 * It manages colors, grid rendering, UI panels, and visual feedback for the player.
 * 
 * Key Features:
 * - Colored grid with distinct 3x3 box and cell borders
 * - Red highlighting for invalid moves and conflicting areas
 * - Real-time timer and move counter display
 * - Help panel with controls
 * - Modular design for easy maintenance
 */

#include "../include/sudoku.h"
#include "../include/game.h"
#include "../include/input.h"
#include "../include/generator.h"
#include "../include/solver.h"
#include "../include/display.h"
#include <ncurses/curses.h>

// Color pair constants for consistent color management
#define COLOR_NORMAL 1      // White text for user-entered numbers
#define COLOR_GIVEN 2       // White text for puzzle clues
#define COLOR_CURSOR 3      // Black text on cyan background for cursor
#define COLOR_INVALID 4     // White text on red background for conflicts
#define COLOR_COMPLETE 5    // Green text for completion messages

// Grid positioning constants for consistent layout
#define GRID_START_Y 4      // Starting Y position of the grid
#define GRID_START_X 2      // Starting X position of the grid
#define CELL_WIDTH 3        // Width of each cell (characters)
#define CELL_HEIGHT 1       // Height of each cell (characters)

/**
 * Initialize color pairs for the game display
 * Sets up all color combinations used throughout the interface
 */
void init_colors(void)
{
    if (has_colors())
    {
        start_color();                                            // Enable color support
        use_default_colors();                                     // Use terminal's default colors
        
        // Define color pairs for different UI elements
        init_pair(COLOR_NORMAL, COLOR_WHITE, COLOR_BLACK);        // Normal numbers - white text
        init_pair(COLOR_GIVEN, COLOR_WHITE, COLOR_BLACK);         // Given numbers - white text
        init_pair(COLOR_CURSOR, COLOR_BLACK, COLOR_CYAN);         // Cursor - black on light blue
        init_pair(COLOR_INVALID, COLOR_WHITE, COLOR_RED);         // Invalid moves - white on red
        init_pair(COLOR_COMPLETE, COLOR_GREEN, COLOR_BLACK);      // Complete state - green text

        // Additional color pairs for borders and UI
        init_pair(6, COLOR_WHITE, COLOR_BLACK);                   // Cell borders - white
        init_pair(7, COLOR_BLUE, COLOR_BLACK);                    // 3x3 box borders - blue
        init_pair(8, COLOR_GREEN, COLOR_BLACK);                   // UI text - green
        init_pair(9, COLOR_CYAN, COLOR_BLACK);                    // Header text - cyan
    }
}

/**
 * Main game drawing function
 * Coordinates the drawing of all game elements in the correct order
 * 
 * @param game Pointer to the current game state
 */
void draw_game(game_state_t *game)
{
    clear();                // Clear the screen
    draw_title_info(game);  // Draw title and game information
    draw_help_panel();      // Draw controls and help text
    draw_grid(game);        // Draw the Sudoku grid and numbers
    refresh();              // Update the display
}

/**
 * Draw the title, subtitle, and game information panel
 * Displays game stats like timer, move count, and difficulty level
 * 
 * @param game Pointer to the current game state
 */
void draw_title_info(game_state_t *game)
{
    // Draw main title in cyan
    attron(COLOR_PAIR(9));
    mvprintw(0, 0, "Nudoku");
    attroff(COLOR_PAIR(9));

    // Draw subtitle in white
    attron(COLOR_PAIR(COLOR_NORMAL));
    mvprintw(2, 0, "Sudoku for your terminal.");
    attroff(COLOR_PAIR(COLOR_NORMAL));

    // Draw game information panel on the right side
    attron(COLOR_PAIR(9));
    mvprintw(4, 50, "nudoku 1.0.0");

    // Display current difficulty level
    const char *difficulty_names[] = {"easy", "medium", "hard", "expert"};
    mvprintw(5, 50, "Level: %s", difficulty_names[game->difficulty]);

    // Display elapsed time if game has started
    if (game->start_time > 0)
    {
        int elapsed = get_elapsed_time(game);
        int minutes = elapsed / 60;
        int seconds = elapsed % 60;
        mvprintw(6, 50, "Time: %02d:%02d", minutes, seconds);
    }

    // Display move counter
    mvprintw(7, 50, "Moves: %d", game->moves);

    attroff(COLOR_PAIR(9));
}

/**
 * Draw the help panel with game controls
 * Shows movement keys and available commands to the player
 */
void draw_help_panel(void)
{
    attron(COLOR_PAIR(9)); // Light blue (cyan) for help panel

    // Movement instructions
    mvprintw(9, 50, "Movement");
    mvprintw(10, 52, "Arrow keys - Move cursor");

    // Game commands
    mvprintw(12, 50, "Commands");
    mvprintw(13, 52, "1-9 - Enter number");
    mvprintw(14, 52, "x - Delete number");
    mvprintw(15, 52, "m - Toggle marks");
    mvprintw(16, 52, "n - New puzzle");
    mvprintw(17, 52, "s - Solve puzzle");
    mvprintw(18, 52, "r - Redraw");
    mvprintw(19, 52, "q - Quit");

    attroff(COLOR_PAIR(9));
}

/**
 * Draw the complete Sudoku grid with borders and numbers
 * Handles both the visual grid structure and number placement
 * 
 * @param game Pointer to the current game state
 */
void draw_grid(game_state_t *game)
{
    // Draw all horizontal lines (top to bottom)
    for (int row = 0; row <= GRID_SIZE; row++)
    {
        int y = GRID_START_Y + row * (CELL_HEIGHT + 1);
        int is_thick = (row % 3 == 0);  // Every 3rd line is thick (3x3 box boundary)

        move(y, GRID_START_X);

        // Draw horizontal line segments and intersections
        for (int col = 0; col < GRID_SIZE; col++)
        {
            // Choose intersection character and color based on position
            if (col == 0)
            {
                // Left edge of the grid
                if (is_thick)
                    attrset(COLOR_PAIR(7)); // Blue for 3x3 boundaries
                else
                    attrset(COLOR_PAIR(6)); // White for cell boundaries

                // Choose appropriate corner/junction character
                if (row == 0)
                    addch(ACS_ULCORNER);        // Top-left corner
                else if (row == GRID_SIZE)
                    addch(ACS_LLCORNER);        // Bottom-left corner
                else
                    addch(ACS_LTEE);            // Left T-junction
            }
            else
            {
                // Internal intersections
                bool thick_h = is_thick;        // Horizontal line is thick
                bool thick_v = (col % 3 == 0);  // Vertical line is thick

                // Use blue if either line is thick (3x3 boundary)
                if (thick_h || thick_v)
                    attrset(COLOR_PAIR(7)); // Blue for any 3x3 boundary
                else
                    attrset(COLOR_PAIR(6)); // White for cell boundaries

                // Choose appropriate junction character
                if (row == 0)
                    addch(ACS_TTEE);            // Top T-junction
                else if (row == GRID_SIZE)
                    addch(ACS_BTEE);            // Bottom T-junction
                else
                    addch(ACS_PLUS);            // Cross intersection
            }

            // Draw horizontal line segment with appropriate color
            if (is_thick)
                attrset(COLOR_PAIR(7)); // Blue for 3x3 horizontal boundaries
            else
                attrset(COLOR_PAIR(6)); // White for cell boundaries

            // Draw the horizontal line characters
            for (int i = 0; i < CELL_WIDTH; i++)
                addch(ACS_HLINE);
        }

        // Draw right edge of the grid
        if (is_thick)
            attrset(COLOR_PAIR(7)); // Blue for 3x3 boundaries
        else
            attrset(COLOR_PAIR(6)); // White for cell boundaries

        // Choose appropriate corner/junction character for right edge
        if (row == 0)
            addch(ACS_URCORNER);        // Top-right corner
        else if (row == GRID_SIZE)
            addch(ACS_LRCORNER);        // Bottom-right corner
        else
            addch(ACS_RTEE);            // Right T-junction
    }

    // Draw all vertical lines
    for (int row = 0; row < GRID_SIZE; row++)
    {
        int y = GRID_START_Y + row * (CELL_HEIGHT + 1) + 1;  // Position between horizontal lines
        for (int col = 0; col <= GRID_SIZE; col++)
        {
            int x = GRID_START_X + col * (CELL_WIDTH + 1);

            // Use blue for 3x3 box boundaries, white for cell boundaries
            if (col % 3 == 0)
                attrset(COLOR_PAIR(7)); // Blue for 3x3 vertical boundaries
            else
                attrset(COLOR_PAIR(6)); // White for cell boundaries

            mvaddch(y, x, ACS_VLINE);   // Draw vertical line character
        }
    }

    // Draw all numbers and handle highlighting
    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            int value = game->grid[row][col];                                   // Current cell value
            int is_given = game->given[row][col];                              // Is this a clue?
            int is_cursor = (row == game->cursor_row && col == game->cursor_col); // Cursor position?

            // Draw the individual cell with appropriate colors
            draw_cell(row, col, value, is_given, is_cursor, game);

            // Draw pencil marks if in mark mode and cell is empty
            if (game->show_marks && value == 0)
            {
                draw_marks(row, col, game->marks[row][col]);
            }
        }
    }

    attrset(COLOR_PAIR(0)); // Reset color attributes
}

/**
 * Check if a cell should be highlighted due to conflicts
 * Determines if the current cell is in the same row, column, or 3x3 box
 * as a cell with an invalid placement
 * 
 * @param game Pointer to the current game state
 * @param check_row Row of the cell to check
 * @param check_col Column of the cell to check
 * @return 1 if cell should be highlighted, 0 otherwise
 */
int should_highlight_cell(game_state_t *game, int check_row, int check_col)
{
    // Scan entire grid for invalid placements
    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            int value = game->grid[row][col];
            // If this cell has an invalid placement
            if (value != 0 && !is_valid_cell_placement(game, row, col, value))
            {
                // Highlight its entire row, column, and 3x3 box

                // Same row as invalid cell
                if (check_row == row)
                    return 1;

                // Same column as invalid cell
                if (check_col == col)
                    return 1;

                // Same 3x3 box as invalid cell
                int invalid_box_start_row = (row / 3) * 3;      // Top-left of invalid cell's box
                int invalid_box_start_col = (col / 3) * 3;
                int check_box_start_row = (check_row / 3) * 3;  // Top-left of check cell's box
                int check_box_start_col = (check_col / 3) * 3;

                // If both cells are in the same 3x3 box
                if (invalid_box_start_row == check_box_start_row &&
                    invalid_box_start_col == check_box_start_col)
                    return 1;
            }
        }
    }
    return 0; // No conflicts affect this cell
}

/**
 * Check if a cell's current value violates Sudoku rules
 * Validates against row, column, and 3x3 box constraints
 * 
 * @param game Pointer to the current game state
 * @param row Row of the cell to check
 * @param col Column of the cell to check
 * @param value Value to validate
 * @return 1 if placement is valid, 0 if it violates rules
 */
int is_valid_cell_placement(game_state_t *game, int row, int col, int value)
{
    // Check for duplicates in the same row
    for (int c = 0; c < GRID_SIZE; c++)
    {
        if (c != col && game->grid[row][c] == value)
            return 0; // Found duplicate in row
    }

    // Check for duplicates in the same column
    for (int r = 0; r < GRID_SIZE; r++)
    {
        if (r != row && game->grid[r][col] == value)
            return 0; // Found duplicate in column
    }

    // Check for duplicates in the same 3x3 box
    int box_start_row = (row / 3) * 3;  // Top-left corner of the 3x3 box
    int box_start_col = (col / 3) * 3;

    for (int r = box_start_row; r < box_start_row + 3; r++)
    {
        for (int c = box_start_col; c < box_start_col + 3; c++)
        {
            // Skip the cell we're checking
            if ((r != row || c != col) && game->grid[r][c] == value)
                return 0; // Found duplicate in 3x3 box
        }
    }

    return 1; // No violations found - valid placement
}

/**
 * Draw an individual cell with appropriate colors and content
 * Handles cursor highlighting, invalid move highlighting, and number display
 * 
 * @param row Row position of the cell
 * @param col Column position of the cell
 * @param value Number in the cell (0 if empty)
 * @param is_given Whether this is a puzzle clue (unchangeable)
 * @param is_cursor Whether the cursor is on this cell
 * @param game Pointer to the current game state
 */
void draw_cell(int row, int col, int value, int is_given, int is_cursor, game_state_t *game)
{
    // Calculate screen position for this cell
    int y = GRID_START_Y + row * (CELL_HEIGHT + 1) + 1;
    int x = GRID_START_X + col * (CELL_WIDTH + 1) + 1;

    // Choose appropriate color based on cell state (priority order matters!)
    if (is_cursor)
    {
        // Cursor position gets highest priority - light blue background
        attron(COLOR_PAIR(COLOR_CURSOR));
    }
    else if (value != 0 && !is_valid_cell_placement(game, row, col, value))
    {
        // Invalid cell gets red background
        attron(COLOR_PAIR(COLOR_INVALID));
    }
    else if (should_highlight_cell(game, row, col))
    {
        // Cells related to invalid placements get red background
        attron(COLOR_PAIR(COLOR_INVALID));
    }
    else if (is_given)
    {
        // Puzzle clues use given color (white)
        attron(COLOR_PAIR(COLOR_GIVEN));
    }
    else
    {
        // User-entered numbers use normal color (white)
        attron(COLOR_PAIR(COLOR_NORMAL));
    }

    // Draw the cell content
    if (value != 0)
    {
        mvprintw(y, x, " %d ", value);  // Display number centered in cell
    }
    else
    {
        mvprintw(y, x, "   ");          // Display empty cell
    }

    // Turn off all color attributes to reset for next cell
    attroff(COLOR_PAIR(COLOR_CURSOR) | COLOR_PAIR(COLOR_GIVEN) | 
            COLOR_PAIR(COLOR_NORMAL) | COLOR_PAIR(COLOR_INVALID));
}

/**
 * Draw pencil marks in an empty cell
 * Shows up to 3 candidate numbers that the player has marked
 * 
 * @param row Row position of the cell
 * @param col Column position of the cell
 * @param marks Array of 9 boolean values indicating which numbers are marked
 */
void draw_marks(int row, int col, int marks[9])
{
    // Calculate screen position for this cell
    int y = GRID_START_Y + row * (CELL_HEIGHT + 1) + 1;
    int x = GRID_START_X + col * (CELL_WIDTH + 1) + 1;

    // Build string of marked numbers (up to 3)
    char mark_str[4] = "   ";  // Initialize with spaces
    int mark_count = 0;

    // Collect first 3 marked numbers
    for (int i = 0; i < 9 && mark_count < 3; i++)
    {
        if (marks[i])
        {
            mark_str[mark_count] = '1' + i;  // Convert 0-8 index to 1-9 character
            mark_count++;
        }
    }

    // Display the marks in normal color
    attron(COLOR_PAIR(COLOR_NORMAL));
    mvprintw(y, x, "%s", mark_str);
    attroff(COLOR_PAIR(COLOR_NORMAL));
}

/**
 * Highlight a specific cell (utility function)
 * Draws a highlighted empty cell at the specified position
 * 
 * @param row Row position to highlight
 * @param col Column position to highlight
 */
void highlight_current_cell(int row, int col)
{
    int y = GRID_START_Y + row * (CELL_HEIGHT + 1) + 1;
    int x = GRID_START_X + col * (CELL_WIDTH + 1) + 1;

    attron(COLOR_PAIR(COLOR_CURSOR));
    mvprintw(y, x, "   ");
    attroff(COLOR_PAIR(COLOR_CURSOR));
}

/**
 * Clear the status message line
 * Removes any previously displayed status messages
 */
void clear_status_line(void)
{
    mvprintw(22, 2, "%-40s", ""); // Clear 40 characters worth of space
    refresh();
}

/**
 * Display completion message when puzzle is solved
 * Shows completion time and move count in green
 * 
 * @param game Pointer to the current game state
 */
void draw_completion_message(game_state_t *game)
{
    attron(COLOR_PAIR(COLOR_COMPLETE));

    // Format and display completion message
    char time_str[32];
    format_time(get_elapsed_time(game), time_str, sizeof(time_str));
    mvprintw(26, 2, "Puzzle completed in %s with %d moves!", time_str, game->moves);

    attroff(COLOR_PAIR(COLOR_COMPLETE));
    refresh();
}

/**
 * Format elapsed time into a readable string
 * Converts seconds into either "M:SS" or "X seconds" format
 * 
 * @param seconds Total elapsed seconds
 * @param buffer Buffer to store formatted string
 * @param buffer_size Size of the buffer
 */
void format_time(int seconds, char *buffer, size_t buffer_size)
{
    int minutes = seconds / 60;
    int secs = seconds % 60;

    if (minutes > 0)
        snprintf(buffer, buffer_size, "%d:%02d", minutes, secs);
    else
        snprintf(buffer, buffer_size, "%d seconds", secs);
}

/**
 * Display a status message to the player
 * Shows temporary messages like error notifications or hints
 * 
 * @param message Text message to display
 */
void draw_status_message(const char *message)
{
    attron(COLOR_PAIR(8));  // Green color for status messages
    mvprintw(22, 2, "%-40s", message); // Clear line and print message
    attroff(COLOR_PAIR(8));
    refresh();
}

/**
 * Legacy compatibility function
 * Provides backward compatibility for existing code that calls redraw_screen
 * 
 * @param game Pointer to the current game state
 */
void redraw_screen(game_state_t *game)
{
    draw_game(game);
}