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
#include QMK_KEYBOARD_H

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
 *     row: { A0, A1, A2, A3, A4 } Analog
 *     col: { 13, 12, 11, 10, 9, 6, 5, TX, SDA }
 */
/* matrix state(1:on, 0:off) */
// pin definitions can be found in variant.h
uint8_t cols[] = MATRIX_COL_PINS;
uint8_t rows[] = MATRIX_ROW_PINS;//A6 is used for battery, A7 is analog reference
int col_count = sizeof(cols)/ sizeof(cols[0]);
int row_count = sizeof(rows)/ sizeof(rows[0]);
static uint8_t matrix_read[LOCAL_MATRIX_COLS][MATRIX_ROWS];
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[LOCAL_MATRIX_COLS];
static bool debouncing = false;
static uint16_t debouncing_time = 0;


uint8_t offset = 0;
#ifdef RIGHT_KEYBOARD // used to math the correct character pressed
offset = MATRIX_ROWS - LOCAL_MATRIX_ROWS;
#endif
static void    send_keyboard(report_keyboard_t *report);
static void    send_mouse(report_mouse_t *report);
static void    send_system(uint16_t data);
static void    send_consumer(uint16_t data);

static uint8_t bluefruit_keyboard_leds = 0;

static uint8_t keyboard_leds(void) { return bluefruit_keyboard_leds; }

static host_driver_t driver = {keyboard_leds, send_keyboard, send_mouse, send_system, send_consumer};

host_driver_t *ergozad_driver(void) { return &driver; }

/* action for key
action_t action_for_key(uint8_t layer, keypos_t key){
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
/*
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
*/

#define READ_ROWS(n) (digitalRead(rows[n]) << n)
#define READ_ROW(r,c) (digitalRead(rows[r]) << c)
const char *byte_to_binary(int x, int size)
{
    static char b[32*2];
    b[0] = '\0';
    
    int z;
    for (z = 0; z < size; z++)
    {
        strcat(b, ((x & (1<<z)) == (1<<z)) ? "1 " : "  ");
    }
    
    return b;
}
  
void matrix_print(void) {
    uint8_t data = 0;
    // strobe the columns
    // Set columns to be output and initialize each to LOW.
    printfn("   1 2 3 4 5 6 7 8 9");
    for (int r = 0; r < row_count; r++) {
        printfn("%d: %s", r+1, byte_to_binary(matrix[r], col_count));
    }
}
/*
 *
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
uint8_t my_matrix_read(void) {
    memset(matrix_read, 0, col_count * row_count * sizeof(uint8_t));
    uint8_t data = 0;
    // strobe the columns
    // Set columns to be output and initialize each to LOW.
    for (int c = 0; c < col_count; c++) {
        digitalWrite(cols[c], HIGH);
        for (int r = 0; r < row_count; r++) {
            matrix_read[c][r] = READ_ROW(r);
        }
        // Set columns to LOW.
        digitalWrite(cols[c], LOW);
    }
    return 1;
}
*/

uint8_t my_matrix_scan(void) {
    //memset(matrix_read, 0, col_count * row_count * sizeof(uint8_t));
    bool changed = false;
    // strobe the columns
    // Set columns to be output and initialize each to LOW.
    for (int r = 0; r < row_count; r++) {
        matrix_row_t data = 0;
        matrix_row_t prev =  matrix[r];
        
        for (int c = 0; c < col_count; c++) {
            digitalWrite(cols[c], HIGH);
            data |= READ_ROW(r, c) ;
            // Set columns to LOW.
            digitalWrite(cols[c], LOW);
        }
        
        if (prev!=data) {
            changed = true;
            //printfn("prev = [%d] 0x%08x",r, prev);
            //printfn("data = [%d] 0x%08x",r, data);
            matrix[r] = data;
        }
    }
    if (changed) {
        matrix_print();
    }
    return 1;
}

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



void led_set(uint8_t usb_led) {
    return;
}
void print(char *string) {
    Serial.println(string);
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
    //matrix_clean();
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
    printf("Setting host driver to bluefruit...\n");
    host_set_driver(ergozad_driver());
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

void serial_debugger() {
    char line[256];
    getline(line, sizeof(line));
    if ( strncmp("l", line, strlen("l")) == 0 ) {
        printfn("reading matrix now");
        //my_matrix_read();
        //my_matrix_print();
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
    //keyboard_task();
    //to debug
    //serial_debugger();
    my_matrix_scan();


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
    printfn("send_keyboard");
}

static void send_mouse(report_mouse_t *report) {
    printfn("send_keyboard");
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