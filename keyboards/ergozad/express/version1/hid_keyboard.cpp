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
#include <stdarg.h>

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
	
void xprintf(char *format, ...)
{
    va_list args;
    va_start(args, format);
    printf(format, args);
    va_end(args);
    return;
}

/*
 * Infinity Ergozad Pinusage:
 * Column pins are input with internal pull-down. Row pins are output and strobe with high.
 * Key is high or 1 when it turns on.
 *
 *     col: { A0, A1, A2, A3, A4 } Analog
 *     row: { 13, 12, 11, 10, 9, 6, 5, SCL, SDA }
 */
/* matrix state(1:on, 0:off) */
// pin definitions can be found in variant.h
uint8_t cols[] = {A0, A1, A2, A3, A4}; //A6 is used for battery, A7 is analog reference
uint8_t rows[] = {13, 12, 11, 10, 9, 6, 5, 22, 23};
int col_count = sizeof(cols)/ sizeof(cols[0]);
int row_count = sizeof(rows)/ sizeof(rows[0]);

static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[LOCAL_MATRIX_ROWS];
static bool debouncing = false;
static uint16_t debouncing_time = 0;


uint8_t offset = 0;
#ifdef RIGHT_KEYBOARD // used to math the correct character pressed
offset = MATRIX_ROWS - LOCAL_MATRIX_ROWS;
#endif

/* action for key */
action_t action_for_key(uint8_t layer, keypos_t key){
    action_t a;
    a.code = 0x0;
    return a;
}
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

void matrix_print(void)
{
    Serial.println("01234567");
    for (uint8_t row = 0; row < row_count; row++) {
        printf("%X0: ", row);
        matrix_row_t data = matrix_get_row(row);
        for (int col = 0; col < MATRIX_COLS; col++) {
            if (data & (1<<col))
                Serial.print("1");
            else
                Serial.print("0");
        }
        Serial.println("");
    }
}

#define READ_ROWS(n) (digitalRead(rows[n]) << n)
uint8_t matrix_scan(void) {
    matrix_row_t data = 0;
    // strobe the columns
    // Set columns to be output and initialize each to LOW.
    for (int c = 0; c < col_count; c++) {
        digitalWrite(cols[c], HIGH);
        // read rows, we cam do it via interrupts
        data = ( READ_ROWS(0) | READ_ROWS(1) | READ_ROWS(2) | READ_ROWS(3) | READ_ROWS(4)
        | READ_ROWS(5) | READ_ROWS(6) | READ_ROWS(7) | READ_ROWS(8));
            // Set columns to LOW.
        digitalWrite(cols[c], LOW);
        if (matrix_debouncing[c] != data) {
            matrix_debouncing[c] = data;
            debouncing = true;
            debouncing_time = millis();
        }
    }

    if (debouncing && (millis() - debouncing_time) > DEBOUNCE) {
        for (int row = 0; row < LOCAL_MATRIX_ROWS; row++) {
            matrix[offset + row] = matrix_debouncing[row];
        }
        debouncing = false;
    }
    //matrix_scan_quantum();
    matrix_print();
    return 1;

}


void led_set(uint8_t usb_led) {
    return;
}
void print(char *string) {
    Serial.println(string);
}

void matrix_clean() {
    memset(matrix, 0, MATRIX_ROWS * sizeof(matrix_row_t));
    memset(matrix_debouncing, 0, LOCAL_MATRIX_ROWS * sizeof(matrix_row_t));
}
/**
 * initialize the gpio for the column and row
 */
void matrix_init(void)
{
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
    matrix_clean();
    // todo: do we need quantum ? matrix_init_quantum();
    print("DONE initializing gpio");
    //matrix_init_quantum();
}


void setup()
{
    Serial.begin(9600);
    matrix_init();
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

void serial_debugger() {
    char line[256];
    getline(line, sizeof(line));
    if ( strncmp("read", line, strlen("read")) == 0 ) {
        printfn("reading matrix now");
        matrix_scan();
        matrix_print();
        matrix_clean();
    } else {
        printfn("you typed %s", line);
    }
}

void loop()
{
    // tmk_core/common/keyboard.c
    keyboard_task();


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