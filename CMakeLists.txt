cmake_minimum_required(VERSION 3.6)
include(ExternalProject)

project(smartHome)

set(CMAKE_CXX_STANDARD 11)
find_package(Threads)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/lib)
ExternalProject_Add(
        json
        GIT_REPOSITORY git@github.com:nlohmann/json.git
        CMAKE_ARGS -DBuildTests=OFF -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)

ExternalProject_Add(
        smartHomePlugin
        SOURCE_DIR ../smartHomePlugin
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)


set(SOURCE_FILES src/main.cpp src/InterruptHandle.hpp)

add_executable(smartHome ${SOURCE_FILES})
target_link_libraries(smartHome PUBLIC ${CMAKE_THREAD_LIBS_INIT})

include_directories(${EXTERNAL_INSTALL_LOCATION}/include/smartHomePlugin)
add_dependencies(smartHome smartHomePlugin)

include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
add_dependencies(smartHome json)