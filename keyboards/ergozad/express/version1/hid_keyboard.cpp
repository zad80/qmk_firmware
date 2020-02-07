/*
 * Arduino.h is absolutely necessary to correctly compile this object
 * In case it isn't included symbols aren't correctly named and
 * linking ends up complainig that loop ans setup aren't defined
 * anywere.
 */
#include <variant.h>
#include <Arduino.h>
#include "matrix.h"
#include "action.h"
#include "config.h"
#include <delay.h>
#include <bluefruit.h>
#include "keyboard.h"

BLEDis bledis;
BLEHidAdafruit blehid;
void set_keyboard_led(uint16_t conn_handle, uint8_t led_bitmap);

void startAdv(void);
bool hasKeyPressed = false;
#ifdef __cplusplus
// place here the simbol you wish to export to c programs too
extern "C"{
    void print(char *string);
    void xprint(char *string, ...);
    void led_set(uint8_t usb_led);
    void xprintf(char *string, ...);
};
#endif

static matrix_row_t matrix[MATRIX_ROWS] = {};
matrix_row_t matrix_get_row(uint8_t row) { return matrix[row]; }

void matrix_print(void) {}
/* action for key */
action_t action_for_key(uint8_t layer, keypos_t key){
    action_t a;
    a.code = 0x0;
    return a;
}
/* user defined special function */
void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {

}
uint8_t matrix_scan(void) {
    // quantum use this matrix_scan_kb();
    return 0x0;
}
void xprintf(char *string, ...) {
    return;
}
void led_set(uint8_t usb_led) {
    return;
}
void print(char *string) {
    Serial.println(string);
}

void setup()
{
    Serial.begin(9600);
    Bluefruit.begin();
    Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
    Bluefruit.setName("ergozad");
    
    // Configure and Start Device Information Service
    bledis.setManufacturer("Adafruit Industries");
    bledis.setModel("Bluefruit Feather 52");
    bledis.begin();
    
    /* Start BLE HID
     * Note: Apple requires BLE device must have min connection interval >= 20m
     * ( The smaller the connection interval the faster we could send data).
     * However for HID and MIDI device, Apple could accept min connection interval
     * up to 11.25 ms. Therefore BLEHidAdafruit::begin() will try to set the min and max
     * connection interval to 11.25  ms and 15 ms respectively for best performance.
     */
    blehid.begin();
    
    // Set callback for set LED from central
    blehid.setKeyboardLedCallback(set_keyboard_led);
    
    /* Set connection interval (min, max) to your perferred value.
     * Note: It is already set by BLEHidAdafruit::begin() to 11.25ms - 15ms
     * min = 9*1.25=11.25 ms, max = 12*1.25= 15 ms
     */
    /* Bluefruit.Periph.setConnInterval(9, 12); */
    
    // Set up and start advertising
    startAdv();
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

char *s = "this is a key\n";
char *p = s;
void loop()
{

    // tmk_core/common/keyboard.c
    print("-");
    keyboard_task();
    delay(2);
    print("+");
}

/**
 * Callback invoked when received Set LED from central.
 * Must be set previously with setKeyboardLedCallback()
 *
 * The LED bit map is as follows: (also defined by KEYBOARD_LED_* )
 *    Kana (4) | Compose (3) | ScrollLock (2) | CapsLock (1) | Numlock (0)
 */
void set_keyboard_led(uint16_t conn_handle, uint8_t led_bitmap)
{
    (void) conn_handle;
    
    // light up Red Led if any bits is set
    if ( led_bitmap )
    {
        ledOn( LED_RED );
    }
    else
    {
        ledOff( LED_RED );
    }
}