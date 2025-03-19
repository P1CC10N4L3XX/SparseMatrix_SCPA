cmake_minimum_required(VERSION 3.10)

project(SparseMatrix VERSION 1.0 LANGUAGES C)

set(CMAKE_C_COMPILER gcc)

set(SRC_FILES
    src/main.c
    src/mmio/mmio.c
    src/utils/read.c
)

add_executable(project ${SRC_FILES})
