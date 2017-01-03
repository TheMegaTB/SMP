
file(GLOB SOURCES src/*.cpp src/*.hpp include/*.hpp include/*.cpp)
add_library(smartHomePlugin ${SOURCES})

include_directories(include)

include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
add_dependencies(smartHomePlugin json)

target_include_directories(smartHomePlugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)