cmake_minimum_required(VERSION 3.23)
project(untitled12)


set(CMAKE_CXX_STANDARD 17)

add_subdirectory(command)

add_executable(untitled12 main.cpp)

target_link_libraries(untitled12 comm)
