#!/bin/bash
if [[ "$1" == '--debug' ]]; then
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug; cmake --build build/debug
exit 0
elif [[ "$1" == '--release' ]]; then
cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release; cmake --build build/release
exit 0
else
echo "No option"
exit 1
fi
