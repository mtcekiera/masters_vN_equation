cmake_minimum_required(VERSION 3.16)

project(density_matrix LANGUAGES CXX)

add_executable(density_matrix
    src/main.cpp
    src/Matrix.cpp
    src/State.cpp
    src/twoState.cpp
    src/tests.cpp
)

target_include_directories(density_matrix PRIVATE headers)
target_compile_features(density_matrix PRIVATE cxx_std_17)