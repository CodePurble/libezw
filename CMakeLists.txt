cmake_minimum_required(VERSION 3.10)
project(libezw)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(wavelib)

file(GLOB SOURCE_FILES "${SOURCE_DIR}/*.c")

include_directories(include)
add_executable(main ${SOURCE_FILES})

target_link_libraries(main PUBLIC wavelib m) # "m" to link math.h

