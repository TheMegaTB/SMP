cmake_minimum_required(VERSION 3.6)
project(gpio)

add_library(gpio SHARED src/main.cpp)

include_directories(include)

#include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
#add_dependencies(gpio json)

target_include_directories(gpio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)