cmake_minimum_required (VERSION 2.8)
include_directories(include)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14 -O0 -g3 -Wall -Wextra -Werror --pedantic -lpthread")
add_executable(SerialMandelbrot main.cpp) //add_executable(<File created by CMake> [file to be executed])
find_package(Threads)
target_link_libraries (SerialMandelbrot ${CMAKE_THREAD_LIBS_INIT}) //target_link_libraries(<File created by CMake... same as above> [file to be executed])