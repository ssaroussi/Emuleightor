cmake_minimum_required(VERSION 3.5)

project(Emuleightor)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lSDL2")

set(SOURCE_FILES main.cpp CPU.cpp Graphics.cpp Graphics.h)

add_executable(Emulator ${SOURCE_FILES})