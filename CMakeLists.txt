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

set(SOURCE_FILES src/main.cpp src/networking/udpsocket.hpp src/networking/udpsocket.cpp src/SafeQueue.hpp src/EventQueue.hpp src/InterruptHandle.hpp src/plugin_framework/Plugin.cpp src/plugin_framework/Plugin.hpp src/plugin_framework/Channel.hpp)

add_executable(smartHome ${SOURCE_FILES})
target_link_libraries(smartHome PUBLIC ${CMAKE_THREAD_LIBS_INIT})
include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
add_dependencies(smartHome json)