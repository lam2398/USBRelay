#ifndef _USB_CONFIG_H
#define _USB_CONFIG_H

// Defines the number of endpoints for this device. (Always add one for EP0). For two EPs, this should be 3.
#define ENDPOINTS 2

#define USB_PORT C  	// [A,C,D] GPIO Port to use with D+, D- and DPU
#define USB_PIN_DP 2  // [0-4] GPIO Number for USB D+ Pin
#define USB_PIN_DM 4  // [0-4] GPIO Number for USB D- Pin

#define RV003USB_DEBUG_TIMING 0
#define RV003USB_OPTIMIZE_FLASH 0
#define RV003USB_EVENT_DEBUGGING 0
#define RV003USB_HANDLE_IN_REQUEST 1
#define RV003USB_OTHER_CONTROL 1
#define RV003USB_HANDLE_USER_DATA 1
#define RV003USB_HID_FEATURES 1
#define DEVICE_ID "RELAY"
#ifndef __ASSEMBLER__

#include <tinyusb_hid.h>
#include <tusb_types.h>

#ifdef INSTANCE_DESCRIPTORS

static const uint8_t device_descriptor[] = {
	18,			// Length
	1,			// Type (Device)
	0x10, 0x01, // Spec
	0x0,		// Device Class
	0x0,		// Device Subclass
	0x0,		// Device Protocol  (000 = use config descriptor)
	0x08,		// Max packet size for EP0 (This has to be 8 because of the USB Low-Speed Standard)
	0xc0, 0x16, // ID Vendor
	0xdf, 0x05, // ID Product
	0x01, 0x00, // ID Rev
	1,			// Manufacturer string
	2,			// Product string
	0,			// Serial string
	1,			// Max number of configurations
};

static const uint8_t usbrelay_report_desc[] = {
	0x06, 0x00, 0xff, /* USAGE PAGE (Generic Desktop) */
	0x09, 0x01,		  /* USAGE (Vendor Usage 1) */
	0xa1, 0x01,		  /* COLLECTION (Application) */
	0x15, 0x00,		  /*   LOGICAL_MINIMUM (0) */
	0x26, 0xff, 0x00, /*   LOGICAL_MAXIMUM (255) */
	0x75, 0x08,		  /*   REPORT_SIZE (8) */
	0x95, 0x08,		  /*   REPORT_COUNT (8) */
	0x09, 0x00,		  /*   USAGE (Undefined) */
	0xb2, 0x02, 0x01, /*   FEATURE (Data, Var, Abs, Buf) */
	0xc0			  /* END_COLLECTION */
};

static const uint8_t config_descriptor[] = {
	// configuration descriptor, USB spec 9.6.3, page 264-266, Table 9-10
	9,						 // bLength;
	TUSB_DESC_CONFIGURATION, // bDescriptorType;  //2
	0x22, 0x00,				 // wTotalLength

	// 34, 0x00, //for just the one descriptor

	0x01, // bNumInterfaces (Normally 1)
	0x01, // bConfigurationValue
	0x00, // iConfiguration
	0x80, // bmAttributes (was 0xa0)
	0x64, // bMaxPower (200mA)

	// Joystick  (It is unusual that this would be here)
	9,					 // bLength
	TUSB_DESC_INTERFACE, // bDescriptorType   //0x04
	0,					 // bInterfaceNumber  = 1 instead of 0 -- well make it second.
	0,					 // bAlternateSetting
	1,					 // bNumEndpoints
	0x03,				 // bInterfaceClass (0x03 = HID)
	0x00,				 // bInterfaceSubClass
	HID_PROTOCOL_NONE,	 // bInterfaceProtocol (1 = Keyboard, 2 = Mouse)
	0,					 // iInterface

	9,			// bLength
	0x21,		// bDescriptorType (HID)
	0x01, 0x01, // bcd 1.1
	0x00,		// country code
	0x01,		// Num descriptors
	0x22,		// DescriptorType[0] (HID)
	sizeof(usbrelay_report_desc), 0x00,

	7,			// endpoint descriptor (For endpoint 1)
	0x05,		// Endpoint Descriptor (Must be 5)
	0x81,		// Endpoint Address
	0x03,		// Attributes
	0x08, 0x00, // Size (We aren't using it)
	20,			// Interval (We don't use it.)
};

#define STR_MANUFACTURER u"www.dcttech.com"
#define STR_PRODUCT u"USBRelay1"
#define STR_SERIAL u"RELAY"

struct usb_string_descriptor_struct
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wString[];
};
const static struct usb_string_descriptor_struct string0 __attribute__((section(".rodata"))) = {
	4,
	3,
	{0x0409}};
const static struct usb_string_descriptor_struct string1 __attribute__((section(".rodata"))) = {
	sizeof(STR_MANUFACTURER),
	3,
	STR_MANUFACTURER};
const static struct usb_string_descriptor_struct string2 __attribute__((section(".rodata"))) = {
	sizeof(STR_PRODUCT),
	3,
	STR_PRODUCT};
const static struct usb_string_descriptor_struct string3 __attribute__((section(".rodata"))) = {
	sizeof(STR_SERIAL),
	3,
	STR_SERIAL};

const static struct descriptor_list_struct
{
	uint32_t lIndexValue;
	const uint8_t *addr;
	uint8_t length;
} descriptor_list[] = {
	{0x00000100, device_descriptor, sizeof(device_descriptor)},
	{0x00000200, config_descriptor, sizeof(config_descriptor)},
	// interface number // 2200 for hid descriptors.
	{0x00002200, usbrelay_report_desc, sizeof(usbrelay_report_desc)},
	{0x00002100, config_descriptor + 18, 9}, // Not sure why, this seems to be useful for Windows + Android.

	{0x00000300, (const uint8_t *)&string0, 4},
	{0x04090301, (const uint8_t *)&string1, sizeof(STR_MANUFACTURER)},
	{0x04090302, (const uint8_t *)&string2, sizeof(STR_PRODUCT)},
	{0x04090303, (const uint8_t *)&string3, sizeof(STR_SERIAL)}};
#define DESCRIPTOR_LIST_ENTRIES ((sizeof(descriptor_list)) / (sizeof(struct descriptor_list_struct)))

#endif // INSTANCE_DESCRIPTORS

#endif

#endif
