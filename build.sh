#!/bin/bash -e

# Build the platform tool using CMake. The first parameter must
# be the out-of-source build directory.
# The script creates a directory for out-of-source build and then start
# the build. The out-of-source build directory is gitignored.
# All options are passed on to 'cmake ..' command.
# For example, if one wants to use the Ninja generator, one should type:
#     $ ./build -G Ninja

TARGET=platform
BUILD_SUBDIR=".build"
SUPER_DIR="$(dirname -- "$0")"
BUILD_DIR="$SUPER_DIR/$BUILD_SUBDIR"
GITIGNORE=".gitignore"

# create out-of-source build directory
if [ ! -d "$BUILD_DIR" ]; then
	mkdir -p "$BUILD_DIR"
fi

cd "$SUPER_DIR"
# create link
if [ ! -e "$TARGET" ] && [ ! -h "$TARGET" ]; then
	ln -s "$BUILD_SUBDIR/$TARGET" "$TARGET"
fi

cd "$BUILD_SUBDIR"
if ! cmake .. "$@"; then
	echo "CMake error."
	echo "Attempting to delete build directory and retry..."
	cd ..
	
	echo "$BUILD_SUBDIR"
	echo "$(pwd)"
	exit
	rm -rf "$BUILD_SUBDIR"
	mkdir -p "$BUILD_SUBDIR"
	cd "$BUILD_SUBDIR"
	cmake .. "$@"
fi

# gitignore the build directory 
echo "*" > "$GITIGNORE"

cmake --build . --target "$TARGET"
