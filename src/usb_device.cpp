#include "usb_device.h"
#include <cstring>
#include "pico/unique_id.h"
#include "tusb.h"
#include "hid_descriptors.h"

static UsbDevice* usbDevice = nullptr;

void usbDevice_setInstance(UsbDevice* dev) {
	usbDevice = dev;
}

// Static HID report descriptor example
const uint8_t UsbDevice::desc_hid_report[] = {
	FLIGHT_SIM_REPORT_DESC(HID_REPORT_ID(1)),
};

// Static configuration descriptor - same as original but static
const uint8_t UsbDevice::desc_configuration[] = {
	TUD_CONFIG_DESCRIPTOR(1, 3, 0, TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN + TUD_HID_DESC_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
	TUD_HID_DESCRIPTOR(2, 0, HID_ITF_PROTOCOL_NONE, sizeof(desc_hid_report), 0x81, CFG_TUD_HID_BUFSIZE, 1),
	TUD_CDC_DESCRIPTOR(0, 4, 0x83, 8, 0x02, 0x84, 64)
};

// Default strings fallback
const char* const UsbDevice::string_desc_arr_default[5] = {
	(const char[]){0x09, 0x04},		 // Supported language: English (0x0409)
	"Raspberry Pi",					 // Manufacturer
	"F16 EWPI",						// Product
	nullptr,						  // Serial to be generated dynamically
	"F16 EWPI CDC"					// CDC Interface string
};

UsbDevice::UsbDevice(const char* name)  : pico_name(name) {

	// Setup device descriptor
	desc_device = {
		sizeof(tusb_desc_device_t), TUSB_DESC_DEVICE,
		USB_BCD,
		TUSB_CLASS_MISC,
		MISC_SUBCLASS_COMMON,
		MISC_PROTOCOL_IAD,
		CFG_TUD_ENDPOINT0_SIZE,
		USB_VID,
		USB_PID,
		0x0100,
		1,  // iManufacturer index in string desc
		2,  // iProduct index
		3,  // iSerialNumber index
		1   // number of configurations
	};

	static char product_name_buf[32];
	static char cdc_name_buf[40];

	strcpy(product_name_buf, "F16 ");
	strcat(product_name_buf, pico_name);

	strcpy(cdc_name_buf, product_name_buf);
	strcat(cdc_name_buf, " CDC");

	// Initialize string descriptors array
	string_desc_arr[0] = string_desc_arr_default[0];
	string_desc_arr[1] = "Raspberry Pi";
	string_desc_arr[2] = product_name_buf;
	string_desc_arr[3] = serial;  // will be updated dynamically
	string_desc_arr[4] = cdc_name_buf;
}

// Return pointer to device descriptor
const uint8_t* UsbDevice::getDeviceDescriptor() const {
	return reinterpret_cast<const uint8_t*>(&desc_device);
}

// Return pointer to configuration descriptor (only one config supported)
const uint8_t* UsbDevice::getConfigurationDescriptor(uint8_t /*index*/) const {
	return desc_configuration;
}

// Return pointer to HID report descriptor
const uint8_t* UsbDevice::getHidReportDescriptor(uint8_t /*instance*/) const {
	return desc_hid_report;
}

// Return string descriptor in UTF-16 format
const uint16_t* UsbDevice::getStringDescriptor(uint8_t index, uint16_t /*langid*/) {
	if (index == 0) {
		memcpy(&_desc_str[1], string_desc_arr[0], 2);
		_desc_str[0] = (TUSB_DESC_STRING << 8) | (2 + 2);
		return _desc_str;
	}

	if (index == 3) {
		pico_get_unique_board_id_string(serial, sizeof(serial));
		string_desc_arr[3] = serial;
	}

	if (index >= 5 || string_desc_arr[index] == nullptr) {
		return nullptr;
	}

	const char* str = string_desc_arr[index];
	uint8_t chr_count = strlen(str);
	if (chr_count > 31) chr_count = 31;

	for (uint8_t i = 0; i < chr_count; i++) {
		_desc_str[1 + i] = str[i];
	}

	_desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * chr_count + 2);
	return _desc_str;
}

extern "C" {
	const uint8_t* tud_descriptor_device_cb(void) {
		return usbDevice->getDeviceDescriptor();
	}

	const uint8_t* tud_descriptor_configuration_cb(uint8_t index) {
		return usbDevice->getConfigurationDescriptor(index);
	}

	const uint8_t* tud_hid_descriptor_report_cb(uint8_t instance) {
		return usbDevice->getHidReportDescriptor(instance);
	}

	const uint16_t* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
		return usbDevice->getStringDescriptor(index, langid);
	}
}