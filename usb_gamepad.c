#define USB_GAMEPAD_PRIVATE_INCLUDE

#include "usb_gamepad.h"

/**************************************************************************
 *
 *  Configurable Options
 *
 **************************************************************************/

// You can change these to give your code its own name.
//#define STR_MANUFACTURER	L"Slashdev"
//#define STR_PRODUCT		L"PS3 Gamepad"
#define STR_MANUFACTURER	L"32teeth"
#define STR_PRODUCT		L"Sparky Jr. Arcade Stick"

/*
	input mapping to ATMEGA32U4
	
	PC	PS3	Port
	---------------------------
	U	U	D2
	D	D	D3
	L	L	D1
	R	R	D0
	1	O	B1
	2	X	B0
	3	[]	B3
	4	/\	B2
	5	L1	B6
	6	R1	B4
	7	L2	B7
	8	R2	B5
	9	Se	D5
	10	St	D4
	11	
	12	
	13	PS	D7
*/


// Mac OS-X and Linux automatically load the correct drivers.  On
// Windows, even though the driver is supplied by Microsoft, an
// INF file is needed to load the driver.  These numbers need to
// match the INF file.

#define VENDOR_ID		0x2341
#define PRODUCT_ID		0x0034

/*
#define VENDOR_ID		0x10C4
#define PRODUCT_ID		0x82C0
*/
// USB devices are supposed to implment a halt feature, which is
// rarely (if ever) used.  If you comment this line out, the halt
// code will be removed, saving 102 bytes of space (gcc 4.3.0).
// This is not strictly USB compliant, but works with all major
// operating systems.
//#define SUPPORT_ENDPOINT_HALT



/**************************************************************************
 *
 *  Endpoint Buffer Configuration
 *
 **************************************************************************/

#define ENDPOINT0_SIZE	64

#define GAMEPAD_INTERFACE	0
#define GAMEPAD_ENDPOINT	1
#define GAMEPAD_SIZE		64
#define GAMEPAD_BUFFER	EP_DOUBLE_BUFFER

static const uint8_t PROGMEM endpoint_config_table[] = {
	1, EP_TYPE_INTERRUPT_IN,  EP_SIZE(GAMEPAD_SIZE) | GAMEPAD_BUFFER,
	0,
	0,
	0
};


/**************************************************************************
 *
 *  Descriptor Data
 *
 **************************************************************************/

// Descriptors are the data that your computer reads when it auto-detects
// this USB device (called "enumeration" in USB lingo).  The most commonly
// changed items are editable at the top of this file.  Changing things
// in here should only be done by those who've read chapter 9 of the USB
// spec and relevant portions of any USB class specifications!


static uint8_t PROGMEM device_descriptor[] = {
	18,					// bLength
	1,					// bDescriptorType
	0x10, 0x01,				// bcdUSB
	0,					// bDeviceClass
	0,					// bDeviceSubClass
	0,					// bDeviceProtocol
	ENDPOINT0_SIZE,				// bMaxPacketSize0
	LSB(VENDOR_ID), MSB(VENDOR_ID),		// idVendor
	LSB(PRODUCT_ID), MSB(PRODUCT_ID),	// idProduct
	0x00, 0x01,				// bcdDevice
	1,					// iManufacturer
	2,					// iProduct
	0,					// iSerialNumber
	1					// bNumConfigurations
};

static uint8_t PROGMEM gamepad_hid_report_desc[] = {
	0x05, 0x01,        // USAGE_PAGE (Generic Desktop)
	0x09, 0x05,        // USAGE (Gamepad)
	0xa1, 0x01,        // COLLECTION (Application)
	0x15, 0x00,        //   LOGICAL_MINIMUM (0)
	0x25, 0x01,        //   LOGICAL_MAXIMUM (1)
	0x35, 0x00,        //   PHYSICAL_MINIMUM (0)
	0x45, 0x01,        //   PHYSICAL_MAXIMUM (1)
	0x75, 0x01,        //   REPORT_SIZE (1)
	0x95, 0x0d,        //   REPORT_COUNT (13)
	0x05, 0x09,        //   USAGE_PAGE (Button)
	0x19, 0x01,        //   USAGE_MINIMUM (Button 1)
	0x29, 0x0d,        //   USAGE_MAXIMUM (Button 13)
	/*
		 0:0x00  1:0x01  2:0x02  3:0x03  4:0x04  5:0x05  6:0x06  7:0x07  8:0x08 9:0x09 
		10:0x0a 11:0x0b 12:0x0c 13:0x0d 14:0x0e 15:0x0f 16:0x10 17:0x11 18:0x12 19:0x13 
		20:0x14 21:0x15 22:0x16 23:0x17 24:0x18 25:0x19 26:0x1a 27:0x1b 28:0x1c 29:0x1d 
		30:0x1e 31:0x1f 32:0x20 33:0x21 34:0x22 35:0x23 36:0x24 37:0x25 38:0x26 39:0x27 
		40:0x28 41:0x29 42:0x2a 43:0x2b 44:0x2c 45:0x2d 46:0x2e 47:0x2f 48:0x30 49:0x31 
	*/
	0x81, 0x02,        //   INPUT (Data,Var,Abs)
	0x95, 0x03,        //   REPORT_COUNT (3)
	0x81, 0x01,        //   INPUT (Cnst,Ary,Abs)
	0x05, 0x01,        //   USAGE_PAGE (Generic Desktop)
	0x25, 0x07,        //   LOGICAL_MAXIMUM (7)
	0x46, 0x3b, 0x01,  //   PHYSICAL_MAXIMUM (315)
	0x75, 0x04,        //   REPORT_SIZE (4)
	0x95, 0x01,        //   REPORT_COUNT (1)
	0x65, 0x14,        //   UNIT (Eng Rot:Angular Pos)
	0x09, 0x39,        //   USAGE (Hat switch)
	0x81, 0x42,        //   INPUT (Data,Var,Abs,Null)
	0x65, 0x00,        //   UNIT (None)
	0x95, 0x01,        //   REPORT_COUNT (1)
	0x81, 0x01,        //   INPUT (Cnst,Ary,Abs)
	0x26, 0xff, 0x00,  //   LOGICAL_MAXIMUM (255)
	0x46, 0xff, 0x00,  //   PHYSICAL_MAXIMUM (255)
	0x09, 0x30,        //   USAGE (X)
	0x09, 0x31,        //   USAGE (Y)
	0x09, 0x32,        //   USAGE (Z)
	0x09, 0x35,        //   USAGE (Rz)
	0x75, 0x08,        //   REPORT_SIZE (8)
	0x95, 0x04,        //   REPORT_COUNT (4)
	0x81, 0x02,        //   INPUT (Data,Var,Abs)
	0x06, 0x00, 0xff,  //   USAGE_PAGE (Vendor Specific)
	0x09, 0x20,        //   Unknown
	0x09, 0x21,        //   Unknown
	0x09, 0x22,        //   Unknown
	0x09, 0x23,        //   Unknown
	0x09, 0x24,        //   Unknown
	0x09, 0x25,        //   Unknown
	0x09, 0x26,        //   Unknown
	0x09, 0x27,        //   Unknown
	0x09, 0x28,        //   Unknown
	0x09, 0x29,        //   Unknown
	0x09, 0x2a,        //   Unknown
	0x09, 0x2b,        //   Unknown
	0x95, 0x0c,        //   REPORT_COUNT (12)
	0x81, 0x02,        //   INPUT (Data,Var,Abs)
	0x0a, 0x21, 0x26,  //   Unknown
	0x95, 0x08,        //   REPORT_COUNT (8)
	0xb1, 0x02,        //   FEATURE (Data,Var,Abs)
	0xc0               // END_COLLECTION
};


#define CONFIG1_DESC_SIZE		(9+9+9+7)
#define GAMEPAD_HID_DESC_OFFSET	(9+9)
static uint8_t PROGMEM config1_descriptor[CONFIG1_DESC_SIZE] = {
	// configuration descriptor, USB spec 9.6.3, page 264-266, Table 9-10
	9, 					// bLength;
	2,					// bDescriptorType;
	LSB(CONFIG1_DESC_SIZE),			// wTotalLength
	MSB(CONFIG1_DESC_SIZE),
	1,					// bNumInterfaces
	1,					// bConfigurationValue
	0,					// iConfiguration
	0x80,					// bmAttributes
	50,					// bMaxPower
	// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
	9,					// bLength
	4,					// bDescriptorType
	GAMEPAD_INTERFACE,			// bInterfaceNumber
	0,					// bAlternateSetting
	1,					// bNumEndpoints
	0x03,					// bInterfaceClass (0x03 = HID)
	0x00,					// bInterfaceSubClass (0x00 = No Boot)
	0x00,					// bInterfaceProtocol (0x00 = No Protocol)
	0,					// iInterface
	// HID interface descriptor, HID 1.11 spec, section 6.2.1
	9,					// bLength
	0x21,					// bDescriptorType
	0x11, 0x01,				// bcdHID
	0,					// bCountryCode
	1,					// bNumDescriptors
	0x22,					// bDescriptorType
	sizeof(gamepad_hid_report_desc),	// wDescriptorLength
	0,
	// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	7,					// bLength
	5,					// bDescriptorType
	GAMEPAD_ENDPOINT | 0x80,		// bEndpointAddress
	0x03,					// bmAttributes (0x03=intr)
	GAMEPAD_SIZE, 0,			// wMaxPacketSize
	10					// bInterval
};

// If you're desperate for a little extra code memory, these strings
// can be completely removed if iManufacturer, iProduct, iSerialNumber
// in the device desciptor are changed to zeros.
struct usb_string_descriptor_struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	int16_t wString[];
};
static struct usb_string_descriptor_struct PROGMEM string0 = {
	4,
	3,
	{0x0409}
};
static struct usb_string_descriptor_struct PROGMEM string1 = {
	sizeof(STR_MANUFACTURER),
	3,
	STR_MANUFACTURER
};
static struct usb_string_descriptor_struct PROGMEM string2 = {
	sizeof(STR_PRODUCT),
	3,
	STR_PRODUCT
};

// This table defines which descriptor data is sent for each specific
// request from the host (in wValue and wIndex).
static struct descriptor_list_struct {
	uint16_t	wValue;
	uint16_t	wIndex;
	const uint8_t	*addr;
	uint8_t		length;
} PROGMEM descriptor_list[] = {
	{0x0100, 0x0000, device_descriptor, sizeof(device_descriptor)},
	{0x0200, 0x0000, config1_descriptor, sizeof(config1_descriptor)},
	{0x2100, GAMEPAD_INTERFACE, config1_descriptor+GAMEPAD_HID_DESC_OFFSET, 9},
	{0x2200, GAMEPAD_INTERFACE, gamepad_hid_report_desc, sizeof(gamepad_hid_report_desc)},
	{0x0300, 0x0000, (const uint8_t *)&string0, 4},
	{0x0301, 0x0409, (const uint8_t *)&string1, sizeof(STR_MANUFACTURER)},
	{0x0302, 0x0409, (const uint8_t *)&string2, sizeof(STR_PRODUCT)}
};
#define NUM_DESC_LIST (sizeof(descriptor_list)/sizeof(struct descriptor_list_struct))


/**************************************************************************
 *
 *  Variables - these are the only non-stack RAM usage
 *
 **************************************************************************/

// zero when we are not configured, non-zero when enumerated
static volatile uint8_t usb_configuration = 0;

static gamepad_state_t PROGMEM gamepad_idle_state = {
	.triangle_btn = 0, .square_btn = 0, .cross_btn = 0, .circle_btn = 0,
	.l1_btn = 0, .r1_btn = 0, .l2_btn = 0, .r2_btn = 0,
	.select_btn = 0, .start_btn = 0, .ps_btn = 0,
	.direction = 0x08,
	.l_x_axis = 0x80, .l_y_axis = 0x80, .r_x_axis = 0x80, .r_y_axis = 0x80,
	.unknown = {0x00, 0x00, 0x00, 0x00},
	.circle_axis = 0x00, .cross_axis = 0x00, .square_axis = 0x00, .triangle_axis = 0x00,
	.l1_axis = 0x00, .r1_axis = 0x00, .l2_axis = 0x00, .r2_axis = 0x00
};

/*
 * Series of bytes that appear in control packets right after the HID
 * descriptor is sent to the host. They where discovered by tracing output
 * from a Madcatz SF4 Joystick. Sending these bytes makes the PS button work.
 */
static uint8_t PROGMEM magic_init_bytes[] = {
	0x21, 0x26, 0x01, 0x07, 0x00, 0x00, 0x00, 0x00
};

static uint8_t gamepad_idle_config = 0;

// protocol setting from the host.  We use exactly the same report
// either way, so this variable only stores the setting since we
// are required to be able to report which setting is in use.
static uint8_t gamepad_protocol = 1;

/**************************************************************************
 *
 *  Public Functions - these are the API intended for the user
 *
 **************************************************************************/

// initialize USB
void usb_init(void) {
	HW_CONFIG();
	USB_FREEZE();				// enable USB
	PLL_CONFIG();				// config PLL
	while (!(PLLCSR & (1<<PLOCK)));		// wait for PLL lock
	USB_CONFIG();				// start USB clock
	UDCON = 0;				// enable attach resistor
	usb_configuration = 0;
	UDIEN = (1<<EORSTE)|(1<<SOFE);
	sei();
}

// return 0 if the USB is not configured, or the configuration
// number selected by the HOST
uint8_t usb_configured(void) {
	return usb_configuration;
}

gamepad_state_t gamepad_state;

inline void usb_gamepad_reset_state(void) {
	memcpy_P(&gamepad_state, &gamepad_idle_state, sizeof(gamepad_state_t));
}

int8_t usb_gamepad_send(void) {
	uint8_t intr_state, timeout, i;

	if (!usb_configuration) return -1;
	intr_state = SREG;
	cli();
	UENUM = GAMEPAD_ENDPOINT;
	timeout = UDFNUML + 50;
	while (1) {
		// are we ready to transmit?
		if (UEINTX & (1<<RWAL)) break;
		SREG = intr_state;
		// has the USB gone offline?
		if (!usb_configuration) return -1;
		// have we waited too long?
		if (UDFNUML == timeout) return -1;
		// get ready to try checking again
		intr_state = SREG;
		cli();
		UENUM = GAMEPAD_ENDPOINT;
	}

	for (i=0; i<sizeof(gamepad_state_t); i++) {
		UEDATX = ((uint8_t*)&gamepad_state)[i];
	}

	UEINTX = 0x3A;
	SREG = intr_state;
	return 0;
}

/**************************************************************************
 *
 *  Private Functions - not intended for general user consumption....
 *
 **************************************************************************/

ISR(USB_GEN_vect)
{
	uint8_t intbits;

	intbits = UDINT;
	UDINT = 0;
	if (intbits & (1<<EORSTI)) {
		UENUM = 0;
		UECONX = 1;
		UECFG0X = EP_TYPE_CONTROL;
		UECFG1X = EP_SIZE(ENDPOINT0_SIZE) | EP_SINGLE_BUFFER;
		UEIENX = (1<<RXSTPE);
		usb_configuration = 0;
	}
}

// Misc functions to wait for ready and send/receive packets
static inline void usb_wait_in_ready(void)
{
	while (!(UEINTX & (1<<TXINI))) ;
}
static inline void usb_send_in(void)
{
	UEINTX = ~(1<<TXINI);
}
static inline void usb_wait_receive_out(void)
{
	while (!(UEINTX & (1<<RXOUTI))) ;
}
static inline void usb_ack_out(void)
{
	UEINTX = ~(1<<RXOUTI);
}

// USB Endpoint Interrupt - endpoint 0 is handled here.  The
// other endpoints are manipulated by the user-callable
// functions, and the start-of-frame interrupt.
//
ISR(USB_COM_vect)
{
	uint8_t intbits;
	const uint8_t *list;
	const uint8_t *cfg;
	uint8_t i, n, len, en;
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;
	uint16_t desc_val;
	const uint8_t *desc_addr;
	uint8_t	desc_length;

	UENUM = 0;
	intbits = UEINTX;
	if (intbits & (1<<RXSTPI)) {
		bmRequestType = UEDATX;
		bRequest = UEDATX;
		wValue = UEDATX;
		wValue |= (UEDATX << 8);
		wIndex = UEDATX;
		wIndex |= (UEDATX << 8);
		wLength = UEDATX;
		wLength |= (UEDATX << 8);
		UEINTX = ~((1<<RXSTPI) | (1<<RXOUTI) | (1<<TXINI));
		if (bRequest == GET_DESCRIPTOR) {
			list = (const uint8_t *)descriptor_list;
			for (i=0; ; i++) {
				if (i >= NUM_DESC_LIST) {
					UECONX = (1<<STALLRQ)|(1<<EPEN);  //stall
					return;
				}
				desc_val = pgm_read_word(list);
				if (desc_val != wValue) {
					list += sizeof(struct descriptor_list_struct);
					continue;
				}
				list += 2;
				desc_val = pgm_read_word(list);
				if (desc_val != wIndex) {
					list += sizeof(struct descriptor_list_struct)-2;
					continue;
				}
				list += 2;
				desc_addr = (const uint8_t *)pgm_read_word(list);
				list += 2;
				desc_length = pgm_read_byte(list);
				break;
			}
			len = (wLength < 256) ? wLength : 255;
			if (len > desc_length) len = desc_length;
			do {
				// wait for host ready for IN packet
				do {
					i = UEINTX;
				} while (!(i & ((1<<TXINI)|(1<<RXOUTI))));
				if (i & (1<<RXOUTI)) return;	// abort
				// send IN packet
				n = len < ENDPOINT0_SIZE ? len : ENDPOINT0_SIZE;
				for (i = n; i; i--) {
					UEDATX = pgm_read_byte(desc_addr++);
				}
				len -= n;
				usb_send_in();
			} while (len || n == ENDPOINT0_SIZE);
			return;
		}
		if (bRequest == SET_ADDRESS) {
			usb_send_in();
			usb_wait_in_ready();
			UDADDR = wValue | (1<<ADDEN);
			return;
		}
		if (bRequest == SET_CONFIGURATION && bmRequestType == 0) {
			usb_configuration = wValue;
			usb_send_in();
			cfg = endpoint_config_table;
			for (i=1; i<5; i++) {
				UENUM = i;
				en = pgm_read_byte(cfg++);
				UECONX = en;
				if (en) {
					UECFG0X = pgm_read_byte(cfg++);
					UECFG1X = pgm_read_byte(cfg++);
				}
			}
			UERST = 0x1E;
			UERST = 0;
			return;
		}
		if (bRequest == GET_CONFIGURATION && bmRequestType == 0x80) {
			usb_wait_in_ready();
			UEDATX = usb_configuration;
			usb_send_in();
			return;
		}

		if (bRequest == GET_STATUS) {
			usb_wait_in_ready();
			i = 0;
			#ifdef SUPPORT_ENDPOINT_HALT
			if (bmRequestType == 0x82) {
				UENUM = wIndex;
				if (UECONX & (1<<STALLRQ)) i = 1;
				UENUM = 0;
			}
			#endif
			UEDATX = i;
			UEDATX = 0;
			usb_send_in();
			return;
		}
		#ifdef SUPPORT_ENDPOINT_HALT
		if ((bRequest == CLEAR_FEATURE || bRequest == SET_FEATURE)
		  && bmRequestType == 0x02 && wValue == 0) {
			i = wIndex & 0x7F;
			if (i >= 1 && i <= MAX_ENDPOINT) {
				usb_send_in();
				UENUM = i;
				if (bRequest == SET_FEATURE) {
					UECONX = (1<<STALLRQ)|(1<<EPEN);
				} else {
					UECONX = (1<<STALLRQC)|(1<<RSTDT)|(1<<EPEN);
					UERST = (1 << i);
					UERST = 0;
				}
				return;
			}
		}
		#endif
		if (wIndex == GAMEPAD_INTERFACE) {
			if (bmRequestType == 0xA1) {
				if (bRequest == HID_GET_REPORT) {
					usb_wait_in_ready();

					for (i=0; i<sizeof(magic_init_bytes); i++) {
						UEDATX = pgm_read_byte(&magic_init_bytes[i]);
					}

					usb_send_in();
					return;
				}
				if (bRequest == HID_GET_IDLE) {
					usb_wait_in_ready();
					UEDATX = gamepad_idle_config;
					usb_send_in();
					return;
				}
				if (bRequest == HID_GET_PROTOCOL) {
					usb_wait_in_ready();
					UEDATX = gamepad_protocol;
					usb_send_in();
					return;
				}
			}
			if (bmRequestType == 0x21) {
				if (bRequest == HID_SET_REPORT) {
					usb_wait_receive_out();
					usb_ack_out();
					usb_send_in();
					return;
				}
				if (bRequest == HID_SET_IDLE) {
					gamepad_idle_config = (wValue >> 8);
					usb_send_in();
					return;
				}
				if (bRequest == HID_SET_PROTOCOL) {
					gamepad_protocol = wValue;
					usb_send_in();
					return;
				}
			}
		}
	}
	UECONX = (1<<STALLRQ) | (1<<EPEN);	// stall
}

