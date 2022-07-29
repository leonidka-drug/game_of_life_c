#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

int** alloc_memory(int** matrix, int rows, int cols);
void clear_memory(int** matrix, int rows);
void read_file(int** matrix, int rows, int cols, char* string);
void read_stdin(int** matrix, int rows, int cols);
int check_cell(int** field, int pos_y, int pos_x, int rows, int cols);
int is_alive(int cell, int neighbours);
void draw(int** field, int rows, int colls);
void play(int** current_field, int** next_field, int size_y, int size_x, int rows, int cols);
void msleep(int milisec);
void input(int* milisec, int* is_playing);
void print_start_menu(const int size_y);
void choose_start(int** matrix, int rows, int cols, char start_key);

int main() {
    const int size_y = 25, size_x = 80, ROWS = size_y - 2, COLS = size_x - 2;
    char start_key;
    int** current_field = NULL;
    int** next_field = NULL;
    current_field = alloc_memory(current_field, ROWS, COLS);
    next_field = alloc_memory(next_field, ROWS, COLS);

    print_start_menu(size_y);
    scanf("%c", &start_key);
    choose_start(current_field, ROWS, COLS, start_key);

    play(current_field, next_field, size_y, size_x, ROWS, COLS);

    clear_memory(current_field, ROWS);
    clear_memory(next_field, ROWS);

    printf("\033c");
    return 0;
}

int** alloc_memory(int** matrix, int rows, int cols) {
    matrix = calloc(rows, sizeof(int*));
    for (int r = 0; r < rows; r++) {
        matrix[r] = calloc(cols, sizeof(int));
    }
    return matrix;
}

void clear_memory(int** matrix, int rows) {
    for (int r = 0; r < rows; r++) {
        free(matrix[r]);
    }
    free(matrix);
}

void read_file(int** matrix, int rows, int cols, char* file_name) {
    FILE* file = fopen(file_name, "r");
    char c;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(file, "%c ", &c);
            if (c == 49) {
                matrix[i][j] = 1;
            } else {
                matrix[i][j] = 0;
            }
        }
    }
}

void read_stdin(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }
}

void play(int** current_field, int** next_field, int size_y, int size_x, int rows, int cols) {
    int is_playing = 1, milisec = 30;
    nodelay(initscr(), 1);
    noecho();

    while (is_playing) {
        clear();
        draw(current_field, size_y, size_x);
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                next_field[row][col] = check_cell(current_field, row, col, rows, cols);
            }
        }
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                current_field[r][c] = next_field[r][c];
            }
        }
        input(&milisec, &is_playing);
        refresh();
        msleep(milisec);
    }
    endwin();
}

int check_cell(int** field, int pos_y, int pos_x, int rows, int cols) {
    int count = 0;
    int col;
    for (int row, r = pos_y - 1; r < pos_y + 2; r++) {
        row = r;
        for (int c = pos_x - 1; c < pos_x + 2; c++) {
            col = c;
            if (row == -1) { row = rows - 1; }
            if (row == rows) { row = 0; }
            if (col == -1) { col = cols - 1; }
            if (col == cols) { col = 0; }
            if (field[row][col]) {
                count++;
            }
        }
    }
    return is_alive(field[pos_y][pos_x], count);
}

int is_alive(int cell, int neighbours) {
    int alive = 0;
    if (cell) {
        neighbours--;
        if (neighbours == 2 || neighbours == 3) {
            alive = 1;
        } else {
            alive = 0;
        }
    } else {
        if (neighbours == 3) {
            alive = 1;
        } else {
            alive = 0;
        }
    }
    return alive;
}

void draw(int** field, int size_y, int size_x) {
    for (int i = 0; i < size_y; i++) {
        for (int j = 0; j < size_x; j++) {
            if (i >= 1 && i <= size_y - 2 && j >= 1 && j <= size_x - 2) {
                if (field[i - 1][j - 1] == 1) {
                    printw("0");
                } else {
                    printw(" ");
                }
            } else if ((i == 0 && j == 0) || (i == size_y - 1 && j == size_x - 1) ||
            (i == size_y - 1 && j == 0) || (i == 0 && j == size_x - 1)) {
                printw("+");
            } else if (i == 0 || i == size_y - 1) {
                printw("-");
            } else if (j == 0 || j == size_x - 1) {
                printw("|");
            }
        }
        printw("\n");
    }
    printw("Control keys: q - exit, 1 - decrease speed, 2 - increase speed ");
}

void msleep(int milisec) {
    milisec *= 1000;
    usleep(milisec);
}

void input(int* milisec, int* is_playing) {
    system("stty raw");
    char key = getch();
    system("stty -raw");

    switch (key) {
        case 50:
            if (*milisec >= 10) {
                *milisec /= 2;
            }
            break;

        case 49:
            if (*milisec <= 250) {
                *milisec *= 2;
            }
            break;

        case 113:
            *is_playing = 0;
            break;

        default:
            break;
    }
}

void print_start_menu(const int size_y) {
    printf("\033c");
    for (int r = 0; r < size_y; r++) {
        if (r == 0 || r == size_y - 1) {
            printf("+------------------------------------------------------------------------------+\n");
        } else if (r == size_y - 21) {
            printf("|                              GAME OF LIFE                                    |\n");
        } else if (r == size_y - 18) {
            printf("|                      1 - start with gun config                               |\n");
        } else if (r == size_y - 16) {
            printf("|                      2 - start with static objects config                    |\n");
        } else if (r == size_y - 14) {
            printf("|                      3 - start with expanding object config                  |\n");
        } else if (r == size_y - 12) {
            printf("|                      4 - start with rake config                              |\n");
        } else if (r == size_y - 10) {
            printf("|                      5 - start with oscillator config                        |\n");
        } else if (r == size_y - 6) {
            printf("|                   To Start choose number and press ENTER                     |\n");
        } else {
            printf("|                                                                              |\n");
        }
    }
}

void choose_start(int** matrix, int rows, int cols, char start_key) {
    char file_name[20] = "file_1.txt";
    switch (start_key) {
        case 50:
            file_name[5] = '2';
            break;

        case 51:
            file_name[5] = '3';
            break;

        case 52:
            file_name[5] = '4';
            break;

        case 53:
            file_name[5] = '5';
            break;
    }
    if (start_key == 64) {
        read_stdin(matrix, rows, cols);
        if (freopen("/dev/tty", "r", stdin) == NULL) {
            printf("Error returning control to standard input\n");
        }
    } else {
        read_file(matrix, rows, cols, file_name);
        if (freopen("/dev/tty", "r", stdin) == NULL) {
            printf("Error returning control to standard input\n");
        }
    }
}
