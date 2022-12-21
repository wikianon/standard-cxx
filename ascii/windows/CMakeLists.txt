cmake_minimum_required(VERSION 3.6)
project(ASCII)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cxx ascii.cxx ascii.hxx getch.hxx getch.cxx)
add_executable(exercicio1 ${SOURCE_FILES}) 
