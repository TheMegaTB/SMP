#
# Make a version file containing the current version from git.
#
include(GetGitRevisionDescription)

# Get tags and dirty/clean status
git_describe(VERSION --tags --dirty=-dirty)

# Get hashes (long and short)
get_git_head_revision(A HASH)
string(SUBSTRING ${HASH} 0 7 SHORT_HASH)

set(VERSION ${VERSION}-${HASH})

message(${CMAKE_CURRENT_SOURCE_DIR})

# Parse the version information into pieces.
string(REGEX REPLACE "^v([0-9]+)\\..*" "\\1" VERSION_MAJOR "${VERSION}")
string(REGEX REPLACE "^v[0-9]+\\.([0-9]+).*" "\\1" VERSION_MINOR "${VERSION}")
string(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" VERSION_PATCH "${VERSION}")
string(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.[0-9]+(.*)" "\\1" VERSION_SHA1 "${VERSION}")
set(VERSION_SHORT "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${SHORT_HASH}")

configure_file(${CMAKE_SOURCE_DIR}/cmake/modules/version.cpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/version.cpp)
set(version_file "${CMAKE_CURRENT_BINARY_DIR}/version.cpp")