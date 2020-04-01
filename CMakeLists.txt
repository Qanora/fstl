cmake_minimum_required(VERSION 3.0)
project(fstl)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -g -fsanitize=address -fno-omit-frame-pointer")
add_subdirectory(${PROJECT_SOURCE_DIR}/test)
