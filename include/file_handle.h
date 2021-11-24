#include <fstream>
#include <iostream>

#ifndef _FILE_HANDLE_HEAD_
#define _FILE_HANDLE_HEAD_

#include "data_type.h"
using namespace std;
class FileHandle {
public:
    FileHandle(TaskInfo &info);
    ~FileHandle();
    void FileReadInit();
    void FileWriteInit();
    int ReadFile(char *ptr, int &rlen);
    int WriteFile(char *ptr, uint32_t wlen);

private:
    TaskInfo host_info;
    fstream file_read;
    fstream file_write;
    bool r_enable;
};

#endif
