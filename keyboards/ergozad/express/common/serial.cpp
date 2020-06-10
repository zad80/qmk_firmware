#include "config.h"
#include "hid_keyboard.h"
#include "serial.h"
#include "matrix.h"
#include "keyboard.h"
#include "serial.h"
#include "fs_utils.h"

char _pf_buffer_[_PRINTF_BUFFER_LENGTH_];

void printfn(char *format, ...) {
    va_list args;
    va_start(args, format);
    ssize_t bufsz = vsnprintf(NULL, 0, format,args);
    va_end(args);
    char* buf = (char *)calloc(bufsz + 1, 1);
    va_start(args, format);
    int len = vsnprintf(buf, bufsz + 1, format, args);
    _Stream_Obj_.println(buf);
    va_end(args);
    free(buf);
    return;
}
void printf(char *format, ...) {
    va_list args;
    va_start(args, format);
    ssize_t bufsz = vsnprintf(NULL, 0, format,args);
    va_end(args);
    char* buf = (char *)calloc(bufsz + 1, 1);
    va_start(args, format);
    int len = vsnprintf(buf, bufsz + 1, format, args);
    _Stream_Obj_.print(buf);
    va_end(args);
    free(buf);
    return;
}
void xprintf(char *format, ...) {
     va_list args;
     va_start(args, format);
     int len = vsnprintf(_pf_buffer_, _PRINTF_BUFFER_LENGTH_, format, args);
     _Stream_Obj_.println(_pf_buffer_);
     va_end(args);
     return;
 }
 void xprint(char *format, ...) {
     va_list args;
     va_start(args, format);
     int len = vsnprintf(_pf_buffer_, _PRINTF_BUFFER_LENGTH_, format, args);
     _Stream_Obj_.println(_pf_buffer_);
     va_end(args);
     return;
 }
 void raw_print(char *string) {
     _Stream_Obj_.print(string);
     _Stream_Obj_.flush();
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
        if (c == 0x7f || c == 0x18 || c == '\b') {
            buffer[--idx] = '\0';
            Serial.print(" ");
            Serial.print(c); // echo on
        } else {
          buffer[idx++] = c;
        }

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
    }else if (strncmp("d", line, strlen("d")) == 0) {
        printfn("toggle debug");
        debug_toggle();
    }else if (strncmp("fshelp", line, strlen("fshelp")) == 0) {
       fs_help();
    }else if (strncmp("format", line, strlen("format")) == 0) {
       format_fs();
    }else if (strncmp("fsls", line, strlen("fsls")) == 0) {
       listFile();
    } else if (strncmp("fsrm:", line, strlen("fsrm:")) == 0) {
        int header_size = strlen("fsrm:");
        char * path = strndup(&line[header_size], strlen(line) - (header_size + 1));
        if (fs_remove_item(path) ) {
            printfn("Success");
        } else {
            printfn("Failure");
        }
        free(path);
    } else if (strncmp("fstouch:", line, strlen("fstouch:")) == 0) {
        int header_size = strlen("fstouch:");
        char * path = strndup(&line[header_size],strlen(line) - (header_size + 1));
        if (fs_touch_item(path) ) {
            printfn("Success");
        } else {
            printfn("Failure");
        }
        free(path);
    } else if (strncmp("b", line, strlen("b")) == 0) {
        toggle_bluetooth();
    } else {
        printfn("you typed %s", line);
    }
}
