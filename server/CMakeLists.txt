find_package(Threads)

project(sh-server)

include(GitVersion)

# Set the sources
set(SOURCE_FILES src/main.cpp src/InterruptHandle.hpp src/PluginLoader.cpp src/PluginLoader.hpp ../plugin-library/src/Logger.cpp ../plugin-library/include/Logger.hpp)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${version_file})

# Link to thread and dyn-linker library
target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_THREAD_LIBS_INIT} dl)

# Link to JSON library
include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
add_dependencies(${PROJECT_NAME} json)

# Link to the plugin library
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC smartHomePlugin)