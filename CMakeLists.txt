cmake_minimum_required(VERSION 3.10)

project(SparseMatrix VERSION 1.0 LANGUAGES C)

set(CMAKE_C_COMPILER gcc)

set(SRC_FILES
    src/main.c
    src/mmio/mmio.c
    src/utils/MRKT_utils.c
    src/utils/CSR_utils.c
    src/utils/HLL_utils.c
    src/utils/array_utils.c
    src/garbage_collector/memory_alloc.c
)

add_executable(project ${SRC_FILES})
