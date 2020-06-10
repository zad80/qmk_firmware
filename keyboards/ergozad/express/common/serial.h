#ifndef SERIAL_HEADER
#define SERIAL_HEADER
#ifdef __cplusplus
// place here the simbol you wish to export to c programs too
extern "C"{
    const char *byte_to_binary(int x, int size, char* zero);
    void print(char *string);
    void printfn(char *string, ...);
    void xprint(char *string, ...);
    void xprintf(char *string, ...);
    void serial_debugger();
};
#define _PRINTF_BUFFER_LENGTH_		256
#define _Stream_Obj_				Serial
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

#endif
#endif
