#CMakeLists.txt for Matrix

cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

add_executable(Matrix
  main.cpp
  matrix.cpp)

#set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -lSDL2 -lSDL2_ttf -std=c++17")

# Tell CMake to create a compilation database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Matrix)
