cmake_minimum_required(VERSION 3.16)
project(NaviSearch VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Phase 1: Console Indexer
add_executable(indexer
    src/core/main.cpp
)

# JSON library for Phase 2 (Header-only)
# nlohmann/json.hpp should be in include/nlohmann/json.hpp

# Compiler-specific options
if(MSVC)
    target_compile_options(indexer PRIVATE /W4)
else()
    target_compile_options(indexer PRIVATE -Wall -Wextra -Wpedantic)
endif()
