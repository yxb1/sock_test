#include "file_handle.h"

FileHandle::FileHandle(TaskInfo &info) {
    host_info = info;

    if((!strcmp("file", host_info.tx_rx_info.data.from.c_str()) && !strcmp("just_send", host_info.tx_rx_info.property.type.c_str())) 
    || (!strcmp("send_recv", host_info.tx_rx_info.property.type.c_str()) && !strcmp("file", host_info.tx_rx_info.data.from.c_str()) && !strcmp("server", host_info.socket_info.identify.c_str()))) {
        r_enable = true;
        std::cout << "According to your choice, we will open a file for reading" << std::endl;
        FileReadInit();
    }
    else
    {
        r_enable = false;
        std::cout << "According to your choice, we will open a file for writing" << std::endl;
        FileWriteInit();
    }
}

FileHandle::~FileHandle() {
    if(file_read) file_read.close();
    if(file_write) file_write.close();
}

void FileHandle::FileReadInit() {
    if(!strcmp("file", host_info.tx_rx_info.data.from.c_str()) && !host_info.tx_rx_info.data.path.empty()) {
        file_read.open(host_info.tx_rx_info.data.path.c_str(), ios::in | ios::binary);
        if (!file_read.is_open()) {
            std::cout << "Open file fail when read." << std::endl;
            exit(-1);
            return;
        }
        file_read.clear();
        file_read.seekg(0, std::ios::beg);
    }
    else
    {
        std::cout << "Sorry, data.from or data.path check NG. You can't read or write." << std::endl;
    }
}

void FileHandle::FileWriteInit() {
    if(!strcmp("file", host_info.tx_rx_info.data.from.c_str()) && !host_info.tx_rx_info.data.path.empty()) {
        file_write.open(host_info.tx_rx_info.data.path.c_str(), ios::out | ios::binary);
        if (!file_write.is_open()) {
            std::cout << "Open file fail when write." << std::endl;
            exit(-1);
            return;
        }
        file_write.clear();
        file_write.seekp(0, std::ios::end);
        std::cout << "The file len is " << file_write.tellp() << std::endl;
        file_write.seekp(0, std::ios::beg);
    }
    else
    {
        std::cout << "Sorry, data.from or data.path check NG. You can't read or write." << std::endl;
    }
}

int FileHandle::ReadFile(char *ptr, int &rlen) {
    if(!r_enable) {
        std::cout << "Sorry, r_enable is false when read. Maybe your setting is wrong." << std::endl;
        exit(-1);
    }
    file_read.read(ptr, host_info.tx_rx_info.data.single_len);
    if(file_read.peek() == EOF) {
        file_read.clear();
        file_read.seekg(0, std::ios::beg);
    }
}

int FileHandle::WriteFile(char *ptr, uint32_t wlen) {
    if(r_enable) {
        std::cout << "Sorry, r_enable is true when write. Maybe your setting is wrong." << std::endl;
        exit(-1);
    }
    file_write.write(ptr, wlen);
}