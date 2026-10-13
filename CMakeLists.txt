cmake_minimum_required(VERSION 3.25)

project(MenuMelon VERSION 1.0.0 LANGUAGES CXX)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()

find_package(Geode REQUIRED)

add_geode_mod(
    MenuMelon
    VERSION 1.0.0
    CONTAINS_GEODE_FILES
    SOURCES src/Main.cpp
)
