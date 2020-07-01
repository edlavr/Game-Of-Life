#include<stdio.h>

#include<string.h>

#include <stdlib.h>



#include"gol.h"


int main(int argc, char * argv[255]) {
    // initialising values
    int i = 0, o = 0, g = 0, s = 0, t = 0;
    char input_filename[255];
    char output_filename[255];
    long int number_of_generations = 5;
    // parsing arguments
    for (int counter = 1; counter < argc; counter++) {
        char
        const * arg = argv[counter];
        if (arg[0] == '-') {
            if (arg[2]) {
                fprintf(stderr, "Flag is not recognised.\n");
                exit(1);
            }
            switch (arg[1]) {
                case 'i':
                    i++;
                    if (argv[counter + 1] == NULL) {
                        fprintf(stderr, "Misses input file.\n");
                        exit(1);
                    }
                    if (i > 1) {
                        if (strcmp(argv[counter + 1], input_filename) != 0) {
                            fprintf(stderr, "Conflicting input files.\n");
                            exit(1);
                        }
                    }

                    strcpy(input_filename, argv[counter + 1]);
                    counter++;
                    break;

                case 'o':
                    o++;
                    if (argv[counter + 1] == NULL) {
                        fprintf(stderr, "Misses output file.\n");
                        exit(1);
                    }
                    if (o > 1) {
                        if (strcmp(argv[counter + 1], output_filename) != 0) {
                            fprintf(stderr, "Conflicting output files.\n");
                            exit(1);
                        }
                    }

                    strcpy(output_filename, argv[counter + 1]);
                    counter++;

                    break;

                case 'g':
                    g++;

                    if (argv[counter + 1] == NULL) {
                        fprintf(stderr, "Misses number of generations\n");
                        exit(1);
                    }

                    if (g > 1) {
                        if (strtol(argv[counter + 1], NULL, 10) != number_of_generations) {
                            fprintf(stderr, "Conflicting number of generations\n");
                            exit(1);
                        }
                    }

                    number_of_generations = strtol(argv[counter + 1], NULL, 10);

                    counter++;


                    break;


                case 's':
                    s++;
                    break;
                case 't':
                    t++;
                    break;
                default:
                    fprintf(stderr, "Flag is not recognised.\n");
                    exit(1);
            }
        } else {
            fprintf(stderr, "%s is not a flag.\n", arg);
            exit(1);
        }
    }

    struct universe v;
    FILE * infile = NULL;

    if (i == 0) {
        read_in_file(stdin, & v);
    } else {
        infile = fopen(input_filename, "r");
        if (infile == NULL) {
            fprintf(stderr, "Could not open specified input file.\n");
            exit(1);
        }
        read_in_file(infile, & v);
        fclose(infile);
    }

    for(int gg = 0;gg < number_of_generations;gg++) {
        if (t == 1) {
            evolve( & v, will_be_alive_torus);
        } else {
            evolve( & v, will_be_alive);
        }
    }
    if (s > 0) {
        print_statistics( & v);
    }

    if (o > 0) {
        FILE * outfile;
        outfile = fopen(output_filename, "w");
        write_out_file(outfile, & v);

    }
    else {
        write_out_file(stdout, & v);
    }
}
