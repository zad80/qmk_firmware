/*
 * Arduino.h is absolutely necessary to correctly compile this object
 * In case it isn't included symbols aren't correctly named and
 * linking ends up complainig that loop ans setup aren't defined
 * anywere.
 */
#include "config.h"
#include "matrix.h"
#include "action.h"

#include "host.h"
#include "report.h"
#include <delay.h>
#include <bluefruit.h>
#include "keyboard.h"
#include <stdarg.h>
#include <BfButton.h>
/**
 * BE AWARE !!!! PIN DEFINITION
 * the number used in digitalWrite and so on is used in the
 * g_ADigitalPinMap[number_used]
 * defined in variant.cpp in keyboards/ergozad/boards/nrf52/core/feather_nrf52840_express/variant.cpp
 * so for example digitalWrite(LED_RED, 1) translate in write i to the pin mapped g_ADigitalPinMap[LED_RED]
 **/


//#include <Adafruit_NeoPixel.h>
#include QMK_KEYBOARD_H
extern const uint8_t hid_keycode_to_ascii[128][2];
BLEDis bledis;
BLEHidAdafruit blehid;
#define BLUTOOTH_TRANSMISSION_DISABLED LED_RED
#define USR_BUTTON_PIN 7
BfButton user_button(BfButton::STANDALONE_DIGITAL, USR_BUTTON_PIN, false, HIGH);
//Adafruit_NeoPixel neopixel = Adafruit_NeoPixel();

void startAdv(void);
bool hasKeyPressed = false;
#ifdef __cplusplus
// place here the simbol you wish to export to c programs too
extern "C"{
    const char *byte_to_binary(int x, int size, char* zero);
    void print(char *string);
    void printfn(char *string, ...);
    void xprint(char *string, ...);
    void led_set(uint8_t usb_led);
    void xprintf(char *string, ...);
    uint8_t eeprom_read_byte(const uint8_t *Address);
    void eeprom_write_byte(uint8_t *Address, uint8_t Value);
    void eeprom_update_byte(uint8_t *Address, uint8_t Value);
    uint16_t eeprom_read_word(const uint16_t *Address);
    void eeprom_write_word(uint16_t *Address, uint16_t Value);
    void eeprom_update_word(uint16_t *Address, uint16_t Value);
    uint32_t eeprom_read_dword(const uint32_t *Address);
    void eeprom_write_dword(uint32_t *Address, uint32_t Value);
    void eeprom_update_dword(uint32_t *Address, uint32_t Value);
    void toggle_bluetooth();
};
#endif


#define _PRINTF_BUFFER_LENGTH_		256
#define _Stream_Obj_				Serial


static char _pf_buffer_[_PRINTF_BUFFER_LENGTH_];

#define printf(a,...)														\
	do{																		\
	snprintf(_pf_buffer_, sizeof(_pf_buffer_), a, ##__VA_ARGS__);			\
	_Stream_Obj_.print(_pf_buffer_);										\
	}while(0)

#define printfn(a,...)														\
	do{																		\
	snprintf(_pf_buffer_, sizeof(_pf_buffer_), a, ##__VA_ARGS__);			\
	_Stream_Obj_.print(_pf_buffer_);										\
	_Stream_Obj_.println("");                                               \
	}while(0)

void xprintf(char *format, ...) {
    va_list args;
    va_start(args, format);
    int len = vsnprintf(_pf_buffer_, _PRINTF_BUFFER_LENGTH_, format, args);
    _Stream_Obj_.print(_pf_buffer_);
    va_end(args);
    return;
}

void print(char *string)
{
    _Stream_Obj_.print(string);
    return;
}

/*
 * Infinity Ergozad Pins usage: look at the config.h
*/
uint8_t cols[] = MATRIX_COL_PINS;
uint8_t rows[] = MATRIX_ROW_PINS;
int col_count = sizeof(cols)/ sizeof(cols[0]);
int row_count = sizeof(rows)/ sizeof(rows[0]);

static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];
static bool debouncing = false;
static uint16_t debouncing_time = 0;

static void    send_keyboard(report_keyboard_t *report);
static void    send_mouse(report_mouse_t *report);
static void    send_system(uint16_t data);
static void    send_consumer(uint16_t data);

static uint8_t bluefruit_keyboard_leds = 0;

static uint8_t keyboard_leds(void) { return bluefruit_keyboard_leds; }

static host_driver_t driver = {keyboard_leds, send_keyboard, send_mouse, send_system, send_consumer};


static int send_bt = 1;

host_driver_t *ergozad_driver(void) { return &driver; }

void user_button_handler(BfButton *btn, BfButton::press_pattern_t pattern) {
    switch (pattern) {
        case BfButton::SINGLE_PRESS:
            printfn(" double pressed.");
            toggle_bluetooth();
            break;
        case BfButton::DOUBLE_PRESS:
            printfn(" double pressed.");
            break;
        case BfButton::LONG_PRESS:
            printfn(" long pressed.");
            break;
        default:
            break;
    }
}
/* action for key
action_t action_for_key(uint8_t layer, keypos_t key){
    printfn("action_for_key layer = %d, keypos = [r=%d,c=%d]",layer, key.row, key.col);
    action_t a;
    a.code = 0x0;
    return a;
}
*/
/* user defined special function */
void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {

}

bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & (1<<col));
}

matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}


#define READ_ROW(r,c) (digitalRead(rows[r]) << c + LOCAL_MATRIX_OFFSET)
const char *byte_to_binary(int x, int size, char* zero)
{
    static char b[32*2];
    b[0] = '\0';

    int z;
    for (z = 0; z < size; z++)
    {
        strcat(b, ((x & (1<<z)) == (1<<z)) ? "1 " : zero);
    }

    return b;
}
const char *string_matrix_row(int layer, int row, int column)
{
    static char b[32*6];
    char buffer[30];
    b[0] = '\0';

    int c;
    for (c = 0; c < column; c++)
    {
        memset(buffer,'\0', 30);
        //sprintf(buffer,"%d[%s]\t",pgm_read_word(&keymaps[(layer)][(row)][(c)]) );
        sprintf(buffer,"%d[%s]\t",keymaps[(layer)][(row)][(c)] );
        strcat(b, buffer);
    }

    return b;
}

void matrix_print(void) {
    uint8_t data = 0;
    // strobe the columns
    // Set columns to be output and initialize each to LOW.
    printfn("   1 2 3 4 5 6 7 8 9");
    for (int r = 0; r < row_count; r++) {
        printfn("%d: %s", r+1, byte_to_binary(matrix[r], col_count, "  "));
    }
}

void print_prg_matrix() {
    uint16_t data = 0;
    printfn("   1 2 3 4 5 6 7 8 9");
    for (int r = 0; r < row_count; r++) {
        printfn("%d: %s", r+1, string_matrix_row(0 , r, col_count));
    }
}

// this is called by the tmk_core
uint8_t matrix_scan(void) {
    // strobe the columns
    // Set columns to be output and initialize each to LOW.
    for (int r = 0; r < row_count; r++) {
        matrix_row_t data = 0;
        matrix_row_t prev = matrix[r];

        for (int c = 0; c < col_count; c++) {
            digitalWrite(cols[c], HIGH);
            data |= READ_ROW(r, c);
            // Set columns to LOW.
            digitalWrite(cols[c], LOW);
        }
        if (matrix_debouncing[r] != data) {
            matrix_debouncing[r] = data;
            if (!debouncing) {
                debouncing      = true;
                debouncing_time = timer_read();
            }
        }
        // matrix[r] = data;
    }
    if (debouncing && timer_elapsed(debouncing_time) > DEBOUNCE) {
        for (int row = 0; row < row_count; row++) {
            matrix[row] = matrix_debouncing[row];
        }
        debouncing = false;
    }
    return 1;
}

void led_set(uint8_t usb_led) {
    return;
}

void matrix_clean() {
    memset(matrix, 0, MATRIX_ROWS * sizeof(matrix_row_t));
    memset(matrix_debouncing, 0, MATRIX_ROWS * sizeof(matrix_row_t));
}


void matrix_init(void)
{
    matrix_clean();
    print("initializing gpio...");
    // Set rows to be input with interrupt for each.
    for (int i = 0; i < row_count; i++) {
        pinMode(rows[i], INPUT_PULLDOWN);
        //attachInterrupt(digitalPinToInterrupt(rows[i].pin), rows[i].event, RISING);
    }

    // Set columns to be output and initialize each to LOW.
    for (int i = 0; i < col_count; i++) {
        pinMode(cols[i], OUTPUT);
        digitalWrite(cols[i], LOW);
    }
    //pinMode(USR_BUTTON, INPUT_PULLUP);
    print("DONE initializing gpio");
}


void setup()
{
    // Config Neopixels
    //neopixel.begin();
    matrix_init();
    Bluefruit.begin();
    Bluefruit.setTxPower(0);    // Check bluefruit.h for supported values
    Bluefruit.setName("ergozad");

    // Configure and Start Device Information Service
    bledis.setManufacturer("Adafruit Industries");
    bledis.setModel("Bluefruit Feather 52");
    bledis.begin();
    //neopixel.setPin(PIN_NEOPIXEL);
    /* Start BLE HID
     * Note: Apple requires BLE device must have min connection interval >= 20m
     * ( The smaller the connection interval the faster we could send data).
     * However for HID and MIDI device, Apple could accept min connection interval
     * up to 11.25 ms. Therefore BLEHidAdafruit::begin() will try to set the min and max
     * connection interval to 11.25  ms and 15 ms respectively for best performance.
     */
    blehid.begin();

    // Set callback for set LED from central
    //blehid.setKeyboardLedCallback(set_keyboard_led);

    /* Set connection interval (min, max) to your perferred value.
     * Note: It is already set by BLEHidAdafruit::begin() to 11.25ms - 15ms
     * min = 9*1.25=11.25 ms, max = 12*1.25= 15 ms
     */
    /* Bluefruit.Periph.setConnInterval(9, 12); */
    printf("Setting host driver to bluefruit...\n");
    host_set_driver(ergozad_driver());
    // Set up and start advertising
    startAdv();
    // Set up user button
    user_button.onPress(user_button_handler)
     .onDoublePress(user_button_handler) // default timeout
     .onPressFor(user_button_handler, 1000); // custom timeout for 1 second
}


void startAdv(void)
{
    // Advertising packet
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addTxPower();
    Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_KEYBOARD);

    // Include BLE HID service
    Bluefruit.Advertising.addService(blehid);

    // There is enough room for the dev name in the advertising packet
    Bluefruit.Advertising.addName();

    /* Start Advertising
     * - Enable auto advertising if disconnected
     * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
     * - Timeout for fast mode is 30 seconds
     * - Start(timeout) with timeout = 0 will advertise forever (until connected)
     *
     * For recommended advertising interval
     * https://developer.apple.com/library/content/qa/qa1931/_index.html
     */
    Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
    Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
    Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
}
void stopAdv(void){
    Bluefruit.setTxPower(-80);
}
char *s = "this is a key\n";
char *p = s;
void getline(char * buffer, int maxchar)
{
    uint8_t idx = 0;
    char c;
    do
    {
        while (Serial.available() == 0) ; // wait for a char this causes the blocking
        c = Serial.read();
        Serial.print(c); // echo on
        buffer[idx++] = c;
    }
    while (c != '\n' && c != '\r' && idx < maxchar);
    buffer[idx] = 0;
}
void toggle_bluetooth() {
    printfn("toggle bluetoot");

    if (send_bt) {
        send_bt = 0;
        Serial.begin(9600);
        stopAdv();
        ledOn(BLUTOOTH_TRANSMISSION_DISABLED);
    } else {
        send_bt = 1;
        Serial.begin(1200);
        startAdv();
        ledOff(BLUTOOTH_TRANSMISSION_DISABLED);
    }
}
void serial_debugger() {
    char line[256];
    getline(line, sizeof(line));
    if (strncmp("m", line, strlen("m")) == 0) {
        printf("print matrix");
        print_prg_matrix();

    }else if (strncmp("l", line, strlen("l")) == 0) {
        printfn("reading matrix now");
        matrix_scan();
        matrix_print();
    } else if (strncmp("d", line, strlen("d")) == 0) {
        printfn("toggle debug");
        debug_toggle();
    }else if (strncmp("b", line, strlen("b")) == 0) {
       toggle_bluetooth();
    } else {
        printfn("you typed %s", line);
    }
}
void    loop() {
    // to debug
    if (send_bt == 0; Serial.available()) {
        serial_debugger();
    }
    // tmk_core/common/keyboard.c
    keyboard_task();
    user_button.read();
}

static void send_keyboard(report_keyboard_t *report) {
    /*
    #ifdef BLUEFRUIT_TRACE_SERIAL
    bluefruit_trace_header();
#endif
    bluefruit_serial_send(0xFD);
    for (uint8_t i = 0; i < KEYBOARD_REPORT_SIZE; i++) {
        bluefruit_serial_send(report->raw[i]);
    }
#ifdef BLUEFRUIT_TRACE_SERIAL
    bluefruit_trace_footer();
#endif
     */
    if (send_bt) {
        blehid.keyboardReport(report->mods, report->keys);
    } else {
        bool shifted = false;
        if (report->mods) {
            if (report->mods & (KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_RIGHTCTRL)) {
                printfn("Ctrl ");
            }

            if (report->mods & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT)) {
                printfn("Shift ");

                shifted = true;
            }

            if (report->mods & (KEYBOARD_MODIFIER_LEFTALT | KEYBOARD_MODIFIER_RIGHTALT)) {
                printfn("Alt ");
            }
        }
        for (uint8_t i = 0; i < KEYBOARD_REPORT_SIZE; i++) {
            char    ch = 0;
            uint8_t kc = report->keys[i];
            if (kc < 128) {
                ch = shifted ? hid_keycode_to_ascii[kc][1] : hid_keycode_to_ascii[kc][0];
            } else {
                // non-US keyboard !!??
            }
            // Printable
            if (ch) {
                printfn("%c", ch);
            }
        }
    }
}

static void send_mouse(report_mouse_t *report) {
    printfn("send_mouse");
    /*
#ifdef BLUEFRUIT_TRACE_SERIAL
    bluefruit_trace_header();
#endif
    bluefruit_serial_send(0xFD);
    bluefruit_serial_send(0x00);
    bluefruit_serial_send(0x03);
    bluefruit_serial_send(report->buttons);
    bluefruit_serial_send(report->x);
    bluefruit_serial_send(report->y);
    bluefruit_serial_send(report->v);  // should try sending the wheel v here
    bluefruit_serial_send(report->h);  // should try sending the wheel h here
    bluefruit_serial_send(0x00);
#ifdef BLUEFRUIT_TRACE_SERIAL
    bluefruit_trace_footer();
#endif
     */
}

static void send_system(uint16_t data) {}

/*
+-----------------+-------------------+-------+
| Consumer Key    | Bit Map           | Hex   |
+-----------------+-------------------+-------+
| Home            | 00000001 00000000 | 01 00 |
| KeyboardLayout  | 00000010 00000000 | 02 00 |
| Search          | 00000100 00000000 | 04 00 |
| Snapshot        | 00001000 00000000 | 08 00 |
| VolumeUp        | 00010000 00000000 | 10 00 |
| VolumeDown      | 00100000 00000000 | 20 00 |
| Play/Pause      | 01000000 00000000 | 40 00 |
| Fast Forward    | 10000000 00000000 | 80 00 |
| Rewind          | 00000000 00000001 | 00 01 |
| Scan Next Track | 00000000 00000010 | 00 02 |
| Scan Prev Track | 00000000 00000100 | 00 04 |
| Random Play     | 00000000 00001000 | 00 08 |
| Stop            | 00000000 00010000 | 00 10 |
+-------------------------------------+-------+
*/
#define CONSUMER2BLUEFRUIT(usage) (usage == AUDIO_MUTE ? 0x0000 : (usage == AUDIO_VOL_UP ? 0x1000 : (usage == AUDIO_VOL_DOWN ? 0x2000 : (usage == TRANSPORT_NEXT_TRACK ? 0x0002 : (usage == TRANSPORT_PREV_TRACK ? 0x0004 : (usage == TRANSPORT_STOP ? 0x0010 : (usage == TRANSPORT_STOP_EJECT ? 0x0000 : (usage == TRANSPORT_PLAY_PAUSE ? 0x4000 : (usage == AL_CC_CONFIG ? 0x0000 : (usage == AL_EMAIL ? 0x0000 : (usage == AL_CALCULATOR ? 0x0000 : (usage == AL_LOCAL_BROWSER ? 0x0000 : (usage == AC_SEARCH ? 0x0400 : (usage == AC_HOME ? 0x0100 : (usage == AC_BACK ? 0x0000 : (usage == AC_FORWARD ? 0x0000 : (usage == AC_STOP ? 0x0000 : (usage == AC_REFRESH ? 0x0000 : (usage == AC_BOOKMARKS ? 0x0000 : 0)))))))))))))))))))

static void send_consumer(uint16_t data) {
    printfn("send_consumer");
    /*
    static uint16_t last_data = 0;
    if (data == last_data) return;
    last_data = data;

    uint16_t bitmap = CONSUMER2BLUEFRUIT(data);

#ifdef BLUEFRUIT_TRACE_SERIAL
    dprintf("\nData: ");
    debug_hex16(data);
    dprintf("; bitmap: ");
    debug_hex16(bitmap);
    dprintf("\n");
    bluefruit_trace_header();
#endif
    bluefruit_serial_send(0xFD);
    bluefruit_serial_send(0x00);
    bluefruit_serial_send(0x02);
    bluefruit_serial_send((bitmap >> 8) & 0xFF);
    bluefruit_serial_send(bitmap & 0xFF);
    bluefruit_serial_send(0x00);
    bluefruit_serial_send(0x00);
    bluefruit_serial_send(0x00);
    bluefruit_serial_send(0x00);
#ifdef BLUEFRUIT_TRACE_SERIAL
    bluefruit_trace_footer();
#endif
     */
}

/* ###############################################################################
 * ############### Unused functions just keep for reference
 * ###############################################################################
 */
/*
 *
static uint8_t matrix_read[LOCAL_MATRIX_COLS][MATRIX_ROWS];
uint8_t my_matrix_print(void) {
    uint8_t data = 0;
    // strobe the columns
    // Set columns to be output and initialize each to LOW.
    printfn("   1 2 3 4 5 6 7 8 9");
    for (int r = 0; r < row_count; r++) {
        printf("%d: ", r + 1);
    for (int c = 0; c < col_count; c++) {
        printf("%d ", matrix_read[c][r]);
    }
    printfn("");
    }
    return 1;

}
static bool debouncing = false;
static uint16_t debouncing_time = 0;
#define READ_ROWS(n) (digitalRead(rows[n]) << n)
uint8_t matrix_scan(void) {
    matrix_row_t data = 0;
    boolean changed = false;

    // strobe the columns
    // Set columns to be output and initialize each to LOW.
    for (int c = 0; c < col_count; c++) {
        digitalWrite(cols[c], HIGH);
        // read rows, we cam do it via interrupts
        data = ( READ_ROWS(0) | READ_ROWS(1) | READ_ROWS(2) | READ_ROWS(3) | READ_ROWS(4));
        // Set columns to LOW.
        printfn("data 0x%x", data);
        digitalWrite(cols[c], LOW);
        if (matrix_debouncing[c] != data) {
            matrix_debouncing[c] = data;
            debouncing = true;
            debouncing_time = millis();
        }
    }
    // offset is used to determine the side left = 0 and right = 9

    if (debouncing && (millis() - debouncing_time) > DEBOUNCE) {
        for (int col = 0; col < LOCAL_MATRIX_COLS; col++) {
            matrix_row_t prev =  matrix[offset + col];
            matrix[offset + col] = matrix_debouncing[col];
            changed = prev !=  matrix[offset + col];
        }
        debouncing = false;
    }
    //matrix_scan_quantum();
    if (changed) {
        matrix_print();
    }
    return 1;

}
 */

/*****************************************************************************
 *  Wrap library in AVR style functions.
 *******************************************************************************/
uint8_t eeprom_read_byte(const uint8_t *Address) {
    const uint16_t p = (const uint32_t)Address;
    //return EEPROM_ReadDataByte(p);
    return 0x0;
}

void eeprom_write_byte(uint8_t *Address, uint8_t Value) {
    uint16_t p = (uint32_t)Address;
    //EEPROM_WriteDataByte(p, Value);
}

void eeprom_update_byte(uint8_t *Address, uint8_t Value) {
    uint16_t p = (uint32_t)Address;
    //EEPROM_WriteDataByte(p, Value);
}

uint16_t eeprom_read_word(const uint16_t *Address) {
    const uint16_t p = (const uint32_t)Address;
    //return EEPROM_ReadDataByte(p) | (EEPROM_ReadDataByte(p + 1) << 8);
    return 0x0;
}

void eeprom_write_word(uint16_t *Address, uint16_t Value) {
    uint16_t p = (uint32_t)Address;
    //EEPROM_WriteDataByte(p, (uint8_t)Value);
    //EEPROM_WriteDataByte(p + 1, (uint8_t)(Value >> 8));
}

void eeprom_update_word(uint16_t *Address, uint16_t Value) {
    uint16_t p = (uint32_t)Address;
    //EEPROM_WriteDataByte(p, (uint8_t)Value);
    //EEPROM_WriteDataByte(p + 1, (uint8_t)(Value >> 8));
}

uint32_t eeprom_read_dword(const uint32_t *Address) {
    const uint16_t p = (const uint32_t)Address;
    //return EEPROM_ReadDataByte(p) | (EEPROM_ReadDataByte(p + 1) << 8) | (EEPROM_ReadDataByte(p + 2) << 16) | (EEPROM_ReadDataByte(p + 3) << 24);
    return 0x0;
}

void eeprom_write_dword(uint32_t *Address, uint32_t Value) {
    uint16_t p = (const uint32_t)Address;
    //EEPROM_WriteDataByte(p, (uint8_t)Value);
    //EEPROM_WriteDataByte(p + 1, (uint8_t)(Value >> 8));
    //EEPROM_WriteDataByte(p + 2, (uint8_t)(Value >> 16));
    //EEPROM_WriteDataByte(p + 3, (uint8_t)(Value >> 24));
}

void eeprom_update_dword(uint32_t *Address, uint32_t Value) {
    uint16_t p             = (const uint32_t)Address;
    /*uint32_t existingValue = EEPROM_ReadDataByte(p) | (EEPROM_ReadDataByte(p + 1) << 8) | (EEPROM_ReadDataByte(p + 2) << 16) | (EEPROM_ReadDataByte(p + 3) << 24);
    if (Value != existingValue) {
        EEPROM_WriteDataByte(p, (uint8_t)Value);
        EEPROM_WriteDataByte(p + 1, (uint8_t)(Value >> 8));
        EEPROM_WriteDataByte(p + 2, (uint8_t)(Value >> 16));
        EEPROM_WriteDataByte(p + 3, (uint8_t)(Value >> 24));
    }
    */
}

