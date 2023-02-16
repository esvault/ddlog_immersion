#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h> // required for strncmp()
#include <errno.h>  // required for error code identifiers

#include "ddlog.h"
#include "ddlog_util.h"

int RESULT = 0;

bool print_records_callback(uintptr_t arg, const ddlog_record *rec, ssize_t weight) {
    char *record_as_string = ddlog_dump_record(rec);
    if (record_as_string == NULL) {
        fprintf(stderr, "failed to dump record\n");
        exit(EXIT_FAILURE);
    }
    char *action = (weight == 1) ? "Inserted" : "Deleted";
    printf("%s record: %s\n", action, record_as_string);
    ddlog_string_free(record_as_string);

    return true;
}

void print_query_callback(uintptr_t arg, const ddlog_record *rec) {
//    RESULT = RESULT + 1;
    char *record_as_string = ddlog_dump_record(rec);

    printf("%s\n", record_as_string);
    ddlog_string_free(record_as_string);
}

ddlog_cmd **generate_cmds(cmd_t type, table_id id, char **srcs, char **dsts, int len) {
    ddlog_cmd **cmds = (ddlog_cmd**)malloc(len * sizeof(ddlog_cmd*));

    for (int i = 0; i < len; i++) {
        ddlog_record **struct_args = (ddlog_record**)malloc(3 * sizeof(ddlog_record*));
        ddlog_record *src = ddlog_string(srcs[i]);
        ddlog_record *dst = ddlog_string(dsts[i]);
        ddlog_record *flg = ddlog_bool(true);

        struct_args[0] = src;
        struct_args[1] = dst;
        struct_args[2] = flg;

        ddlog_record *general_record = ddlog_struct("Edges", struct_args, 3);

//        char *record_to_insert_as_string = ddlog_dump_record(general_record);
//        printf("Inserting the following record: %s\n", record_to_insert_as_string);
//        ddlog_string_free(record_to_insert_as_string);

        if (type == INSERT) {
            cmds[i] = ddlog_insert_cmd(id, general_record);
        } else {
            cmds[i] = ddlog_delete_val_cmd(id, general_record);
        }

        //fprintf(stdout, "%s", "I'm here\n");
        free(struct_args);
        //free(src);
        //free(dst);
        //free(flg);
        //free(general_record);
    }

    return cmds;
    
} 

const char* getfield(char* line, int num) {
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

int get_num_of_lines(const char *file) {
    FILE *stream = fopen(file, "r");

    if (stream == NULL) {
        fprintf(stderr, "Cannot open file %s\n", file);
        exit(-1);
    }

    int count = 0;
    for (char c = getc(stream); c != EOF; c = getc(stream))
        if (c == '\n') 
            count = count + 1;

    fclose(stream);

    return count;
}

void get_vertecies(const char *file, char **vertecies, size_t field_num) {
    FILE *stream = fopen(file, "r");

    char line[1024];
    int index = 0;
    while (fgets(line, 1024, stream))
    {
        char* tmp = strdup(line);
	vertecies[index] = (char*)malloc(1024 * sizeof(char));
	sprintf(vertecies[index], "%s", getfield(tmp, field_num));
	// (*vertecies)[index] = getfield(tmp, field_num);
        // printf("Field %ld would be %s\n", field_num, getfield(tmp, field_num));
        // NOTE strtok clobbers tmp
        free(tmp);
	index = index + 1;
    }

    fclose(stream);
}

//const char* FILENAME = "../bzip/bzip.csv";
//
//int main() {
//    printf("DDlog program!\n");
//    ddlog_prog prog = ddlog_run(1, true, NULL, NULL);
//    if (prog == NULL) {
//        fprintf(stderr, "failed to initialize DDlog program\n");
//	exit(EXIT_FAILURE);
//    }
//
//    table_id EdgesTableID = ddlog_get_table_id(prog, "Edges");
//    table_id ConnectedNodesTableID = ddlog_get_table_id(prog, "ConnectedNodes");
//    index_id ReachabilityIndexID = ddlog_get_index_id(prog, "Reachability");
//
//    int edges_num = get_num_of_lines(FILENAME);
//
//    char **srcs = (char**)malloc(edges_num * sizeof(char*));
//    char **dsts = (char**)malloc(edges_num * sizeof(char*));
//
//    get_vertecies(FILENAME, srcs, 1);
//    get_vertecies(FILENAME, dsts, 2);
//
//    ddlog_transaction_start(prog);
//
//    ddlog_cmd **cmds = generate_cmds(EdgesTableID, srcs, dsts, edges_num);
//
//    ddlog_apply_updates(prog, cmds, edges_num);
//
//    ddlog_transaction_commit(prog);
//
//    char src[15];
//    char dst[15];
//
//    while (strcmp(src, "a") != 0 && strcmp(dst, "a") != 0) {
//        printf("Enter the source node and destination node: \n");
//
//
//        scanf("%s %s", src, dst);
//
//        ddlog_record **struct_args = (ddlog_record**)malloc(2 * sizeof(ddlog_record*));
//        ddlog_record *src_rec = ddlog_string(src);
//        ddlog_record *dst_rec = ddlog_string(dst);
//
//        struct_args[0] = ddlog_string(src);
//        struct_args[1] = ddlog_string(dst);
//
//        ddlog_record *general_struct = ddlog_tuple(struct_args, 2);
//
//        ddlog_query_index(prog, ReachabilityIndexID, general_struct, &print_query_callback, (uintptr_t)(void*)(NULL));
//
//        if (RESULT > 0) {
//            printf("Yes\n");
//        } else {
//            printf("No\n");
//        }
//
//        RESULT = 0;
//    }
//
//  //  ddlog_dump_index(prog, ReachabilityIndexID, print_query_callback, (uintptr_t)(void*)(NULL));
//
//  //  ddlog_dump_table(prog, ConnectedNodesTableID, &print_records_callback, (uintptr_t)(void*)(NULL));
//
//    ddlog_stop(prog);
//
//    return EXIT_SUCCESS;
//}
