#include <thread>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>

#ifndef _MY_TASK_HEAD_
#define _MY_TASK_HEAD_

#include "data_type.h"
#include "udp_sock.h"
#include "tcp_sock.h"

class ThreadProcessor {
public:
    ThreadProcessor(TaskInfo &info);
    ~ThreadProcessor();
    void task_start();
    static void task_run(void *args);
    void task_sleep();

private:
    TaskInfo host_info;
    std::thread task;
    uint8_t timer_type;
};

#endif
