#!/bin/bash
if [[ "$1" == '--debug' ]]; then
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug; cmake --build build/debug
exit 0
else
echo "No option"
exit 1
fi
