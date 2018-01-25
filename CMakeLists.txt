cmake_minimum_required(VERSION 3.8)
project(compiler)

set(CMAKE_CXX_STANDARD 98)

set(SOURCE_FILES main.cpp cmake-build-debug/symbol.h)
add_executable(compiler ${SOURCE_FILES} symbol0.h error.h grammer.h genmid.h optim.h mips.h)