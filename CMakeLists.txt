cmake_minimum_required(VERSION 3.10)
project(libezw)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB SOURCE_FILES "${SOURCE_DIR}/*.c")

include_directories(include wavelib/header)
add_executable(main ${SOURCE_FILES})

target_link_libraries(main ${CMAKE_CURRENT_SOURCE_DIR}/libwavelib.a)
target_link_libraries(main m) # link math.h

