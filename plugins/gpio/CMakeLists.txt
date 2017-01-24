cmake_minimum_required(VERSION 3.6)
project(sh-gpio)

# Set the sources
set(SOURCE_FILES src/main.cpp)

# Add the shared library target
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# Add the jsoncpp project as a dependency
include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
add_dependencies(${PROJECT_NAME} json)

# Link to the plugin library
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC smartHomePlugin)