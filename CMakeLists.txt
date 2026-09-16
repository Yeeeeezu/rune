cmake_minimum_required(VERSION 3.16)
project(rune CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(rune src/main.cpp)
target_include_directories(rune PRIVATE src)

install(TARGETS rune DESTINATION bin)
