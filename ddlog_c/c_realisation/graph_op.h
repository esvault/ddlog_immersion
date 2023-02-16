//
// Created by esvault on 21.11.22.
//

#ifndef C_REALISATION_GRAPH_OP_H
#define C_REALISATION_GRAPH_OP_H

#include "matrix_op.h"

void update_front(matrix_t *res, matrix_t *delta);
double bfs(const matrix_t *adj, int from, int to);
int get_node_with_max_out_degree(const matrix_t *adj);
void simple_bfs(matrix_t *adj, int v);
void bfs_tc(matrix_t *adj);
void transitive_closure(matrix_t *adj);
void delete_edge(const matrix_t *adj, int from, int to);

void reset_global();

#endif //C_REALISATION_GRAPH_OP_H
