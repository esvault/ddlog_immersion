//
// Created by esvault on 18.11.22.
//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "matrix_op.h"

#define NODES_NUM 41383
#define EDGES_NUM 40200

const char* getfield_(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ";");
         tok && *tok;
         tok = strtok(NULL, ";\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

int get_num_of_lines_(const char *file) {
    FILE *stream = fopen(file, "r");

    int count = 0;
    for (char c = getc(stream); c != EOF; c = getc(stream))
        if (c == '\n')
            count = count + 1;

    fclose(stream);

    return count;
}

void get_vertecies_(const char *file, char **vertecies, size_t field_num) {
    FILE *stream = fopen(file, "r");

    if (stream == NULL) {
        fprintf(stderr, "File %s not found\n", file);
        exit(-1);
    }

    char line[1024];
    int index = 0;
    while (fgets(line, 1024, stream))
    {
        char* tmp = strdup(line);
        vertecies[index] = (char*)malloc(1024 * sizeof(char));
        sprintf(vertecies[index], "%s", getfield_(tmp, field_num));
        // (*vertecies)[index] = getfield_(tmp, field_num);
//         printf("Field %ld would be %s\n", field_num, getfield_(tmp, field_num));
        // NOTE strtok clobbers tmp
        free(tmp);
        index = index + 1;
    }

    fclose(stream);
}

matrix_t *get_matrix_from_file(const char *filename) {
    char **srcs = (char**)malloc(EDGES_NUM * sizeof(char*));
    if (srcs == NULL) {
        fprintf(stderr, "Allocation error\n");
        exit(-1);
    }

    char **dsts = (char**)malloc(EDGES_NUM * sizeof(char*));
    if (dsts == NULL) {
        fprintf(stderr, "Allocation error\n");
        exit(-1);
    }

    get_vertecies_(filename, srcs, 2);
    get_vertecies_(filename, dsts, 1);

    matrix_t *result = create_matrix((shape_t) { .h = NODES_NUM, .w = NODES_NUM});

    for (int i = 0; i < EDGES_NUM; i++) {
        int from = atoi(srcs[i]);
        int to = atoi(dsts[i]);
        result->data[from][to] = 1;
    }

    free(srcs);
    free(dsts);

    return result;
}