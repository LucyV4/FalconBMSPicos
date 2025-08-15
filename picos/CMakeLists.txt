# CMakeLists.txt (picos)

# Sets common sources 
set(SHARED_SOURCES
	${SOURCE_DIR}/pico_device.cpp
	${SOURCE_DIR}/pico_data.cpp
	${SOURCE_DIR}/usb_device.cpp
	${SOURCE_DIR}/usb_callbacks.cpp
	${SOURCE_DIR}/output_module.cpp
	${SOURCE_DIR}/input_module.cpp
	${SOURCE_DIR}/uart_module.cpp
)

# Sets common directories
set(SHARED_INCLUDE_DIRS
	${INCLUDE_DIR}
	${INCLUDE_DIR}/modules
)

# Sets common libraries
set(SHARED_LIBS
	pico_stdlib
	hardware_adc
	hardware_pwm
	hardware_uart
	tinyusb_device
	tinyusb_board
	pico_unique_id
)

# DEFINE PICOS HERE
add_subdirectory("ewpi")
add_subdirectory("ewmu")
add_subdirectory("example")