cmake_minimum_required(VERSION 3.3)
project(oprendszerek1)

set(CMAKE_CC FLAGS "${CMAKE_CC_FLAGS}")

set(SOURCE_FILES main.c)
add_executable(oprendszerek1 ${SOURCE_FILES})