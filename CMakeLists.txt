cmake_minimum_required(VERSION 3.6)
include(ExternalProject)

project(smartHome)
set(CMAKE_CXX_STANDARD 11)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/lib)
ExternalProject_Add(
        json
        GIT_REPOSITORY git@github.com:nlohmann/json.git
        CMAKE_ARGS -DBuildTests=OFF -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)

add_subdirectory(plugin)
add_subdirectory(server)
add_subdirectory(exampleClient)