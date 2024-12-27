#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "line_invert.h"

int main(int argc, char *argv[]) {
    FILE *input_file;
    FILE *output_file;
    int opt;
    char *output_filename = NULL;

    while ((opt = getopt(argc, argv, "o:")) != -1) {
        switch (opt) {
            case 'o':
                output_filename = optarg;
                break;
            default:
                fprintf(stderr, "Использование: %s [-o output_file] input_file\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }


    if (optind >= argc) {
        fprintf(stderr, "Не указан входной файл\n");
        exit(EXIT_FAILURE);
    }

    input_file = fopen(argv[optind], "r");
    if (input_file == NULL) {
        perror("Не удалось открыть входной файл");
        exit(EXIT_FAILURE);
    }

    if (output_filename != NULL) {
        output_file = fopen(output_filename, "w");
        if (output_file == NULL) {
            perror("Не удалось открыть выходной файл");
            fclose(input_file);
            exit(EXIT_FAILURE);
        }
    } else {
        output_file = stdout;
    }

    line_invert(input_file, output_file);

    fclose(input_file);
    if (output_filename != NULL) {
        fclose(output_file);
    }

    return 0;
}

// make
// ./line_invert input.txt output.txt > output.txt
// rm -f *.o line_invert
// make test