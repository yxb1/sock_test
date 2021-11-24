#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include <memory.h>
#include <unistd.h>
#include <stdio.h>

#ifndef _COMMON_DATA_TYPE_HEAD_
#define _COMMON_DATA_TYPE_HEAD_

struct SocketInfo {
    uint32_t dst_port;
    uint32_t src_port;
    std::string dst_ip;
    std::string src_ip;
    std::string protocol;
    std::string identify;
};

struct TRXInfo {
    uint32_t interval;
    std::string unit;
    struct data_info {
        uint32_t single_len;
        std::string from;
        std::string path;
    }data;
    struct fun_property {
        std::string type;
        std::string in2out;
    }property;
};

struct TaskInfo {
    SocketInfo socket_info;
    TRXInfo tx_rx_info;
};

#endif
