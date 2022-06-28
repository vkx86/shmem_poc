cmake_minimum_required(VERSION 3.22)
project(algo_svc)

set(CMAKE_CXX_STANDARD 14)

include_directories(../common)

add_executable(algo_svc main.cpp ../common/ShmDataSender.cpp ../common/ShmDataSender.h)
target_link_libraries (algo_svc rt pthread)