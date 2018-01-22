cmake_minimum_required(VERSION 3.8)
project(kilo)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        kilo.c
        Makefile
        README.md)

add_executable(kilo ${SOURCE_FILES})