//
// Created by esvault on 19.11.22.
//
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "../ddlog_realisation/ddlog_util.h"
#include "../ddlog_realisation/reachability_ddlog/ddlog.h"
#include "../c_realisation/graph_op.h"
#include "../c_realisation/file_op.h"
#include "../c_realisation/matrix_op.h"

#define FILENAME "../graphs/eclipse/eclipse.csv"

#define NUM_OF_NODES 41383

typedef struct {
    double first;
    double second;
} pair_t;

extern int visited[NUM_OF_NODES];

double c_time(matrix_t *adj, int v, int u) {
    clock_t start = clock();

//    int v = get_node_with_max_out_degree(adj);
    simple_bfs(adj, v);

    int result = visited[u];

//    free_matrix(adj);

    reset_global();

    clock_t end = clock();
//    printf("C realisation was executed in %.8f second(s)\n", ((double) end - start) / ((double) CLOCKS_PER_SEC));

    return ((double) end - start) / ((double) CLOCKS_PER_SEC);
}



void ddlog_delete_edges(ddlog_prog *prog, table_id id, int *from, int *to, int len) {
    char **srcs = malloc(len * sizeof(char*));
    char **dsts = malloc(len * sizeof(char*));

    for (int i = 0; i < len; i++) {
        srcs[i] = malloc(10 * sizeof(char));
        dsts[i] = malloc(10 * sizeof(char));
        sprintf(srcs[i], "%d", from[i]);
        sprintf(dsts[i], "%d", to[i]);
    }

    ddlog_cmd **cmds = generate_cmds(DELETE, id, srcs, dsts, len);

    ddlog_transaction_start(prog);

    ddlog_apply_updates(prog, cmds, len);

    ddlog_transaction_commit(prog);

    for (int i = 0; i < len; i++) {
        free(srcs[i]);
        free(dsts[i]);
    }

    free(srcs);
    free(dsts);
}

ddlog_prog *init_ddlog_prog_time() {
    ddlog_prog prog = ddlog_run(1, true, NULL, NULL);
    if (prog == NULL) {
        fprintf(stderr, "failed to initialize DDlog program\n");
        exit(EXIT_FAILURE);
    }

    table_id EdgesTableID = ddlog_get_table_id(prog, "Edges");
    table_id ConnectedNodesTableID = ddlog_get_table_id(prog, "ConnectedNodes");


    int edges_num = get_num_of_lines(FILENAME);

    char **srcs = (char**)malloc(edges_num * sizeof(char*));
    char **dsts = (char**)malloc(edges_num * sizeof(char*));

    get_vertecies(FILENAME, srcs, 1);
    get_vertecies(FILENAME, dsts, 2);

    ddlog_transaction_start(prog);

    ddlog_cmd **cmds = generate_cmds(INSERT, EdgesTableID, srcs, dsts, edges_num);

    ddlog_apply_updates(prog, cmds, edges_num);

    ddlog_transaction_commit(prog);

//    ddlog_dump_table(prog, EdgesTableID, )

    return prog;
}

double ddlog_time(ddlog_prog *prog, index_id id, int v, int u) {

//    printf("DDlog initialisation was executed in %.8f second(s)\n", ((double) end1 - start1) / ((double) CLOCKS_PER_SEC));

    clock_t start = clock();

    ddlog_record **struct_args = (ddlog_record**)malloc(2 * sizeof(ddlog_record*));

    char src[7];
    char dst[7];

    sprintf(src, "%d", v);
    sprintf(dst, "%d", u);

    struct_args[0] = ddlog_string(src);
    struct_args[1] = ddlog_string(dst);

    ddlog_record *general_struct = ddlog_tuple(struct_args, 2);

    ddlog_query_index(prog, id, general_struct, &print_query_callback, (uintptr_t)(void*)(NULL));

//    ddlog_dump_index(prog, id, print_query_callback, (uintptr_t)(void*)(NULL));

//    ddlog_dump_table(prog, (ddlog_get_table_id(prog, "Edges")), &print_records_callback, (uintptr_t)(void*)(NULL));


    clock_t end = clock();

//    printf("DDlog query was executed in %.8f second(s)\n", ((double) end - start) / ((double) CLOCKS_PER_SEC));

    return ((double) end - start) / ((double) CLOCKS_PER_SEC);

}

double count_average(matrix_t *m) {
    double res = 0.0;
    for (int i = 0; i < m->shape.w; ++i) {
        res += m->data[0][i];
    }

    return res / m->shape.w;
}

int main() {
    const int experiments_num = 5;

    shape_t shape = { .h=1, .w=experiments_num };
    matrix_t *c_result = create_matrix(shape);
    matrix_t *d_result = create_matrix(shape);

    matrix_t *adj = get_matrix_from_file(FILENAME);
    ddlog_prog *prog = init_ddlog_prog_time();
    table_id EdgesTableID = ddlog_get_table_id(prog, "Edges");
    table_id ConnectedNodesTableID = ddlog_get_table_id(prog, "ConnectedNodes");
    index_id ReachabilityIndexID = ddlog_get_index_id(prog, "Reachability");

//    int max_out_degree = 832;

//// On the same nodes
    for (int i = 0; i < experiments_num; i++) {
//        printf("Iteration: %d\n", i);
        c_result->data[0][i] = c_time(adj, 832, 1);
        d_result->data[0][i] = ddlog_time(prog, ReachabilityIndexID, 832, 1);
    }

    double c = count_average(c_result);
    double d = count_average(d_result);

    printf("\nAlgorithms are started on the same nodes without deleting\n");
    printf("C realisation time: %.4f s\n", c);
    printf("DDlog realisation time: %.4f s\n", d);

//    print_matrix(c_result);
//    print_matrix(d_result);

//// After deleting some edges
    int srcs[] = {1, 832, 433};
    int dsts[] = {0, 453, 200};
    int len = sizeof(srcs) / sizeof(srcs[0]);

    for (int i = 0; i < len; i++) {
        delete_edge(adj, srcs[i], dsts[i]);
    }

    ddlog_delete_edges(prog, EdgesTableID, srcs, dsts, len);

    for (int i = 0; i < experiments_num; i++) {
//        printf("Iteration: %d\n", i);
        c_result->data[0][i] = c_time(adj, 832, 1);
        d_result->data[0][i] = ddlog_time(prog, ReachabilityIndexID, 832, 1);
    }

    c = count_average(c_result);
    d = count_average(d_result);

    printf("\nAlgorithms are started on the same nodes with deleting\n");
    printf("C realisation time: %.4f s\n", c);
    printf("DDlog realisation time: %.4f s\n", d);

//    print_matrix(c_result);
//    print_matrix(d_result);

//    ddlog_dump_table(prog, ConnectedNodesTableID, &print_records_callback, (uintptr_t)(void*)(NULL));

    ddlog_stop(prog);

    free(adj);
    free_matrix(c_result);
    free_matrix(d_result);

    return EXIT_SUCCESS;
}