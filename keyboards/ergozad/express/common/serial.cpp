#include "config.h"
#include "hid_keyboard.h"
#include <stdarg.h>
#include "serial.h"
#include "matrix.h"
#include "keyboard.h"
#include "serial.h"

char _pf_buffer_[_PRINTF_BUFFER_LENGTH_];

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
