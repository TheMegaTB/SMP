mkdir -p cmake-build-debug/plugins
find cmake-build-debug -type f -name "libsh-*.so" -exec rsync -a {} cmake-build-debug/plugins/ \;