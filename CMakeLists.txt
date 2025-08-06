# CMakeLists.txt
cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if(NOT DEFINED ENV{PICO_SDK_PATH})
	message(FATAL_ERROR "PICO_SDK_PATH environment variable is not set.")
endif()

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(FIRMWARE_DIR ${CMAKE_SOURCE_DIR}/firmware)

set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
set(PICO_BOARD pico)

include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

project(f16_builds C CXX ASM)

pico_sdk_init()

# Include whichever target you want
add_subdirectory(picos)