set(PICO_NAME "ewmu")
set(MAIN_SRC "${PICO_NAME}.cpp")

add_executable(${PICO_NAME} ${MAIN_SRC} ${SHARED_SOURCES})
target_include_directories(${PICO_NAME} PRIVATE ${SHARED_INCLUDE_DIRS})
target_link_libraries(${PICO_NAME} PRIVATE ${SHARED_LIBS})

# for each target, after add_executable(...)
set_target_properties(${PICO_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${FIRMWARE_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${FIRMWARE_DIR}
)

pico_add_extra_outputs(${PICO_NAME})