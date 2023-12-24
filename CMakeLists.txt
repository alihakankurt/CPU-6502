cmake_minimum_required(VERSION 3.20)

project(CPU-6502 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")

add_executable(${PROJECT_NAME} src/main.cc src/cpu.cc src/memory.cc)

target_include_directories(${PROJECT_NAME} PRIVATE include)
