cmake_minimum_required(VERSION 3.30)

project(DCGrid)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMPILE_FLAGS -Wall -Wextra -Werror -pedantic)

add_executable(main main.cpp)
target_compile_options(main PRIVATE ${COMPILE_FLAGS})

