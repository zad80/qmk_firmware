#ifndef OLED_DRIVER_HEADER
#define OLED_DRIVER_HEADER
#include <stdarg.h>
#include <delay.h>
#include <U8g2lib.h>
#define oled_font u8g2_font_6x12_mr
#define LINE1_Y 7
#define LINE2_Y 18
#define LINE3_Y 30
#define N_LINES 3
#define OLED_TEXT_MAX 40
#ifdef __cplusplus
// place here the simbol you wish to export to c programs too
extern "C"{
    void setLine(int line, char *);
    void oled_setup();
    void oled_update();
};
#endif
#endif
