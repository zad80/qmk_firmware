#include "config.h"
#include "matrix.h"
#include "action.h"
#include "hid_keyboard.h"
#include "serial.h"
#include "fs_utils.h"
using namespace Adafruit_LittleFS_Namespace;
extern char _pf_buffer_[_PRINTF_BUFFER_LENGTH_];
void        startFs() { InternalFS.begin(); }
void        stopFs() { InternalFS.end(); }

static int n_items = 0;
/**************************************************************************/
/*!
    @brief  Print out whole directory tree of an folder
            until the level reach MAX_LEVEL

    @note   Recursive call
*/
/**************************************************************************/
void printTreeDir(const char* cwd, uint8_t level) {
    // Open the input folder
    File dir(cwd, FILE_O_READ, InternalFS);

    // File within folder
    File item(InternalFS);
    if (level ==0) printfn("root");
    // Loop through the directory
    while ((item = dir.openNextFile(FILE_O_READ))) {
        // Indentation according to dir level
        for(int i=0; i<level; i++) raw_print("|  ");
        if (item.isDirectory()) {
            printfn("|_ %s/", item.name());

            // ATTENTION recursive call to print sub folder with level+1 !!!!!!!!
            // High number of MAX_LEVEL can cause memory overflow

            char *dpath = (char *)calloc(1,strlen(cwd) + strlen(item.name()) + 2);
            strcpy(dpath, cwd);
            strcat(dpath, "/");
            strcat(dpath, item.name());
            //printfn("%s/", dpath);
            printTreeDir(dpath, level + 1);
            free(dpath);
        } else {
            // Print file size starting from position 30
            int pos = level * 3 + 3 + strlen(item.name());
            raw_print((char *)item.name());
            // Print padding
            for (int i = pos; i < 30; i++) raw_print(" ");
            // Print at least one extra space in case current position > 50
            printfn(" %d Bytes", item.size());
        }
        item.close();
    }
    dir.close();
}

/**************************************************************************/
/*!
    @brief  count the number of directory level

    @note   Recursive call
*/
/**************************************************************************/
void level_count(const char* cwd, int* level) {
    bool dir_encountered = false;
    if (level == nullptr) {
        printfn("null pointer as level");
        return;
    }
    // Open the input folder
    File dir(cwd, FILE_O_READ, InternalFS);

    // Print root
    if (*level == 0) printfn("root");

    // File within folder
    File item(InternalFS);

    // Loop through the directory

    while ((item = dir.openNextFile(FILE_O_READ))) {
        if (item.isDirectory()) {
            // increment only if is the first dir we meet
            if (!dir_encountered) {
                (*level)++;
                dir_encountered = true;
                printfn("level down %d", *level);
            }
            char dpath[strlen(cwd) + strlen(item.name()) + 2];
            strcpy(dpath, cwd);
            strcat(dpath, "/");
            strcat(dpath, item.name());
            level_count(dpath, level);
        }
        item.close();
    }
    dir.close();
}

char *commands [] = {"fshelp", "fsls", "fsrm:<path>", "fstouch:<path>", "format", nullptr};
void fs_help() {
    int i = 0;
    while(commands[i] != nullptr) {
        printfn("%s",commands[i++]);
    }
}


void listFile() {
    int dept = 0;
    level_count("/", &dept);
    printfn("dirs dept = %d", dept);

    printTreeDir("/", 0);
    printfn("");
}

/**************************************************************************/
/*!
    @brief  remove a file or dir inside the fs
*/
/**************************************************************************/
boolean fs_remove_item(char* path) {
    if (path == nullptr) {
        printfn("null path");
        return false;
    }
    boolean res = false;
    if (!InternalFS.exists(path)) {
        printfn("not found '%s'", path);
    } else {
        File f = InternalFS.open(path, FILE_O_READ);
        boolean isDir = f.isDirectory();
        f.close();
        if (isDir) {
            printfn("removing dir '%s'", path);
            res = InternalFS.rmdir_r(path);
        } else {
            printfn("removing file '%s'", path);
            res = InternalFS.remove(path);
        }
    }
    return res;
}

/**************************************************************************/
/*!
    @brief  toucch a file or dir inside the fs
*/
/**************************************************************************/
boolean _fs_touch_item(char* path) {
    if (path == nullptr || strlen(path)==0) {
        printfn("invalid path");
        return false;
    }

    if (InternalFS.exists(path)) {
        printfn("File already present");
        return false;
    }

    if (path[strlen(path)-1] == '/') {
        printfn("touching directory '%s'", path);
        delay(1000);
        return InternalFS.mkdir(path);
    } else {
        printfn("touching file '%s'", path);
        delay(1000);
        File f = InternalFS.open(path, FILE_O_WRITE);
        if (InternalFS.exists(path)){
            f.close();
            return true;
        }
    }
    return false;
}

boolean fs_touch_item(char *path) {
    boolean res = _fs_touch_item(path);
    return res;
}
boolean format_fs() {
    boolean res = InternalFS.format();
    return res;
}
