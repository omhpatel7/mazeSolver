#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 41 // Increased width for better scaling
#define HEIGHT 41 // Increased height for better scaling
#define WALL 1
#define PATH 0

int maze[HEIGHT][WIDTH];
int start_x, start_y, end_x, end_y;

// Directions (up, down, left, right)
int dir[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

typedef struct {
    int x, y;
} Cell;

// Function to check if a position is valid and inside the maze
int is_valid(int x, int y) {
    return x > 0 && y > 0 && x < HEIGHT - 1 && y < WIDTH - 1;
}

// Function to generate the maze using Prim's algorithm
void generate_maze() {
    srand(time(NULL));
    
    // Initialize the maze with walls
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            maze[i][j] = WALL;
        }
    }
    
    // Create a random starting point (must be an odd-numbered position)
    start_x = 1 + 2 * (rand() % ((HEIGHT - 1) / 2));
    start_y = 1 + 2 * (rand() % ((WIDTH - 1) / 2));
    
    // Create a random finishing point (must be an odd-numbered position)
    do {
        end_x = 1 + 2 * (rand() % ((HEIGHT - 1) / 2));
        end_y = 1 + 2 * (rand() % ((WIDTH - 1) / 2));
    } while (end_x == start_x && end_y == start_y);  // Ensure start and end are not the same

    // Mark the start and end points as part of the maze
    maze[start_x][start_y] = PATH;
    maze[end_x][end_y] = PATH;
    
    // List of cells that are part of the maze (initially the starting point)
    Cell cells[WIDTH * HEIGHT];
    int num_cells = 0;
    cells[num_cells++] = (Cell){start_x, start_y};

    while (num_cells > 0) {
        // Pick a random cell from the list
        int random_index = rand() % num_cells;
        Cell current = cells[random_index];
        
        // Remove the picked cell from the list
        cells[random_index] = cells[num_cells - 1];
        num_cells--;
        
        // Check the neighboring cells
        for (int i = 0; i < 4; i++) {
            int nx = current.x + dir[i][0] * 2;
            int ny = current.y + dir[i][1] * 2;
            
            if (is_valid(nx, ny) && maze[nx][ny] == WALL) {
                // Carve a path to the neighboring cell
                maze[current.x + dir[i][0]] [current.y + dir[i][1]] = PATH;
                maze[nx][ny] = PATH;
                
                // Add the neighboring cell to the list
                cells[num_cells++] = (Cell){nx, ny};
            }
        }
    }
}

// Function to output the maze to an SVG file
void output_svg(const char *filename) {
    FILE *file = fopen(filename, "w");
    
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    
    // SVG header
    fprintf(file, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%d\" height=\"%d\">\n", WIDTH * 20, HEIGHT * 20);
    
    // Drawing the maze
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int x = j * 20;
            int y = i * 20;
            if (maze[i][j] == WALL) {
                fprintf(file, "<rect x=\"%d\" y=\"%d\" width=\"20\" height=\"20\" fill=\"black\" />\n", x, y);
            } else {
                // Path is white (background), no need to draw
            }
        }
    }
    
    // Drawing a circle around the start (S) point
    fprintf(file, "<circle cx=\"%d\" cy=\"%d\" r=\"12\" fill=\"none\" stroke=\"white\" stroke-width=\"3\" />\n", start_y * 20 + 10, start_x * 20 + 15);
    
    // Drawing a circle around the end (E) point
    fprintf(file, "<circle cx=\"%d\" cy=\"%d\" r=\"12\" fill=\"none\" stroke=\"yellow\" stroke-width=\"3\" />\n", end_y * 20 + 10, end_x * 20 + 15);

    // Drawing the start (S) and end (E) points with more contrast and bold
    fprintf(file, "<text x=\"%d\" y=\"%d\" font-family=\"Arial\" font-size=\"20\" font-weight=\"bold\" fill=\"white\" stroke=\"black\" stroke-width=\"2\" text-anchor=\"middle\" dy=\"0.4em\">S</text>\n", start_y * 20 + 10, start_x * 20 + 15);
    fprintf(file, "<text x=\"%d\" y=\"%d\" font-family=\"Arial\" font-size=\"20\" font-weight=\"bold\" fill=\"yellow\" stroke=\"black\" stroke-width=\"2\" text-anchor=\"middle\" dy=\"0.4em\">E</text>\n", end_y * 20 + 10, end_x * 20 + 15);
    
    // SVG footer
    fprintf(file, "</svg>\n");
    
    fclose(file);
    printf("Maze saved as %s\n", filename);
}

int main() {
    generate_maze();
    output_svg("maze.svg");
    return 0;
}
