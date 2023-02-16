cmake_minimum_required(VERSION 3.23)
project(c_realisation C)

set(CMAKE_C_STANDARD 11)

add_executable(c_realisation
        graph_op.c
        file_op.c
        matrix_op.c)