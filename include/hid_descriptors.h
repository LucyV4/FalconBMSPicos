#include "common/tusb_common.h"
#include "device/usbd.h"

#define FLIGHT_SIM_REPORT_DESC(...)								\
	HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),						\
	HID_USAGE(HID_USAGE_DESKTOP_MULTI_AXIS_CONTROLLER),			\
	HID_COLLECTION(HID_COLLECTION_APPLICATION),					\
		__VA_ARGS__ 											\
		/* Axes (Throttle, X, Y, Z, Rz) */						\
		HID_USAGE(HID_USAGE_DESKTOP_X),							\
		HID_USAGE(HID_USAGE_DESKTOP_Y),							\
		HID_USAGE(HID_USAGE_DESKTOP_Z),							\
		HID_USAGE(HID_USAGE_DESKTOP_RX),						\
		HID_USAGE(HID_USAGE_DESKTOP_RY),						\
		HID_USAGE(HID_USAGE_DESKTOP_RZ),						\
		HID_LOGICAL_MIN(0x00),									\
		HID_LOGICAL_MAX_N(0xFFFF, 2),							\
		HID_REPORT_COUNT(6),									\
		HID_REPORT_SIZE(16),									\
		HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),		\
																\
		/* 128 Buttons */										\
		HID_USAGE_PAGE(HID_USAGE_PAGE_BUTTON),					\
		HID_USAGE_MIN(1),										\
		HID_USAGE_MAX(128),										\
		HID_LOGICAL_MIN(0),										\
		HID_LOGICAL_MAX(1),										\
		HID_REPORT_COUNT(128),									\
		HID_REPORT_SIZE(1),										\
		HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),		\
																\
		/* Vendor-defined output (e.g. RGB or LEDs) */			\
		HID_USAGE_PAGE_N(0xFF00, 2),							\
		HID_USAGE(0x01),										\
		HID_LOGICAL_MIN(0),										\
		HID_LOGICAL_MAX(255),									\
		HID_REPORT_COUNT(25),									\
		HID_REPORT_SIZE(8),										\
		HID_OUTPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),		\
	HID_COLLECTION_END

#define REBOOT_CONTROL_REPORT_DESC(...)							\
	HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),						\
	HID_USAGE(HID_USAGE_DESKTOP_SYSTEM_CONTROL),				\
	HID_COLLECTION(HID_COLLECTION_APPLICATION),					\
		__VA_ARGS__												\
		HID_USAGE(HID_USAGE_DESKTOP_SYSTEM_POWER_DOWN),			\
		HID_LOGICAL_MIN(0),										\
		HID_LOGICAL_MAX(255),									\
		HID_REPORT_SIZE(8),										\
		HID_REPORT_COUNT(1),									\
		HID_OUTPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),		\
	HID_COLLECTION_END
