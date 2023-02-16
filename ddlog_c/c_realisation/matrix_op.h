//
// Created by esvault on 18.11.22.
//

#ifndef DDLOG_C_MATRIX_OP_H
#define DDLOG_C_MATRIX_OP_H

#include "stdlib.h"

typedef struct {
    size_t h;
    size_t w;
} shape_t;

typedef struct {
    shape_t shape;
    double **data;
} matrix_t;

matrix_t *create_matrix(shape_t shape);
void copy_matrix(matrix_t *new, matrix_t *old);
void free_matrix(matrix_t *matrix);
matrix_t *matrix_mul(const matrix_t *left, const matrix_t *right);
int matrix_eq(const matrix_t *l, const matrix_t *r);
void print_matrix(const matrix_t *matrix);

#endif //DDLOG_C_MATRIX_OP_H
