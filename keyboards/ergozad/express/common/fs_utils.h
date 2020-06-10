#ifndef FS_UTILS_HEADER
#define FS_UTILS_HEADER
#include <Adafruit_LittleFS.h>

#include <InternalFileSystem.h>
#include <stdarg.h>
#include <bluefruit.h>
#include <delay.h>
#ifdef __cplusplus
// place here the simbol you wish to export to c programs too
extern "C"{
    void startFs();
    void stopFs();
    void listFile();
    void fs_help();
    boolean fs_remove_item(char* path);
    boolean fs_touch_item(char* path);
    boolean format_fs();
};

#endif
#endif
