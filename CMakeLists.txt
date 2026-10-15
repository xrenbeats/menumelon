cmake_minimum_required(VERSION 3.25)

project(MenuMelon VERSION 1.0.0 LANGUAGES CXX)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()cmake_minimum_required(VERSION 3.25)

project(MenuMelon VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "GEODE_SDK must point to the Geode SDK directory.")
endif()

add_library(${PROJECT_NAME} SHARED Main.cpp)

add_subdirectory("$ENV{GEODE_SDK}" "${CMAKE_CURRENT_BINARY_DIR}/geode")
setup_geode_mod(${PROJECT_NAME})
