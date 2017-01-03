find_package(Threads)

set(SOURCE_FILES src/main.cpp src/InterruptHandle.hpp)

add_executable(smartHome ${SOURCE_FILES})

# Link to thread and dyn-linker library
target_link_libraries(smartHome PUBLIC ${CMAKE_THREAD_LIBS_INIT} dl)

# Link to JSON library
include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
add_dependencies(smartHome json)

# Link to the plugin library
target_link_libraries(smartHome LINK_PUBLIC smartHomePlugin)