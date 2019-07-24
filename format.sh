#!/bin/sh

cd `dirname $0`

clang-format -i -style=Google ./**/*.h
clang-format -i -style=Google ./**/*.hpp
clang-format -i -style=Google ./**/*.cpp

