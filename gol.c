#include<stdio.h>

#include <stdlib.h>

#include"gol.h"

void read_in_file(FILE * infile, struct universe * u) {
    // reading the file and initialising values
    char c;
    ( * u).alive = 0;
    ( * u).rows = 0;
    ( * u).cols = 0;
    ( * u).max_cols = 0;
    ( * u).alive_total = 0;
    ( * u).n_of_gens = 1;

    // malloc for rows
    ( * u).array = malloc(sizeof(char * ));
    if (( * u).array == NULL) {
        fprintf(stderr, "Malloc Error.\n");
        exit(1);
    }

    // malloc for columns
    ( * u).array[0] = malloc(sizeof(char * ) * 512);
    if (( * u).array[0] == NULL) {
        fprintf(stderr, "Malloc Error.\n");
        exit(1);
    }

    // reading the input file character by character
    while ((c = fgetc(infile)) && c != EOF) {
        if (c == '*') {
            ( * u).alive++;
        } else if (c == '.' ||  c == '\n') {

        } else {
            fprintf(stderr, "Invalid character in the input.\n");
            exit(1);
        }

        if (( * u).rows == 0) {
            if (c != '\n') {
                ( * u).array[( * u).rows][( * u).cols] = c;
                ( * u).cols++;
                if (( * u).cols > 512) {
                    fprintf(stderr, "Too many columns.\n");
                    exit(1);
                }
            } else {
                ( * u).max_cols = ( * u).cols;
                ( * u).rows++;
                ( * u).array = realloc(( * u).array, (sizeof(char * )) * ( * u).rows);
                if (( * u).array == NULL) {
                    fprintf(stderr, "Realloc Error.\n");
                    exit(1);
                }
                // malloc for the second row
                ( * u).array[1] = malloc((sizeof(char * )) * ( * u).max_cols);
                if (( * u).array[1] == NULL) {
                    fprintf(stderr, "Malloc Error.\n");
                    exit(1);
                }
                ( * u).cols = 0;
            }
        } else {
            if (c != '\n') {
                ( * u).array[( * u).rows][( * u).cols] = c;
                ( * u).cols++;
            } else {
                if (( * u).cols != ( * u).max_cols) {
                    fprintf(stderr, "The grid is not a rectangle.\n");
                    exit(1);
                }
                ( * u).rows++;
                // expanding row by row
                ( * u).array = realloc(( * u).array, sizeof(char * ) * ( * u).rows * ( * u).max_cols);
                if (( * u).array == NULL) {
                    fprintf(stderr, "Realloc Error.\n");
                    exit(1);
                }
                // malloc for n-th row
                ( * u).array[( * u).rows] = malloc((sizeof(char * )) * ( * u).max_cols);
                if (( * u).array[( * u).rows] == NULL) {
                    fprintf(stderr, "Malloc Error.\n");
                    exit(1);
                }
                ( * u).cols = 0;
            }
        }
    }

    ( * u).alive_total += ( * u).alive;

}

void write_out_file(FILE * outfile, struct universe * u) {
    for (int counter = 0; counter < ( * u).rows; counter++) {
        for (int counter1 = 0; counter1 <= ( * u).max_cols; counter1++) {
            if (counter1 != ( * u).max_cols) {
                fprintf(outfile, "%c", ( * u).array[counter][counter1]);
            } else {
                fprintf(outfile, "\n");
            }
        }
    }

    // freeing memory
    for (int i = 0; i < ( * u).rows; i++)
        free(( * u).array[i]);
    free(( * u).array);

}

int is_alive(struct universe * u, int column, int row) {
    if (( * u).array[row][column] == '*') {
        return 1;
    } else {
        return 0;
    }
}

int will_be_alive(struct universe * u, int column, int row) {
    // counting alive neighbors
    int n_alive = 0;

    // top
    if (row - 1 >= 0) {
        if (( * u).array[row - 1][column] == '*') {
            n_alive++;
        }
    }
    // top right
    if (row - 1 >= 0 && column + 1 < ( * u).max_cols) {
        if (( * u).array[row - 1][column + 1] == '*') {
            n_alive++;
        }
    }
    // right
    if (column + 1 < ( * u).max_cols) {
        if (( * u).array[row][column + 1] == '*') {
            n_alive++;
        }
    }
    // bottom right
    if (row + 1 < ( * u).rows && column + 1 < ( * u).max_cols) {
        if (( * u).array[row + 1][column + 1] == '*') {
            n_alive++;
        }
    }
    // bottom
    if (row + 1 < ( * u).rows) {
        if (( * u).array[row + 1][column] == '*') {
            n_alive++;
        }
    }
    // bottom left
    if (row + 1 < ( * u).rows && column - 1 >= 0) {
        if (( * u).array[row + 1][column - 1] == '*') {
            n_alive++;
        }
    }
    // left
    if (column - 1 >= 0) {
        if (( * u).array[row][column - 1] == '*') {
            n_alive++;
        }
    }
    // top left
    if (row - 1 >= 0 && column - 1 >= 0) {
        if (( * u).array[row - 1][column - 1] == '*') {
            n_alive++;
        }
    }

    // Convay's rule
    if (is_alive(u, column, row) == 1) {
        if (n_alive == 2 || n_alive == 3) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if (n_alive == 3) {
            return 1;
        } else {
            return 0;
        }
    }
}

int will_be_alive_torus(struct universe * u, int column, int row) {
    // counting alive neighbors with the topology of a torus
    int n_alive = 0;

    // top
    if (row - 1 >= 0) {
        if (( * u).array[row - 1][column] == '*') {
            n_alive++;
        }
    } else {
        if (( * u).array[( * u).rows - 1][column] == '*') {
            n_alive++;
        }
    }
    // right
    if (column + 1 < ( * u).max_cols) {
        if (( * u).array[row][column + 1] == '*') {
            n_alive++;
        }
    } else {
        if (( * u).array[row][0] == '*') {
            n_alive++;
        }
    }
    // bottom
    if (row + 1 < ( * u).rows) {
        if (( * u).array[row + 1][column] == '*') {
            n_alive++;
        }
    } else {
        if (( * u).array[0][column] == '*') {
            n_alive++;
        }
    }
    // left
    if (column - 1 >= 0) {
        if (( * u).array[row][column - 1] == '*') {
            n_alive++;
        }
    } else {
        if (( * u).array[row][( * u).max_cols - 1] == '*') {
            n_alive++;
        }
    }

    // top right
    if (row - 1 >= 0 && column + 1 < ( * u).max_cols) {
        if (( * u).array[row - 1][column + 1] == '*') {
            n_alive++;
        }
    } else {
        if (row - 1 < 0 && column + 1 < ( * u).max_cols) {
            //row oob
            if (( * u).array[( * u).rows - 1][column + 1] == '*') {
                n_alive++;
            }
        } else if (row - 1 >= 0 && column + 1 >= ( * u).max_cols) {
            //col oob
            if (( * u).array[row - 1][0] == '*') {
                n_alive++;
            }
        } else {
            //row col oob
            if (( * u).array[( * u).rows - 1][0] == '*') {
                n_alive++;
            }
        }
    }
    // top left
    if (row - 1 >= 0 && column - 1 >= 0) {
        if (( * u).array[row - 1][column - 1] == '*') {
            n_alive++;
        }
    } else {
        if (row - 1 < 0 && column - 1 >= 0) {
            //row oob
            if (( * u).array[( * u).rows - 1][column - 1] == '*') {
                n_alive++;
            }
        } else if (row - 1 >= 0 && column - 1 < 0) {
            //col oob
            if (( * u).array[row - 1][( * u).max_cols - 1] == '*') {
                n_alive++;
            }
        } else {
            //row col oob
            if (( * u).array[( * u).rows - 1][( * u).max_cols - 1] == '*') {
                n_alive++;
            }
        }
    }
    // bottom right
    if (row + 1 < ( * u).rows && column + 1 < ( * u).max_cols) {
        if (( * u).array[row + 1][column + 1] == '*') {
            n_alive++;
        }
    } else {
        if (row + 1 >= ( * u).rows && column + 1 < ( * u).max_cols) {
            //row oob
            if (( * u).array[0][column + 1] == '*') {
                n_alive++;
            }
        } else if (row + 1 < ( * u).rows && column + 1 >= ( * u).max_cols) {
            //col oob
            if (( * u).array[row + 1][0] == '*') {
                n_alive++;
            }
        } else {
            //row col oob
            if (( * u).array[0][0] == '*') {
                n_alive++;
            }
        }
    }
    // bottom left
    if (row + 1 < ( * u).rows && column - 1 >= 0) {
        if (( * u).array[row + 1][column - 1] == '*') {
            n_alive++;
        }
    } else {
        if (row + 1 >= ( * u).rows && column - 1 >= 0) {
            //row oob
            if (( * u).array[0][column - 1] == '*') {
                n_alive++;
            }
        } else if (row + 1 < ( * u).rows && column - 1 < 0) {
            //col oob
            if (( * u).array[row + 1][( * u).max_cols - 1] == '*') {
                n_alive++;
            }
        } else {
            //row col oob
            if (( * u).array[0][( * u).max_cols - 1] == '*') {
                n_alive++;
            }
        }
    }

    // Convay's rule
    if (is_alive(u, column, row) == 1) {
        if (n_alive == 2 || n_alive == 3) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if (n_alive == 3) {
            return 1;
        } else {
            return 0;
        }
    }
}

void evolve(struct universe * u, int( * rule)(struct universe * u, int column, int row)) {
    // making a temporary grid with the same size as the original one. We need a this grid
    // for evolving so that the already evolved cells do not affect the not yet evolved ones
    ( * u).alive = 0;
    ( * u).temp = malloc(sizeof(char * ) * ( * u).rows);
    if (( * u).temp == NULL) {
        fprintf(stderr, "Malloc Error.\n");
        exit(1);
    }
    for (int i = 0; i < ( * u).rows; i++) {
        ( * u).temp[i] = malloc(sizeof(char) * ( * u).max_cols);
        if (( * u).temp[i] == NULL) {
            fprintf(stderr, "Malloc Error.\n");
            exit(1);
        }
    }

    // evolving the temporary grid
    for (int i = 0; i < ( * u).rows; i++) {
        for (int j = 0; j < ( * u).max_cols; j++) {
            if (( * rule)(u, j, i) == 1) {
                ( * u).temp[i][j] = '*';
                ( * u).alive += 1;
            } else {
                ( * u).temp[i][j] = '.';
            }
        }
    }

    // copying the evolved grid to the original one
    for (int i = 0; i < ( * u).rows; i++) {
        for (int j = 0; j < ( * u).max_cols; j++) {
            ( * u).array[i][j] = ( * u).temp[i][j];
        }
    }

    // for statistics
    ( * u).alive_total += ( * u).alive;
    ( * u).n_of_gens += 1;

    // freeing memory
    for (int i = 0; i < ( * u).rows; i++)
        free(( * u).temp[i]);
    free(( * u).temp);

}

void print_statistics(struct universe * u) {
    // calculating and printing stats
    float total_cells = ( * u).rows * ( * u).max_cols;
    float current = (( * u).alive / total_cells) * 100;
    float average = ((( * u).alive_total / ( * u).n_of_gens) / total_cells) * 100;
    printf("%0.3f%% of cells currently alive\n", current);
    printf("%0.3f%% of cells alive on average\n", average);
}
