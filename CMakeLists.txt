cmake_minimum_required(VERSION 3.6)
project(smartHome)

set(CMAKE_CXX_STANDARD 11)
find_package(Threads)


set(SOURCE_FILES src/main.cpp src/networking/udpsocket.hpp src/networking/udpsocket.cpp)

add_executable(smartHome ${SOURCE_FILES})
target_link_libraries(smartHome PUBLIC ${CMAKE_THREAD_LIBS_INIT})