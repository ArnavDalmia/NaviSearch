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

# For future: add nlohmann/json when we're ready
# find_package(nlohmann_json 3.2.0 REQUIRED)
# target_link_libraries(indexer nlohmann_json::nlohmann_json)

# Compiler-specific options
if(MSVC)
    target_compile_options(indexer PRIVATE /W4)
else()
    target_compile_options(indexer PRIVATE -Wall -Wextra -Wpedantic)
endif()
