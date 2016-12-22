cmake_minimum_required(VERSION 3.6)
project(smartHome)

set(CMAKE_CXX_STANDARD 11)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#include_directories(${PROJECT_SOURCE_DIR}/lib/libsocket/headers)
#add_subdirectory(${PROJECT_SOURCE_DIR}/lib/libsocket)

set(SOURCE_FILES src/main.cpp src/networking/networking.cpp src/networking/networking.hpp)
#set(SERVER_SOURCE_FILES src/server.cpp)

#add_executable(smartHomeServer ${SERVER_SOURCE_FILES})
#target_link_libraries(smartHomeServer socket++)
add_executable(smartHome ${SOURCE_FILES})
#target_link_libraries(smartHome socket++)