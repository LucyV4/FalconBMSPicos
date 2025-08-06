# CMakeLists.txt (picos/ewpi)
set(PICO_NAME "ewmu")

# Adds the source and shared sources
add_executable(${PICO_NAME} "${PICO_NAME}.cpp" ${SHARED_SOURCES})

# Adds the shared directories and shared libraries
target_include_directories(${PICO_NAME} PRIVATE ${SHARED_INCLUDE_DIRS})
target_link_libraries(${PICO_NAME} PRIVATE ${SHARED_LIBS})

# Set UF2 and ELF output directory
set_target_properties(${PICO_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${FIRMWARE_DIR}
)

# Required
pico_add_extra_outputs(${PICO_NAME})