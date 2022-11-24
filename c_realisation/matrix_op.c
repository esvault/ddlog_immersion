//
// Created by esvault on 18.11.22.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix_op.h"


matrix_t *create_matrix(shape_t shape) {
    matrix_t *result = malloc(sizeof(matrix_t));

    double **data = (double**)malloc(shape.h * sizeof(double*));

    for (int i = 0; i < shape.h; i++) {
        data[i] = (double*)malloc(shape.w * sizeof(double));
        memset(data[i], 0, shape.w * sizeof(double));
    }

    result->shape = shape;
    result->data = data;

    return result;
}

void copy_matrix(matrix_t *new, matrix_t *old) {
    new->shape = old->shape;

    for (size_t i = 0; i < new->shape.h; i++) {
        for (size_t j = 0; j < new->shape.w; j++) {
            new->data[i][j] = old->data[i][j];
        }
    }
}

void free_matrix(matrix_t *matrix) {
    for (int i = 0; i < matrix->shape.h; i++) {
        free(matrix->data[i]);
    }

    free(matrix->data);
    free(matrix);
}

matrix_t *matrix_mul(const matrix_t *left, const matrix_t *right) {
    if (left->shape.w != right->shape.h) {
        fprintf(stderr, "Matrices have incompatible shapes\n");
        exit(-1);
    }

    size_t new_h = left->shape.h;
    size_t new_w = right->shape.w;
    shape_t new_shape = { .h = new_h, .w = new_w };

    matrix_t *result = create_matrix(new_shape);

    for(size_t i = 0; i < new_h; i++) {
        for (size_t j = 0; j < new_w; j++) {
            result->data[i][j] = 0;
            for (size_t k = 0; k < left->shape.w; k++)
                result->data[i][j] += left->data[i][k] * right->data[k][j];
        }
    }

    return result;
}

int matrix_eq(const matrix_t *l, const matrix_t *r) {
    if ((l->shape.h != r->shape.h) || (l->shape.w != r->shape.w)) {
        return 0;
    }

    for (size_t i = 0; i < l->shape.h; i++) {
        for (size_t j = 0; j < l->shape.w; j++) {
            if (l->data[i][j] != r->data[i][j]) {
                return 0;
            }
        }
    }

    return 1;
}

void print_matrix(const matrix_t *matrix) {
    for (size_t i = 0; i < matrix->shape.h; i++) {
        for (size_t j = 0; j < matrix->shape.w; j++) {
            printf("%.4f ", matrix->data[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}