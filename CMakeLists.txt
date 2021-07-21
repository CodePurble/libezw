cmake_minimum_required(VERSION 3.10)
project(libezw)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)
add_executable(main
    # ./src/ezw.c
    ./src/main.c
    ./src/utils.c
    # ./src/sbtree.c
    # ./src/smap.c
    # ./src/node.c
    # ./src/queue.c
    # ./src/stack.c
    )

target_link_libraries(main PUBLIC gsl gslcblas m) # "m" to link math.h

