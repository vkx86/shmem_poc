cmake_minimum_required(VERSION 3.22)
project(sync_poc)

set(CMAKE_CXX_STANDARD 14)

add_executable(sync_poc main.cpp)
target_link_libraries (sync_poc rt pthread)