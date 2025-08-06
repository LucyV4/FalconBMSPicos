# CMakeLists.txt
cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Set some directories for files
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(FIRMWARE_DIR ${CMAKE_SOURCE_DIR}/firmware)

# Check if PICO_SDK_PATH is defined
if(NOT DEFINED ENV{PICO_SDK_PATH})
	message(FATAL_ERROR "PICO_SDK_PATH environment variable is not set.")
endif()

# Adds pico sdk
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

project(f16_builds C CXX ASM)

pico_sdk_init()

# Adds the picos
add_subdirectory(picos)