cmake_minimum_required(VERSION 2.8) 

set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -fopenmp -Wno-unused-local-typedefs -pthread"
    CACHE STRING "Flags used by the compiler during all build types.")

project(master_system)

include_directories("~/src/CmdParser/" "defs")

link_directories("~/src/CmdParser/build/")

add_library(Console masterSystem.cpp)

add_executable(MasterSystem main.cpp)

target_link_libraries(MasterSystem Console CmdParser)
