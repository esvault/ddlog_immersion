//
// Created by esvault on 24.11.22.
//

#ifndef C_REALISATION_DDLOG_UTIL_H
#define C_REALISATION_DDLOG_UTIL_H

#include <stdlib.h>
#include <stdbool.h>
#include "ddlog.h"

typedef enum {
    INSERT,
    DELETE
} cmd_t;

bool print_records_callback(uintptr_t arg, const ddlog_record *rec, ssize_t weight);
void print_query_callback(uintptr_t arg, const ddlog_record *rec);
ddlog_cmd **generate_cmds(cmd_t type, table_id id, char **srcs, char **dsts, int len);
int get_num_of_lines(const char *file);
void get_vertecies(const char *file, char **vertecies, size_t field_num);

#endif //C_REALISATION_DDLOG_UTIL_H
