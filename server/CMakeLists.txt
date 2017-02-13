find_package(Threads)

project(smp-server)

include(GitVersion)

# Set the sources
set(SOURCE_FILES src/main.cpp src/InterruptHandle.hpp src/PluginLoader.cpp src/PluginLoader.hpp)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${version_file})

# Link to thread and dyn-linker library
target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_THREAD_LIBS_INIT} dl)

# Link to JSON library
include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
add_dependencies(${PROJECT_NAME} json)

# Add the CLog project as a dependency
include_directories(${EXTERNAL_INSTALL_LOCATION}/include/CLog)
add_dependencies(${PROJECT_NAME} CLog)

# Link to the plugin library
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC smp-plugin ${EXTERNAL_INSTALL_LOCATION}/lib/CLog/libCLog.a)