#pragma once
#include "config_common.h"
/*
 *
All projects need to have a config.h file that sets things like the matrix size, product name, USB VID/PID,
 description and other settings.
 In general, use this file to set essential information and defaults for your keyboard that will always work.
 */
/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW

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

#define MATRIX_ROWS 18
#define MATRIX_COLS 5
#define LOCAL_MATRIX_ROWS 9

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


