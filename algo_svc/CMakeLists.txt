cmake_minimum_required(VERSION 3.22)
project(algo_svc)

set(CMAKE_CXX_STANDARD 14)

add_executable(algo_svc main.cpp)
target_link_libraries (algo_svc rt)