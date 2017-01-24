project(smp-client)

set(SOURCE_FILES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
add_dependencies(${PROJECT_NAME} json)

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC smp-plugin)