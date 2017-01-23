cmake_minimum_required(VERSION 3.6)

# Appends the cmake/modules path to MAKE_MODULE_PATH variable.
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

# Include necessary files
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
        GIT_TAG 81065748e315026017c633fca1bfc57cba5b246a
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        CMAKE_ARGS -DBuildTests=OFF -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
        UPDATE_DISCONNECTED 1
        ${OFFLINE_PARAM}
)

# Add subdirectories with targets
add_subdirectory(plugin)
add_subdirectory(server)
add_subdirectory(plugin_gpio)
add_subdirectory(plugin_knx)
add_subdirectory(exampleClient)