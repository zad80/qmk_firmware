#pragma once
#include "config_common.h"
#include <variant.h>
//arduino15/packages/adafruit/hardware/nrf52/0.14.6/variants/feather_nrf52840_express/variant.h
#include <Arduino.h>
#include <stdbool.h>


/*
 *
All projects need to have a config.h file that sets things like the matrix size, product name, USB VID/PID,
 description and other settings.
 In general, use this file to set essential information and defaults for your keyboard that will always work.
 */
/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION ROW2COL

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

//#define NO_DEBUG
//#define NO_PRINT
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
#define NO_ACTION_ONESHOT
#define NO_ACTION_MACRO
//#define NO_ACTION_FUNCTION

/*
 * 2 times local because it is a split keyboard,
 * and we address the keys in a full matrix structure
 * on both halves.
 * */
#define LOCAL_MATRIX_COLS 7
#define LOCAL_MATRIX_OFFSET 0
#define MATRIX_COLS LOCAL_MATRIX_COLS*2
#define MATRIX_ROWS 6
#define PIN_WIRE_SDA         (22)
#define PIN_SERIAL_TX       (0)
/*
 * Analog pins
 */
#define A0               (14) // don't use this don't know why it doesn't work
#define A1               (15)
#define A2               (16)
#define A3               (17)
#define A4               (18)
#define A5               (19)
#define A6               (20)
#define A7               (21)

#define MATRIX_COL_PINS {13, 12, 11, 10, 9, 6, 5}
#define MATRIX_ROW_PINS {A0, A1, A2, A3, A4, A5}


/*

	Using printf on the Arduino.
	by Michael McElligott

	Usage:
	Set a buffer size with _PRINTF_BUFFER_LENGTH_, default is 64 bytes, or about a single line
	Set output stream with _Stream_Obj_. eg; SerialUSB

	printf(format string, argument(s) list).
	printfn(): As above but appends a new line on each print; aka serial.println()

	eg; printf("%.2f, 0x%X", 1234.5678f, 32768);

	For a detailed list on printf specifiers:
	http://www.cplusplus.com/reference/cstdio/printf/
	and
	http://man7.org/linux/man-pages/man3/printf.3.html



	Tested with the Arduino Due 1.6.6
	Jan 2016

*/

/* necessary for quantum.c */
//extern int debug_enable;
typedef uint8_t pin_t;

#define setPinInput(pin) pinMode(pin, INPUT_PULLDOWN)
#define setPinInputHigh(pin)  pinMode(pin, INPUT_PULLUP)
#define setPinInputLow(pin) pinMode(pin, INPUT_PULLDOWN)
#define setPinOutput(pin) pinMode(pin, OUTPUT)
#define writePinHigh(pin) digitalWrite(pin, HIGH)
#define writePinLow(pin) digitalWrite(pin, LOW)
#define writePin(pin, level) ((level) ? writePinHigh(pin) : writePinLow(pin))
#define readPin(pin) digitalRead(pin)
