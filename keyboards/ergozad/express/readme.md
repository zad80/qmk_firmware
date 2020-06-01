at this moment the project is still using the Arduino compiler so issue the following:
~~~
export PATH=$PATH:/home/zad/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin/
~~~

Make example for this keyboard (after setting up your build environment):


Ther are two 'projects':
- test
  This is a simple project used only to validate the building inside the qmk source tree.
  The project builds a simple nrf52680express adafruit compatible application wihch blink the 
  default led at 2 seconds intervals.
~~~
    make ergozad/express/test:default
    make ergozad/express/test:default:flash
~~~
- version1
~~~
    make ergozad/express/version1:default
    make ergozad/express/version1:default:flash
~~~

Note: for both projects you have to manually place the board in dfu mode,
double press the reset button once connected.
Todo: implement the open close routine to place the board in dfu via software.

## tmk_core
To integrate the tmk_core, we need to find out which is the main loop
 which execute and read the keys from the matrix
    keyboard_task(); defined in keyboard.c
 1. scan matrix
 
     matrix scanning is done by defining the function matrix_scan in your keyboard
     in case you dont want this done by the quantum, in that case you have only to define
     function to set pin LOW or HIGH 
 1. handle mouse movements
 1. run visualizer code
 1. handle midi commands
 1. light LEDs
 
 The matrix_scan output is a structure organized in array of row (also array) which contain
 zero if the switch isn't pressed and 1 in the contrary.
 To map a key to te actual matrix coordinate to physical key is defined in a C macro named LAYOUT. 
 This is repeatedly called as fast as possible.
 At this point we end up getting the function
 action_for_key layer = 0, keypos = \[r=2,c=5\] called but we don't know
 who call it.
 
 in the keyboard_scan if a detection in the matrix is sensed the function:
action_exec() is called tmk_core/common/action.c and then process_record() called
action_exec() has as argument a keyrecord_t*
~~~
/* key matrix position */
typedef struct {
    uint8_t col;
    uint8_t row;
} keypos_t;

/* key event */
typedef struct {
    keypos_t key;
    bool     pressed;
    uint16_t time;
} keyevent_t;

(keyevent_t){
  .key = (keypos_t){.row = r, .col = c},
  .pressed = (matrix_row & col_mask),
  .time = (timer_read() | 1) /* time should not be 0 */
} 

~~~
at some point process_record call process_action
when there is a switch state detection it is dispatched to the process_record set of functions.

Process Record
The process_record() function itself is deceptively simple, but hidden within is a gateway to overriding functionality at various levels of QMK. The chain of events is listed below, using cluecard whenever we need to look at the keyboard/keymap level functions. Depending on options set in rules.mk or elsewhere, only a subset of the functions below will be included in final firmware.
void process_record(keyrecord_t *record)
bool process_record_quantum(keyrecord_t *record)
Map this record to a keycode
void velocikey_accelerate(void)
void preprocess_tap_dance(uint16_t keycode, keyrecord_t *record)
bool process_key_lock(uint16_t keycode, keyrecord_t *record)
bool process_clicky(uint16_t keycode, keyrecord_t *record)
bool process_haptic(uint16_t keycode, keyrecord_t *record)
bool process_record_kb(uint16_t keycode, keyrecord_t *record)
bool process_record_user(uint16_t keycode, keyrecord_t *record)
bool process_rgb_matrix(uint16_t keycode, keyrecord_t *record)
bool process_midi(uint16_t keycode, keyrecord_t *record)
bool process_audio(uint16_t keycode, keyrecord_t *record)
bool process_steno(uint16_t keycode, keyrecord_t *record)
bool process_music(uint16_t keycode, keyrecord_t *record)
bool process_tap_dance(uint16_t keycode, keyrecord_t *record)
bool process_unicode_common(uint16_t keycode, keyrecord_t *record)
calls one of:
bool process_unicode(uint16_t keycode, keyrecord_t *record)
bool process_unicodemap(uint16_t keycode, keyrecord_t *record)
bool process_ucis(uint16_t keycode, keyrecord_t *record)
bool process_leader(uint16_t keycode, keyrecord_t *record)
bool process_combo(uint16_t keycode, keyrecord_t *record)
bool process_printer(uint16_t keycode, keyrecord_t *record)
bool process_auto_shift(uint16_t keycode, keyrecord_t *record)
bool process_terminal(uint16_t keycode, keyrecord_t *record)
Identify and process Quantum-specific keycodes
At any step during this chain of events a function (such as process_record_kb()) can return false to halt all further processing.
After this is called, post_process_record() is called, which can be used to handle additional cleanup that needs to be run after the keycode is normally handled.
void post_process_record(keyrecord_t *record)
void post_process_record_quantum(keyrecord_t *record)
Map this record to a keycode
void post_process_clicky(uint16_t keycode, keyrecord_t *record)
void post_process_record_kb(uint16_t keycode, keyrecord_t *record)
void post_process_record_user(uint16_t keycode, keyrecord_t *record)

