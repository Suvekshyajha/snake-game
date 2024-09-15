#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define WIDTH 60
#define HEIGHT 20
#define MAX_LENGTH 100

int score = 0;
int gameover = 0;
int fruitx, fruity;
int x[MAX_LENGTH], y[MAX_LENGTH]; // Arrays to store the coordinates of snake segments
int length = 1; // Initial length of the snake
int direction = 0; // 0 - stop, 1 - up, 2 - right, 3 - down, 4 - left
void print();


char screen[HEIGHT + 1][WIDTH + 1]; // Double buffer to store the game state

// Structure to store player's name and score
struct Player {
    char name[50];
    int score;
};

// Function prototypes
void record(char playerName[]);
void showScores();

void clearScreen() {
    for (int i = 1; i <= HEIGHT; i++) {
        for (int j = 0; j <= WIDTH; j++) {
            screen[i][j] = ' ';
        }
    }
}

void draw() {
    clearScreen();

    // Draw the game board
    for (int i = 1; i <= HEIGHT; i++) {
        for (int j = 0; j <= WIDTH; j++) {
            if (i == 1 || i == HEIGHT) {
                screen[i][j] = '%';
            } else if (j == 0 || j == WIDTH) {
                screen[i][j] = '%';
            } else {
                int isSnakeSegment = 0;
                for (int k = 0; k < length; k++) {
                    if (x[k] == i && y[k] == j) {
                        screen[i][j] = '0';
                        isSnakeSegment = 1;
                        break;
                    }
                }
                if (!isSnakeSegment && i == fruitx && j == fruity)
                    screen[i][j] = '@';
            }
        }
    }

    // Print the score
    sprintf(screen[0], "Score: %d", score);

    // Print the screen buffer
    COORD coord;
    coord.X = 0;
    coord.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    for (int i = 0; i <= HEIGHT; i++) {
        for (int j = 0; j <= WIDTH; j++) {
            printf("%c", screen[i][j]);
        }
        printf("\n");

    }
}

void input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'w':
            direction = 1; // Up
            break;
        case 'd':
            direction = 2; // Right
            break;
        case 's':
            direction = 3; // Down
            break;
        case 'a':
            direction = 4; // Left
            break;
        case 'x':
            gameover = 1; // Exit
            break;
        }
    }
}

void logic() {
    // Move the snake
    for (int i = length - 1; i > 0; i--) {
        x[i] = x[i - 1];
        y[i] = y[i - 1];
    }

    switch (direction) {
    case 1:
        x[0]--;
        break;
    case 2:
        y[0]++;
        break;
    case 3:
        x[0]++;
        break;
    case 4:
        y[0]--;
        break;
    }

    // Wrap around boundaries
    if (x[0] < 0 || x[0] >= HEIGHT || y[0] < 0 || y[0] >= WIDTH)
        gameover = 1;

    // Check if snake eats fruit
    if (x[0] == fruitx && y[0] == fruity) {
        score += 5;
        fruitx = rand() % (HEIGHT - 3) + 1; // Adjust range to avoid border positions
        fruity = rand() % (WIDTH - 3) + 1;  // Adjust range to avoid border positions
        length++; // Increase the length of the snake
    }

    // Check for collision with snake's own body
    for (int i = 1; i < length; i++) {
        if (x[0] == x[i] && y[0] == y[i])
            gameover = 1;
    }
    
}

void record(char playerName[]) {
    FILE *file = fopen("record.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s %d\n", playerName, score);
        fclose(file);
    } else {
        printf("Error: Unable to save score.\n");
    }
}

void showScores() {
    printf("Previous Scores:\n");
    FILE *file = fopen("record.txt", "r");
    if (file != NULL) {
        struct Player player;
        while (fscanf(file, "%s %d", player.name, &player.score) != EOF) {
            printf("%s - %d\n", player.name, player.score);
        }
        fclose(file);
    } else {
        printf("No previous scores found.\n");
    }
}
void print(){
	printf("snake game\nInstructions to control the direction:\n-'a'for left\n-'d'for right\n-'w'for up\n-'s'for down\n-initial length of snake is 1 so,eat'@'as food to score points\n- for each food eaten the score increases by 5 and the length of the snake increases by 1\n-avoid colliding with the walls or colliding wiht the snake's own body\npress any key to continue.....\n");
getch();
system("cls");
}
int main() {
	print();
    // Initialize random seed
    srand(time(NULL));

    // Initialize snake position
    x[0] = HEIGHT / 2;
    y[0] = WIDTH / 2;
    length=1;
    // Initialize fruit position
    fruitx = rand() % (HEIGHT - 3) + 1; // Adjust range to avoid border positions
    fruity = rand() % (WIDTH - 3) + 1;  // Adjust range to avoid border positions
     
    char playerName[50];
    
    printf("Enter your name: ");
    scanf("%s", playerName);
    system("cls");
    // Game loop
    while (!gameover) {
        draw();
        input();
        logic();
        Sleep(70); // Delay in milliseconds
    }

    printf("Game Over!\n");
    printf("Your Score: %d\n", score);
    record(playerName); // Record the score with player's name
    showScores(); // Show scores after game over

	
  

    return 0;
}
