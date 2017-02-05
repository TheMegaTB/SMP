cmake_minimum_required(VERSION 3.6)

# Appends the cmake/modules path to MAKE_MODULE_PATH variable.
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

# Include necessary files
include(ExternalProject)

# Set generic variables
project(SMP)
set(CMAKE_CXX_STANDARD 11)

# Enable the use of the global offset table
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Fetch the jsoncpp project
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/lib)
ExternalProject_Add(
        json
        GIT_TAG develop
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        CMAKE_ARGS -DBuildTests=OFF -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
        UPDATE_DISCONNECTED 1
)

# Add subdirectories with targets
add_subdirectory(plugin-library)
add_subdirectory(server)
add_subdirectory(plugins/gpio)
add_subdirectory(plugins/knx)
add_subdirectory(plugins/Websocket)
add_subdirectory(plugins/X11)
add_subdirectory(clients/c++)

# Add custom target to build all plugins
add_custom_target(Plugins DEPENDS smp-gpio smp-knx smp-WS smp-X11)