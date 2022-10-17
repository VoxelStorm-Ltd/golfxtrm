#!/bin/bash

target="$1"
if [ -z "$target" ]; then target="client"; fi

if [ "$target" = "clean" ]; then
  echo "Cleaning build directory..."
  rm -r build
  target="client"
fi

procs=$(nproc)
./generate_git_version.sh

if [ ! -d "build" ]; then
  echo "Creating build directory..."
  mkdir "build"
fi
cd "build"

if [ ! -f "Makefile" ] || [ "../CMakeLists.txt" -nt "Makefile" ]; then
  echo "Running CMAKE to generate Makefile..."
  emcmake cmake .. || exit 1
fi

echo "Running make..."
#emmake make -j"$procs" VERBOSE=1 "$target" || exit 1
emmake make -j"$procs" "$target" || exit 1

cd ..
echo "Done."
