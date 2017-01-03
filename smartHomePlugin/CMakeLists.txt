cmake_minimum_required(VERSION 3.6)
include(ExternalProject)

project(smartHomePlugin)

set(CMAKE_CXX_STANDARD 11)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/lib)
ExternalProject_Add(
        json
        GIT_REPOSITORY git@github.com:nlohmann/json.git
        CMAKE_ARGS -DBuildTests=OFF -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)

# Find source files
file(GLOB SOURCES src/*.cpp src/*.hpp)

# Include header files
include_directories(include)

# Create shared library
add_library(${PROJECT_NAME} SHARED ${SOURCES})
add_dependencies(${PROJECT_NAME} json)
include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)

# Install library
install(TARGETS ${PROJECT_NAME} DESTINATION lib/${PROJECT_NAME})

# Install library headers
file(GLOB HEADERS include/*.hpp)
message("HI THERE")
message(${HEADERS})
install(FILES ${HEADERS} DESTINATION include/${PROJECT_NAME})

#set(SOURCE_FILES src/EventQueue.hpp src/SafeQueue.hpp include/Channel.hpp src/Channel.cpp include/Plugin.hpp src/Plugin.cpp)

#add_library(smartHomePlugin SHARED ${SOURCE_FILES})
#target_link_libraries(smartHomePlugin PUBLIC ${CMAKE_THREAD_LIBS_INIT})
#include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
#add_dependencies(smartHomePlugin json)