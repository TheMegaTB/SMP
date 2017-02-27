cmake_minimum_required(VERSION 3.6)
project(smp-X11)

# Set the sources
set(SOURCE_FILES src/xhklib.hpp src/xhklib.cpp src/main.cpp)

# Add the shared library target
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# Add the jsoncpp project as a dependency
include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
add_dependencies(${PROJECT_NAME} json)

# Add the CLog project as a dependency
include_directories(${EXTERNAL_INSTALL_LOCATION}/include/CLog)
add_dependencies(${PROJECT_NAME} CLog)

# Add X11 as a dependency
find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})

# Link to the plugin and X11 library
target_link_libraries(${PROJECT_NAME} smp-plugin ${X11_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${EXTERNAL_INSTALL_LOCATION}/lib/CLog/libCLog.a)