cmake_minimum_required(VERSION 3.6)
project(smp-scenes)

# Set the sources
set(SOURCE_FILES src/main.cpp)

# Add the shared library target
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# Add the jsoncpp project as a dependency
include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
add_dependencies(${PROJECT_NAME} json)

# Add the CLog project as a dependency
include_directories(${EXTERNAL_INSTALL_LOCATION}/include/CLog)
add_dependencies(${PROJECT_NAME} CLog)

# Link to the plugin library
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC smp-plugin ${EXTERNAL_INSTALL_LOCATION}/lib/CLog/libCLog.a)