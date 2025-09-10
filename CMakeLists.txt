cmake_minimum_required(VERSION 3.16)
project(NaviSearch VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Phase 1: Console Indexer (Simple Threading)
add_executable(indexer
    src/core/main.cpp
)

# Phase 2: Loader Program with Cache Management
add_executable(loader
    src/loader/main.cpp
)

# JSON library for Phase 2 (Header-only)
# nlohmann/json.hpp should be in include/nlohmann/json.hpp

# Compiler-specific options
if(MSVC)
    target_compile_options(indexer PRIVATE /W4)
    target_compile_options(loader PRIVATE /W4)
else()
    target_compile_options(indexer PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(loader PRIVATE -Wall -Wextra -Wpedantic)
endif()
