project(smp-plugin)

# Set the sources
file(GLOB SOURCES src/*.cpp src/*.hpp include/*.hpp include/*.cpp)

# Add the library target
add_library(${PROJECT_NAME} ${SOURCES})

# Include the header files for other targets
include_directories(include)

# Add the jsoncpp project as a dependency
include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
add_dependencies(${PROJECT_NAME} json)

# Add the CLog project as a dependency
include_directories(${EXTERNAL_INSTALL_LOCATION}/include/CLog)
add_dependencies(${PROJECT_NAME} CLog)

# Include the header files with the target
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Find and link to boost
find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif ()
set(Boost_USE_STATIC_LIBS ON) # enable dynamic linking
set(Boost_USE_MULTITHREAD OFF)  # enable multithreading
find_package(Boost COMPONENTS REQUIRED system)

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${EXTERNAL_INSTALL_LOCATION}/lib/CLog/libCLog.a)