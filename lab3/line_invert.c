//
// Created by panton8 on 2/13/24.
//

#include <stdio.h>
#include <string.h>
#include "line_invert.h"

#define MAX_LINE_LENGTH 1000

void line_invert(FILE *input_file, FILE *output_file) {
    char line[MAX_LINE_LENGTH];
    char lines[MAX_LINE_LENGTH][MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(line, MAX_LINE_LENGTH, input_file)) {
        int len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        strcpy(lines[count], line);
        count++;
    }

    for (int i = count - 1; i > 0; i--) {
        fprintf(output_file, "%s\n", lines[i]);
    }

    fprintf(output_file, "%s", lines[0]);
}


