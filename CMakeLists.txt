# CMakeLists.txt for Matrix project

cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

add_executable(Matrix
  main.cpp
  matrix.cpp
  letter.cpp
  font.cpp
  randomizer.cpp)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -lSDL2 -lSDL2_ttf")

# Tell CMake to create a compilation database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Matrix)
