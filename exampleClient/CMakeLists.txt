set(SOURCE_FILES main.cpp)

add_executable(smartHomeClient ${SOURCE_FILES})

include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
add_dependencies(smartHomeClient json)

target_link_libraries(smartHomeClient LINK_PUBLIC smartHomePlugin)