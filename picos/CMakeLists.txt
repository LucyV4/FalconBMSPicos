# CMakeLists.txt (picos)
set(SHARED_SOURCES
	${SOURCE_DIR}/f16pico.cpp
	${SOURCE_DIR}/f16data.cpp
	${SOURCE_DIR}/usb_device.cpp
	${SOURCE_DIR}/usb_callbacks.cpp
)

set(SHARED_INCLUDE_DIRS
	${INCLUDE_DIR}
	${INCLUDE_DIR}/modules
)

set(SHARED_LIBS
	pico_stdlib
	hardware_adc
	hardware_pwm
	hardware_uart
	tinyusb_device
	tinyusb_board
	pico_unique_id
)

add_subdirectory("ewpi")
add_subdirectory("ewmu")