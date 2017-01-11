
# Set the sources
file(GLOB SOURCES src/*.cpp src/*.hpp include/*.hpp include/*.cpp)

# Add the library target
add_library(smartHomePlugin ${SOURCES})

# Include the header files for other targets
include_directories(include)

# Add the jsoncpp project as a dependency
include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
add_dependencies(smartHomePlugin json)

# Include the header files with the target
target_include_directories(smartHomePlugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)