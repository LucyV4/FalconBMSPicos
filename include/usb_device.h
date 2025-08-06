#ifndef USB_DEVICE_H
#define USB_DEVICE_H

#include <cstdint>

class UsbDevice {
public:
	UsbDevice(const char* name = "TEST");

	// Callbacks for TinyUSB stack
	const uint8_t* getDeviceDescriptor() const;
	const uint8_t* getConfigurationDescriptor(uint8_t index) const;
	const uint8_t* getHidReportDescriptor(uint8_t instance) const;
	const uint16_t* getStringDescriptor(uint8_t index, uint16_t langid);

private:
	void updateSerialString();

	// Descriptor data members
	static constexpr uint16_t USB_VID = 0xCafe;
	static constexpr uint16_t USB_BCD = 0x0200;

	// USB PID calculated as before
	static constexpr uint16_t USB_PID = (0x4000 | (1 << 0) /*CDC*/ | (1 << 2) /*HID*/);

	// Device descriptor
	struct DeviceDescriptor {
		uint8_t bLength;
		uint8_t bDescriptorType;
		uint16_t bcdUSB;
		uint8_t bDeviceClass;
		uint8_t bDeviceSubClass;
		uint8_t bDeviceProtocol;
		uint8_t bMaxPacketSize0;
		uint16_t idVendor;
		uint16_t idProduct;
		uint16_t bcdDevice;
		uint8_t iManufacturer;
		uint8_t iProduct;
		uint8_t iSerialNumber;
		uint8_t bNumConfigurations;
	} __attribute__((packed));

	DeviceDescriptor desc_device;

	// HID report descriptor - simplified, can be updated for your real one
	static const uint8_t desc_hid_report[];

	// Configuration descriptor buffer
	static const uint8_t desc_configuration[];

	// String descriptors
	static const char* const string_desc_arr_default[5];
	const char* string_desc_arr[5];

	// Serial number string buffer (UTF-8)
	char serial[2 * 8 + 1];  // assuming 8 bytes unique id, 2 hex chars per byte + null

	// UTF-16 string buffer for descriptor responses
	uint16_t _desc_str[33];

	// Stored manufacturer/product/CDC names
	const char* pico_name;
};

void usbDevice_setInstance(UsbDevice* dev);

#endif // USB_DEVICE_H