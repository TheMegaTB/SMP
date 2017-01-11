cmake_minimum_required(VERSION 3.6)
include(ExternalProject)

# Set generic variables
project(smartHome)
set(CMAKE_CXX_STANDARD 11)

# Enable the use of the global offset table
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Check if repository updates should be skipped
if($ENV{OFFLINE})
    set(OFFLINE_PARAM "UPDATE_COMMAND \"\"")
endif()

# Fetch the jsoncpp project
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/lib)
ExternalProject_Add(
        json
        GIT_REPOSITORY git@github.com:nlohmann/json.git
        CMAKE_ARGS -DBuildTests=OFF -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
        ${OFFLINE_PARAM}
)

# Add subdirectories with targets
add_subdirectory(plugin)
add_subdirectory(server)
add_subdirectory(plugin_gpio)
add_subdirectory(exampleClient)