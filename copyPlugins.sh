mkdir -p cmake-build-debug/plugins
find cmake-build-debug -type f -name "libsmp-*.so" -exec rsync -a {} cmake-build-debug/plugins/ \;