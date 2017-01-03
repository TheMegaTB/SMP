find_package(Threads)

set(SOURCE_FILES src/main.cpp src/InterruptHandle.hpp)

add_executable(smartHome ${SOURCE_FILES})
target_link_libraries(smartHome PUBLIC ${CMAKE_THREAD_LIBS_INIT})

include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
add_dependencies(smartHome json)

target_link_libraries(smartHome LINK_PUBLIC smartHomePlugin)