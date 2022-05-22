cmake_minimum_required(VERSION 3.22)
project(video_svc)

set(CMAKE_CXX_STANDARD 14)

#https://caiorss.github.io/C-Cpp-Notes/boost-libraries.html#orgb7c3fb4
find_package (PkgConfig REQUIRED)

#set(Boost_USE_STATIC_LIBS ON)
#find_package(Boost REQUIRED system)

add_executable(video_svc main.cpp ShmDataReceiver.cpp ShmDataReceiver.h)
#target_link_libraries (video_svc Boost::system rt)
target_link_libraries (video_svc rt pthread)
