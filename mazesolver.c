#include <stdio.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#define WIDTH 21
#define HEIGHT 21
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
    return x >= 0 && y >= 0 && x < HEIGHT && y < WIDTH;
}

// Function to print the maze
void print_maze() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (maze[i][j] == WALL) {
                printf("█");
            } else if (i == start_x && j == start_y) {
                printf("S");  // Start point
            } else if (i == end_x && j == end_y) {
                printf("E");  // End point
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
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

// Depth-First Search (DFS) to find a path from start to end
int solve_maze(int x, int y) {
    if (x == end_x && y == end_y) {
        return 1;  // Reached the end
    }
    
    // Mark the current cell as part of the solution path
    maze[x][y] = 2;  // 2 indicates part of the solution path
    
    // Try all possible directions (up, down, left, right)
    for (int i = 0; i < 4; i++) {
        int nx = x + dir[i][0];
        int ny = y + dir[i][1];
        
        if (is_valid(nx, ny) && maze[nx][ny] == PATH) {
            if (solve_maze(nx, ny)) {
                return 1;  // Return true if the path is found
            }
        }
    }
    
    // If no path is found, backtrack by marking the current cell as visited
    maze[x][y] = 3;  // 3 indicates visited but not part of the solution
    return 0;  // No path found
}

int main() {
    // Step 1: Generate the maze
    generate_maze();
    
    // Step 2: Print the generated maze
    printf("Generated Maze:\n");
    print_maze();
    
    // Step 3: Solve the maze using DFS
    if (solve_maze(start_x, start_y)) {
        printf("\nSolved Maze:\n");
        print_maze();
    } else {
        printf("\nNo path found from S to E.\n");
    }
    
    return 0;
}
