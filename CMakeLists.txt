cmake_minimum_required(VERSION 3.6)
project(PotatoShuffle)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -ansi")

set(SOURCE_FILES main.c deck.c deck.h utf8.h utils.c utils.h)
add_executable(shuffle ${SOURCE_FILES})