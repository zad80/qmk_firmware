/*
 * Arduino.h is absolutely necessary to correctly compile this object
 * In case it isn't included symbols aren't correctly named and
 * linking ends up complainig that loop ans setup aren't defined
 * anywere.
 */
#include "config.h"
#include "matrix.h"
#include "action.h"
#include "hid_keyboard.h"
#include "serial.h"
#include "host.h"
#include "report.h"
#include <delay.h>

#include "keyboard.h"
#include <BfButton.h>
#include "fs_utils.h"
#include "battery_utils.h"
#include "singleLed.h"
//#define HALF_LAYOUT_LEFT
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
BLEDis               bledis;
uint8_t              myuuid[16] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x11, 0x00, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
BLEUuid              pear(myuuid);
u_int16_t            serial_last_hb = 0;
u_int32_t            _r             = 0;
u_int32_t            _c             = 0;
#ifdef HALF_LAYOUT_LEFT
// Peripheral hid service
BLEHidAdafruit blehid;

// Central uart client
BLEClientUart clientUart;
#else
// Peripheral uart service
BLEUart bleuart;
#endif
#define BLUETOOTH_TRANSMISSION_DISABLED LED_RED
sllib bt_trans_led(BLUETOOTH_TRANSMISSION_DISABLED);
#define USR_BUTTON_PIN 7
BfButton *user_button;
// Adafruit_NeoPixel neopixel = Adafruit_NeoPixel();

void    startAdv(void);
int     send_bt          = 1;
int     send_volt        = 0;
boolean master_connected = false;
/*
 * Infinity Ergozad Pins usage: look at the config.h
 */
uint8_t cols[]    = MATRIX_COL_PINS;
uint8_t rows[]    = MATRIX_ROW_PINS;
int     col_count = sizeof(cols) / sizeof(cols[0]);
int     row_count = sizeof(rows) / sizeof(rows[0]);

static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];
static bool         debouncing      = false;
static uint16_t     debouncing_time = 0;

static void send_keyboard(report_keyboard_t *report);
static void send_mouse(report_mouse_t *report);
static void send_system(uint16_t data);
static void send_consumer(uint16_t data);

static uint8_t bluefruit_keyboard_leds = 0;

static uint8_t keyboard_leds(void) { return bluefruit_keyboard_leds; }

static host_driver_t driver = {keyboard_leds, send_keyboard, send_mouse, send_system, send_consumer};

extern char _pf_buffer_[_PRINTF_BUFFER_LENGTH_];

host_driver_t *ergozad_driver(void) { return &driver; }

/**
 ****************************************************************************
 ******************* BUTTON FUNCTIONS                   *********************
 ****************************************************************************
 * */
void user_button_handler(BfButton *btn, BfButton::press_pattern_t pattern) {
    switch (pattern) {
        case BfButton::SINGLE_PRESS:
            printfn(" single pressed.");
            toggle_bluetooth();
            break;
        case BfButton::DOUBLE_PRESS:
            printfn(" double pressed.");

            break;
        case BfButton::LONG_PRESS:
            toggle_volt();
            printfn(" long pressed. send_volt=%d", send_volt);
            break;
        default:
            break;
    }
}

void init_user_button() {
    // internal pullup because is an onboard button
    user_button = new BfButton(BfButton::STANDALONE_DIGITAL, USR_BUTTON_PIN, true, LOW);
    user_button->onPress(user_button_handler);
    user_button->onDoublePress(user_button_handler);
    user_button->onPressFor(user_button_handler, 2000);  // two seconds long press
}

void purge_serial() {
    while (Serial.available()) Serial.read();
}

void toggle_bluetooth() {
    printfn("toggle bluetooth");
    if (send_bt) {
        send_bt = 0;
        if (!send_volt) {
            bt_trans_led.setOnSingle();
            printfn("toggle bluetooth forcing off");
        }
        purge_serial();
    } else {
        send_bt = 1;
        if (!send_volt) {
            bt_trans_led.setOffSingle();
            printfn("toggle bluetooth forcing on");
        }
    }
}
void toggle_volt() {
    printfn("toggle volt");

    if (send_volt) {
        send_volt = 0;
#ifdef HALF_LAYOUT_LEFT
        if (isDebugging()) blehid.keySequence("toggle volt OFF\n");
#endif
        if (send_bt) {
            bt_trans_led.setOffSingle();
        } else {
            bt_trans_led.setOnSingle();
        }
    } else {
#ifdef HALF_LAYOUT_LEFT
        if (isDebugging()) blehid.keySequence("toggle volt ON\n");
#endif
        send_volt = 1;
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
void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {}

/**
 ****************************************************************************
 ******************* MATRIX FUNCTIONS                   *********************
 ****************************************************************************
 * */

bool matrix_is_on(uint8_t row, uint8_t col) { return (matrix[row] & (1 << col)); }

matrix_row_t matrix_get_row(uint8_t row) { return matrix[row]; }

#define READ_ROW(r, c) (digitalRead(rows[r]) << c + LOCAL_MATRIX_OFFSET)

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

#ifndef HALF_LAYOUT_LEFT
        if (!send_volt && _r == r && timer_elapsed(serial_last_hb) > 1000 * 5) {
            matrix_debouncing[_r] = 1 << (_c + LOCAL_MATRIX_OFFSET);
            debouncing            = true;
            _c++;
            serial_last_hb = timer_read();
            if (_c == LOCAL_MATRIX_COLS) {
                _c = 0;
                _r++;
            }
            if (_r == MATRIX_ROWS) {
                _r = 0;
            }
        }
#endif  // simulation
    }
    if (debouncing && timer_elapsed(debouncing_time) > DEBOUNCE) {
        for (int row = 0; row < row_count; row++) {
            matrix[row] = matrix_debouncing[row];
        }
#ifndef HALF_LAYOUT_LEFT
        if (master_connected) {
            if (!send_volt) {
                char keys[50];
                snprintf(keys, 50, "%x0x%08x", _r, matrix[_r]);
                bleuart.print(keys);
            }
        }
#endif
        debouncing = false;
    }
    return 1;
}

void led_set(uint8_t usb_led) { return; }

void matrix_print(void) {
    uint8_t data = 0;
    // strobe the columns
    // Set columns to be output and initialize each to LOW.
    printfn("   1 2 3 4 5 6 7 8 9");
    for (int r = 0; r < row_count; r++) {
        printfn("%d: %s", r + 1, byte_to_binary(matrix[r], col_count, "  "));
    }
}

void print_prg_matrix() {
    uint16_t data = 0;
    printfn("   1 2 3 4 5 6 7 8 9");
    for (int r = 0; r < row_count; r++) {
        printfn("%d: %s", r + 1, string_matrix_row(0, r, col_count));
    }
}

const char *string_matrix_row(int layer, int row, int column) {
    static char b[32 * 6];
    char        buffer[30];
    b[0] = '\0';

    int c;
    for (c = 0; c < column; c++) {
        memset(buffer, '\0', 30);
        // sprintf(buffer,"%d[%s]\t",pgm_read_word(&keymaps[(layer)][(row)][(c)]) );
        sprintf(buffer, "%d[%s]\t", keymaps[(layer)][(row)][(c)]);
        strcat(b, buffer);
    }

    return b;
}
void matrix_clean() {
    memset(matrix, 0, MATRIX_ROWS * sizeof(matrix_row_t));
    memset(matrix_debouncing, 0, MATRIX_ROWS * sizeof(matrix_row_t));
}

void matrix_init(void) {
    matrix_clean();
    printf("initializing gpio...");
    // Set rows to be input with interrupt for each.
    for (int i = 0; i < row_count; i++) {
        pinMode(rows[i], INPUT_PULLDOWN);
        // attachInterrupt(digitalPinToInterrupt(rows[i].pin), rows[i].event, RISING);
    }

    // Set columns to be output and initialize each to LOW.
    for (int i = 0; i < col_count; i++) {
        pinMode(cols[i], OUTPUT);
        digitalWrite(cols[i], LOW);
    }
    // pinMode(USR_BUTTON, INPUT_PULLUP);
    printf("DONE initializing gpio");
}

/**
 ****************************************************************************
 ******************* ARDUINO FUNCTIONS                  *********************
 ****************************************************************************
 * */

void setup() {
    // Config Neopixels
    // neopixel.begin();
    Serial.begin(9600);
    // Set up user button

    matrix_init();

#ifdef HALF_LAYOUT_LEFT
    Bluefruit.setName("ergozad MASTER");
    // initialize the Bluefruit with one peripheral (1 by default) connection and one central.
    Bluefruit.begin(1, 1);
    // Callbacks for Central
    Bluefruit.Central.setConnectCallback(cent_connect_callback);
    Bluefruit.Central.setDisconnectCallback(cent_disconnect_callback);
    // Init BLE Central Uart Serivce
    clientUart.begin();
    clientUart.setRxCallback(cent_bleuart_rx_callback);
    /* Start Central Scanning
     * - Enable auto scan if disconnected
     * - Interval = 100 ms, window = 80 ms
     * - Filter only accept bleuart service
     * - Don't use active scan
     * - Start(timeout) with timeout = 0 will scan forever (until connected)
     */
    Bluefruit.Scanner.setRxCallback(scan_callback);
    Bluefruit.Scanner.restartOnDisconnect(true);
    Bluefruit.Scanner.setInterval(160, 80);  // in unit of 0.625 ms
    Bluefruit.Scanner.filterUuid(BLEUART_UUID_SERVICE);
    Bluefruit.Scanner.useActiveScan(false);
    Bluefruit.Scanner.start(0);  // 0 = Don't stop scanning after n seconds
#else
    Bluefruit.begin();
    Bluefruit.setName("ergozad RIGHT");
    // Callbacks for Peripheral
    Bluefruit.Periph.setConnectCallback(prph_connect_callback);
    Bluefruit.Periph.setDisconnectCallback(prph_disconnect_callback);

    // Configure and Start BLE Uart Service
    bleuart.begin();
    bleuart.setRxCallback(prph_bleuart_rx_callback);
#endif

    Bluefruit.setTxPower(0);  // Check bluefruit.h for supported values
    // Configure and Start Device Information Service
    bledis.setManufacturer("Adafruit Industries");
    bledis.setModel("Bluefruit Feather 52");
    bledis.begin();
    // neopixel.setPin(PIN_NEOPIXEL);
#ifdef HALF_LAYOUT_LEFT
    /* Start BLE HID
     * Note: Apple requires BLE device must have min connection interval >= 20m
     * ( The smaller the connection interval the faster we could send data).
     * However for HID and MIDI device, Apple could accept min connection interval
     * up to 11.25 ms. Therefore BLEHidAdafruit::begin() will try to set the min and max
     * connection interval to 11.25  ms and 15 ms respectively for best performance.
     */

    blehid.begin();
#endif
    // Set callback for set LED from central
    // blehid.setKeyboardLedCallback(set_keyboard_led);

    /* Set connection interval (min, max) to your perferred value.
     * Note: It is already set by BLEHidAdafruit::begin() to 11.25ms - 15ms
     * min = 9*1.25=11.25 ms, max = 12*1.25= 15 ms
     */
    /* Bluefruit.Periph.setConnInterval(9, 12); */
    init_user_button();
    printf("Setting host driver to bluefruit...\n");
    host_set_driver(ergozad_driver());
    // Set up and start advertising
    startAdv();
    send_bt = 1;
}

void startAdv(void) {
    // Advertising packet
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addTxPower();
    Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_KEYBOARD);

#ifdef HALF_LAYOUT_LEFT
    // Include BLE HID service
    Bluefruit.Advertising.addService(blehid);
#else
    // Include bleuart 128-bit uuid
    Bluefruit.Advertising.addService(bleuart);
#endif

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
    Bluefruit.Advertising.setInterval(32, 244);  // in unit of 0.625 ms
    Bluefruit.Advertising.setFastTimeout(30);    // number of seconds in fast mode
    Bluefruit.Advertising.start(0);              // 0 = Don't stop advertising after n seconds
}
void check_bt_led_state() {
    if (send_volt && bt_trans_led.runningFunction == 0) {
#ifdef HALF_LAYOUT_LEFT
        if (isDebugging()) blehid.keySequence("check_bt_led_state forcing breath\n");
#endif
        bt_trans_led.setBreathSingle(1000);
    }
}

void loop() {
    // to debug
    if (send_bt == 0 && Serial.available()) {
        serial_debugger();
    }
    // tmk_core/common/keyboard.c
    keyboard_task();
    user_button->read();
    check_bt_led_state();
    bt_trans_led.update();
    if (send_volt && timer_elapsed(serial_last_hb) > 1000 * 60) {  // every 1minutes
        char  volts[50];
        float mv = readVBAT();
        snprintf(volts, 50, "[%f]mv %d% \n", mv, mvToPercent(mv));
        printfn(volts);
#ifdef HALF_LAYOUT_LEFT
        blehid.keySequence(volts);
        serial_last_hb = timer_read();
#endif
    }
}

/**
 ****************************************************************************
 ******************* KEYCODE FUNCTIONS                   ********************
 ****************************************************************************
 * */

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
#ifdef HALF_LAYOUT_LEFT
        blehid.keyboardReport(report->mods, report->keys);
#endif
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
    // return EEPROM_ReadDataByte(p);
    return 0x0;
}

void eeprom_write_byte(uint8_t *Address, uint8_t Value) {
    uint16_t p = (uint32_t)Address;
    // EEPROM_WriteDataByte(p, Value);
}

void eeprom_update_byte(uint8_t *Address, uint8_t Value) {
    uint16_t p = (uint32_t)Address;
    // EEPROM_WriteDataByte(p, Value);
}

uint16_t eeprom_read_word(const uint16_t *Address) {
    const uint16_t p = (const uint32_t)Address;
    // return EEPROM_ReadDataByte(p) | (EEPROM_ReadDataByte(p + 1) << 8);
    return 0x0;
}

void eeprom_write_word(uint16_t *Address, uint16_t Value) {
    uint16_t p = (uint32_t)Address;
    // EEPROM_WriteDataByte(p, (uint8_t)Value);
    // EEPROM_WriteDataByte(p + 1, (uint8_t)(Value >> 8));
}

void eeprom_update_word(uint16_t *Address, uint16_t Value) {
    uint16_t p = (uint32_t)Address;
    // EEPROM_WriteDataByte(p, (uint8_t)Value);
    // EEPROM_WriteDataByte(p + 1, (uint8_t)(Value >> 8));
}

uint32_t eeprom_read_dword(const uint32_t *Address) {
    const uint16_t p = (const uint32_t)Address;
    // return EEPROM_ReadDataByte(p) | (EEPROM_ReadDataByte(p + 1) << 8) | (EEPROM_ReadDataByte(p + 2) << 16) | (EEPROM_ReadDataByte(p + 3) << 24);
    return 0x0;
}

void eeprom_write_dword(uint32_t *Address, uint32_t Value) {
    uint16_t p = (const uint32_t)Address;
    // EEPROM_WriteDataByte(p, (uint8_t)Value);
    // EEPROM_WriteDataByte(p + 1, (uint8_t)(Value >> 8));
    // EEPROM_WriteDataByte(p + 2, (uint8_t)(Value >> 16));
    // EEPROM_WriteDataByte(p + 3, (uint8_t)(Value >> 24));
}

void eeprom_update_dword(uint32_t *Address, uint32_t Value) {
    uint16_t p = (const uint32_t)Address;
    /*uint32_t existingValue = EEPROM_ReadDataByte(p) | (EEPROM_ReadDataByte(p + 1) << 8) | (EEPROM_ReadDataByte(p + 2) << 16) | (EEPROM_ReadDataByte(p + 3) << 24);
    if (Value != existingValue) {
        EEPROM_WriteDataByte(p, (uint8_t)Value);
        EEPROM_WriteDataByte(p + 1, (uint8_t)(Value >> 8));
        EEPROM_WriteDataByte(p + 2, (uint8_t)(Value >> 16));
        EEPROM_WriteDataByte(p + 3, (uint8_t)(Value >> 24));
    }
    */
}

#ifdef HALF_LAYOUT_LEFT
/*------------------------------------------------------------------*/
/* Central
 *------------------------------------------------------------------*/
void scan_callback(ble_gap_evt_adv_report_t *report) {
    //   /* Display the timestamp and device address */
    //   if (report->scan_rsp)
    //   {
    //     /* This is a Scan Response packet */
    //     printfn("[SR%10d] Packet received from ", millis());
    //   }
    //   else
    //   {
    //     /* This is a normal advertising packet */
    //     printfn("[ADV%9d] Packet received from ", millis());
    //   }

    Serial.printBuffer(report->peer_addr.addr, 6, ':');
    printfn("\n");

    /* Raw buffer contents */
    printfn("%14s %d bytes\n", "PAYLOAD", report->data.len);
    if (report->data.len) {
        printfn("%15s", " ");
        Serial.printBuffer(report->data.p_data, report->data.len, '-');
        printfn("");
    }

    /* RSSI value */
    printfn("%14s %d dBm\n", "RSSI", report->rssi);

    /* Adv Type */
    printfn("%14s ", "ADV TYPE");
    /*
    switch (report->type)
    {
      case BLE_GAP_ADV_TYPE_ADV_IND:
        printfn("Connectable undirected\n");
        break;
      case BLE_GAP_ADV_TYPE_ADV_DIRECT_IND:
        printfn("Connectable directed\n");
        break;
      case BLE_GAP_ADV_TYPE_ADV_SCAN_IND:
        printfn("Scannable undirected\n");
        break;
      case BLE_GAP_ADV_TYPE_ADV_NONCONN_IND:
        printfn("Non-connectable undirected\n");
        break;
    }
    */

    /* Check for BLE UART UUID */
    if (Bluefruit.Scanner.checkReportForUuid(report, BLEUART_UUID_SERVICE)) {
        printfn("%14s %s\n", "BLE UART", "UUID Found!");
    }

    /* Check for DIS UUID */
    if (Bluefruit.Scanner.checkReportForUuid(report, UUID16_SVC_DEVICE_INFORMATION)) {
        printfn("%14s %s\n", "DIS", "UUID Found!");
    }

    printfn("");
    // Since we configure the scanner with filterUuid()
    // Scan callback only invoked for device with bleuart service advertised
    // Connect to the device with bleuart service in advertising packet
    Bluefruit.Central.connect(report);
}

void cent_connect_callback(uint16_t conn_handle) {
    // Get the reference to current connection
    BLEConnection *connection = Bluefruit.Connection(conn_handle);

    char peer_name[32] = {0};
    connection->getPeerName(peer_name, sizeof(peer_name));

    printfn("[Cent] Connected to ");
    printfn(peer_name);

    if (clientUart.discover(conn_handle)) {
        // Enable TXD's notify
        clientUart.enableTXD();
    } else {
        // disconnect since we couldn't find bleuart service
        Bluefruit.disconnect(conn_handle);
    }
}

void cent_disconnect_callback(uint16_t conn_handle, uint8_t reason) {
    (void)conn_handle;
    (void)reason;

    printfn("[Cent] Disconnected");
}

/**
 * Callback invoked when uart received data
 * @param cent_uart Reference object to the service where the data
 * arrived. In this example it is clientUart
 */
void cent_bleuart_rx_callback(BLEClientUart &cent_uart) {
    char str[40+1] = {0};
    cent_uart.read(str, 40);

    printfn("[Cent] RX: ");
    printfn(str);
    int r     = str[0] - 48;
    uint32_t data = strtod(&str[1], NULL);
    printfn("[Prph] RX: row=%d, data=[%s] 0x%008x", r, str[1], data);
    matrix[r] |=data;
    /*
      if ( bleuart.notifyEnabled() )
      {
        // Forward data from our peripheral to Mobile
        bleuart.print( str );
      }else
      {
        // response with no prph message
        clientUart.println("[Cent] Peripheral role not connected");
      }
      */
}
#else
/*------------------------------------------------------------------*/
/* Peripheral
 *------------------------------------------------------------------*/
void prph_connect_callback(uint16_t conn_handle) {
    // Get the reference to current connection
    BLEConnection *connection = Bluefruit.Connection(conn_handle);

    char peer_name[32] = {0};
    connection->getPeerName(peer_name, sizeof(peer_name));

    printfn("[Prph] Connected to ");
    printfn(peer_name);
    master_connected = true;
}

void prph_disconnect_callback(uint16_t conn_handle, uint8_t reason) {
    (void)conn_handle;
    (void)reason;
    printfn("[Prph] Disconnected");
    master_connected = false;
}

void prph_bleuart_rx_callback(uint16_t conn_handle) {
    (void)conn_handle;

    // Forward data from Mobile to our peripheral
    char str[40] = {0};
    bleuart.read(str, 40);

    printfn("[Prph] RX: ");
    printfn(str);
    /*
        if (clientUart.discovered()) {
            clientUart.print(str);
        } else {
            bleuart.println("[Prph] Central role not connected");
        }
        */
}
#endif
