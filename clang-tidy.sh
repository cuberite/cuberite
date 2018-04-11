#!/bin/sh

cmake --target Cuberite -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .
run-clang-tidy.py -header-filter ".+/cuberite/src/\w+/.+" -export-fixes "tidy-fixes.yaml" ".+/cuberite/src/\w+/.+"
