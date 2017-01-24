cmake_minimum_required(VERSION 3.6)
project(sh-knx)

# Download and build knxd libraries
ExternalProject_Add(
        knxd
        GIT_TAG
        GIT_REPOSITORY https://github.com/knxd/knxd
        UPDATE_DISCONNECTED 1
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND <SOURCE_DIR>/bootstrap.sh && <SOURCE_DIR>/configure --disable-systemd --with-pic --prefix=${EXTERNAL_INSTALL_LOCATION} --sysconfdir=<TMP_DIR> --libdir=${EXTERNAL_INSTALL_LOCATION} --libexecdir=<TMP_DIR>
)

# Set the sources
set(SOURCE_FILES src/main.cpp)

# Add the shared library target
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# Add the jsoncpp project as a dependency
include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
add_dependencies(${PROJECT_NAME} json)

# Add knxd (more specifically eibclient) as a dependency
include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
add_dependencies(${PROJECT_NAME} knxd)

# Link against the eibdclient and sh-plugin library
target_link_libraries(${PROJECT_NAME} ${EXTERNAL_INSTALL_LOCATION}/libeibclient.a smartHomePlugin)