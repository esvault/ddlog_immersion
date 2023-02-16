//
// Created by esvault on 18.11.22.
//
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "matrix_op.h"
#include "file_op.h"


#define NUM_OF_NODES 41383
#define EDGES_NUM 40200

void update_front(matrix_t *res, matrix_t *delta) {
    if (res->shape.h != delta->shape.h && res->shape.w != delta->shape.w) {
        fprintf(stderr, "Incompatible shapes\n");
        exit(-1);
    }

    for (size_t i = 0; i < res->shape.w; i++) {
        if (res->data[0][i] == 0 && delta->data[0][i] == 1) {
            res->data[0][i] = 1;
        }
    }
}

double bfs(const matrix_t *adj, int from, int to) {
    size_t nodes_num = adj->shape.h;
    shape_t vector_shape = { .h = 1, .w = nodes_num};

    matrix_t *front = create_matrix(vector_shape);
    matrix_t *front2 = create_matrix(vector_shape);
    matrix_t *visited = create_matrix(vector_shape);
    matrix_t *old_visited = create_matrix(vector_shape);

    front->data[0][from] = 1;

    clock_t start = clock();

    while (1) {
        copy_matrix(old_visited, visited);

        if (front != NULL) {
            free_matrix(front2);
            front2 = matrix_mul(front, adj);
        } else {
            free_matrix(front2);
            front2 = matrix_mul(visited, adj);\

            free_matrix(front);
            front = NULL;
        }

        update_front(visited, front2);

        if (matrix_eq(visited, old_visited)) {
            break;
        }
    }

    clock_t end = clock();
    return ((double) end - start) / ((double) CLOCKS_PER_SEC);

    int result = visited->data[0][to];

    free_matrix(front2);
    free_matrix(visited);
    free_matrix(old_visited);

    return result;
}

int queue[NUM_OF_NODES], visited[NUM_OF_NODES];
int f = 0, r = -1;

void reset_global() {
    f = 0;
    r = -1;

    for (int i = 0; i < NUM_OF_NODES; i++) {
        queue[i] = 0;
        visited[i] = 0;
    }
}

void simple_bfs(matrix_t *adj, int v) {
    for (int i = 0; i < NUM_OF_NODES; i++) {
        if (adj->data[v][i] && !visited[i]) {
            queue[++r] = i;
        }
    }

    if (f <= r) {
        visited[queue[f]] = 1;
        simple_bfs(adj, queue[f++]);
    }
}

void bfs_tc(matrix_t *adj) {
    for (int i = 0; i < adj->shape.h; ++i) {
        simple_bfs(adj, i);
        reset_global();
    }
}

double my_min(double left, double right) {
    if (left > right) {
        return right;
    } else {
        return left;
    }
}

void transitive_closure(matrix_t *adj) {
    for (int k = 0; k < adj->shape.h; k++) {
        for (int i = 0; i < adj->shape.h; i++) {
            for (int j = 0; j < adj->shape.h; j++) {
                adj->data[i][j] = my_min(adj->data[i][j], adj->data[i][k] + adj->data[k][j]);
            }
        }
    }
}

void delete_edge(const matrix_t *adj, int from, int to) {
    adj->data[from][to] = 0;
}

int get_node_with_max_out_degree(const matrix_t *adj) {
    int n = (int)adj->shape.h;
    int w = (int)adj->shape.w;
    int *vertecies_degree = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        vertecies_degree[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < w; j++) {
            vertecies_degree[i] = vertecies_degree[i] + adj->data[i][j];
        }
    }

    int max = 0;
    for (int i = 0; i < n; i++) {
        if (vertecies_degree[i] > vertecies_degree[max]) {
            max = i;
        }
    }

    return max;
}

//int main() {
//    matrix_t *adj = get_matrix_from_file(FILENAME);
//
//    int result = bfs(adj, 12, 14);
//    if (result) {
//        printf("Vertexes are reachable\n");
//    } else {
//        printf("Vertexes are not reachable\n");
//    }
//
//    free_matrix(adj);
//
//    return 0;
//}


