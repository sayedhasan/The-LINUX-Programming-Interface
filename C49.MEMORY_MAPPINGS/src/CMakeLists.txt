cmake_minimum_required(VERSION 3.22)

# Set the project
project(chapter_49)

# Specify the CPP standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable
add_executable(prog1 prog1.cpp)

# Optional flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Og -g -O0 -fno-inline")
