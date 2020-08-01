#include "config.h"
#include "matrix.h"
#include "timer.h"
#include "hid_keyboard.h"
#include "serial.h"
#include "host.h"
#include "report.h"
#include <delay.h>

#include "keyboard.h"
#include "oled_driver.h"
char* roll_text_state(char* text, int index);
// U8X8_SSD1306_128X32_UNIVISION_SW_I2C u8x8( PIN_WIRE_SCL, PIN_WIRE_SDA, U8X8_PIN_NONE);
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/PIN_WIRE_SCL, /* data=*/PIN_WIRE_SDA, /* reset=*/U8X8_PIN_NONE);  // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED
uint16_t                               last_update = 0;
char                                   lines[N_LINES + 1][OLED_TEXT_MAX];
int                                    x_rolling[]   = {0, 0, 0};
int                                    y_position[]  = {LINE1_Y, LINE2_Y, LINE3_Y};
boolean                                update_needed = 1;
extern boolean                         master_connected;
#define BUFFER_SIZE 16
#define REFRESH_RATE 200
char buffer[BUFFER_SIZE + 1];

int min(int a, int b) { return (a < b) ? a : b; }

char* roll_text_state_lcd(int line) {
    if (strlen(lines[line]) > BUFFER_SIZE) {
        char* res       = roll_text_state(lines[line], x_rolling[line]);
        x_rolling[line] = x_rolling[line] + 1;
        return res;
    } else {
        return lines[line];
    }
}
// this handles both text bigger or smaller than buffer
// is the fusion of algo_2 and algo_3
char* roll_text_state(char* text, int index) {
    int sl           = strlen(text);
    int i            = index;
    int BUFFER_UTILE = BUFFER_SIZE - 1;
    int text_bigger  = sl > BUFFER_SIZE;
    int index_word   = (text_bigger) ? i % sl : i % BUFFER_SIZE;
    int head_size    = min(strlen(&text[index_word]), BUFFER_SIZE);

    int tail_size = text_bigger ? BUFFER_UTILE - (head_size) : min(index_word, sl);

    int index_buffer = text_bigger ? (BUFFER_SIZE - tail_size) : (BUFFER_SIZE - index_word);
    int has_tail     = (tail_size > 0);
    // printf("index %i head_size %i tail_size %i has_tail %i index_buffer %i\n", index_word, head_size, tail_size, has_tail, index_buffer);
    memset(buffer, ' ', BUFFER_SIZE);

    if (head_size > 0 && (text_bigger || index_word < sl)) {
        strncpy(buffer, &text[index_word], head_size);
    }
    if (has_tail) {
        strncpy(&buffer[index_buffer], text, tail_size);
    }
    buffer[BUFFER_SIZE] = '\0';
    return buffer;
}

void oled_setup() {
    memset(&lines[N_LINES][0], '\0', OLED_TEXT_MAX - 1);
    u8g2.begin();
    // u8g2.setPowerSave(0);
    // To draw the background: use font mode 0
    u8g2.setFontMode(0);
    u8g2.setFont(oled_font);
// u8x8.drawString(0,0,"Hello World!");
#ifdef HALF_LAYOUT_LEFT
    setLine(0, "ErgoZad MasterL0");
    setLine(1, "ErgoZad MasterL1");
    setLine(2, "ErgoZad MasterL2");
#else
    setLine(0, "ErgoZad SlaveL0");
    setLine(1, "ErgoZad SlaveL1");
    setLine(2, "ErgoZad SlaveL2");
#endif
}

void print_oled(int x, int y, char* str) { u8g2.drawStr(x, y, str); }

void update_text() {
    if (timer_elapsed(last_update) > REFRESH_RATE) {
        last_update = timer_read();
        int i       = 0;
        while (i < N_LINES) {
            if (strlen(lines[i++]) > BUFFER_SIZE) {
                update_needed = 1;
                break;
            }
        }

        if (update_needed) {
            u8g2.clearBuffer();
            i = 0;
            while (i < N_LINES) {
                print_oled(0, y_position[i], roll_text_state_lcd(i));
                i++;
            }
            u8g2.sendBuffer();
            update_needed = 0;
        }
    }
}
void oled_update_master() { update_text(); }

void oled_update_slave() {
    if (master_connected)
        setLine(0, "ErgoZad LEFT <-->");
    else
        setLine(0, "ErgoZad LEFT");

    update_text();
}

void setLine(int line, char* str) {
    if (line >= 0 && line < 3 && strncmp(str, lines[line], OLED_TEXT_MAX) != 0) {
        lines[line][0] = '\0';
        strncat(lines[line], str, OLED_TEXT_MAX);
        update_needed = 1;
    }
}

void oled_update() {
#ifdef HALF_LAYOUT_LEFT
    oled_update_master();
#else
    oled_update_slave();
#endif
}
