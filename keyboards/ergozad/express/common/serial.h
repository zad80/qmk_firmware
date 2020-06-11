#ifndef SERIAL_HEADER
#define SERIAL_HEADER
#include <stdarg.h>
#ifdef __cplusplus
// place here the simbol you wish to export to c programs too
extern "C"{
    const char *byte_to_binary(int x, int size, char* zero);
    void printfn(char *string, ...);
    void xprint(char *string, ...);
    void xprintf(char *string, ...);
    void raw_print(char *string);
    void serial_debugger();
    boolean isDebugging();
};
#define _PRINTF_BUFFER_LENGTH_		256
#define _Stream_Obj_				Serial

#endif
#endif
