cmake_minimum_required(VERSION 3.6)
project(smp-WS)

ExternalProject_Add(
        uWS
        GIT_TAG patch-2
        GIT_REPOSITORY https://github.com/TheMegaTB/uWebSockets
        CMAKE_ARGS -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION} -DBUILD_STATIC_LIBS=OFF -DBUILD_SHARED_LIBS=ON -DBUILD_EXAMPLES=OFF
        UPDATE_DISCONNECTED 1
)

find_package(ZLIB REQUIRED)

if (NOT LIBUV_INCLUDE_DIR)
    find_path(LIBUV_INCLUDE_DIR uv.h)
endif ()
if (NOT LIBUV_LIBRARY)
    find_library(LIBUV_LIBRARY NAMES uv uv1)
endif ()


# Set the sources
set(SOURCE_FILES src/main.cpp)

# Add the shared library target
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# Add the jsoncpp project as a dependency
include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
add_dependencies(${PROJECT_NAME} json)

# Add the pistache REST server project as a dependency
include_directories(${EXTERNAL_INSTALL_LOCATION}/include/uWS)
target_include_directories(${PROJECT_NAME} PUBLIC ${EXTERNAL_INSTALL_LOCATION}/include/uWS)
add_dependencies(${PROJECT_NAME} uWS)

# Link against pistache, smp-plugin and thread libraries
target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARY} ${EXTERNAL_INSTALL_LOCATION}/lib64/libuWS.so smp-plugin ${CMAKE_THREAD_LIBS_INIT})