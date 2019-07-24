#!/bin/sh

cd `dirname $0`

clang-format -i -style=Google ./**/*.h ./**/*.hpp ./**/*.cpp

