#include "task.h"

typedef struct
{
	ThreadProcessor *pThis;
}task_t;
static task_t pTask;

ThreadProcessor::ThreadProcessor(TaskInfo &info) {
    host_info = info;
}

ThreadProcessor::~ThreadProcessor() {
    if(task.joinable()) task.join();
}

void ThreadProcessor::task_start() {
    task= std::thread(&ThreadProcessor::task_run, this);
}

void ThreadProcessor::task_run(void *args) {
    pTask.pThis = (ThreadProcessor *)args;
    std::shared_ptr<UdpSock> sock = make_shared<UdpSock>(pTask.pThis->host_info);
    sock->SocketInit();
    uint32_t type = 0;
    uint32_t tick = 0;

    if(!strcmp("just_send", pTask.pThis->host_info.tx_rx_info.property.type.c_str())) {
        type = 1;
        std::cout << "Thread type is " << type << std::endl;
    }
    else if(!strcmp("just_recv", pTask.pThis->host_info.tx_rx_info.property.type.c_str())) {
        type = 2;
        std::cout << "Thread type is " << type << std::endl;
    }
    else if(!strcmp("send_recv", pTask.pThis->host_info.tx_rx_info.property.type.c_str()) && !strcmp("server", pTask.pThis->host_info.socket_info.identify.c_str())) {
        type = 3;
        std::cout << "Thread type is " << type << std::endl;
    }
    else if(!strcmp("send_recv", pTask.pThis->host_info.tx_rx_info.property.type.c_str()) && !strcmp("client", pTask.pThis->host_info.socket_info.identify.c_str())) {
        type = 4;
        std::cout << "Thread type is " << type << std::endl;
    }

    if(strcmp("ms", pTask.pThis->host_info.tx_rx_info.unit.c_str())) {
        std::cout << "Only millisecond timing is supported" << std::endl;
        exit(-1);
    }
    else
    {
        tick = pTask.pThis->host_info.tx_rx_info.interval*1000;
    }

    static char rbf[MAX_BUFF_LEN] = {0};
    uint32_t rlen = 0;
    while(true) {
        switch(type) {
            case 1:
                sock->SocketSend(NULL, 0);
                usleep(tick);
                break;
            case 2:
                sock->SocketRecv(NULL, 0);
                usleep(tick);
                break;
            case 3:
                sock->SocketSend(NULL, 0);
                usleep(tick);
                sock->SocketRecv(NULL, 0);
                break;
            case 4:
                if(!strcmp("opposite", pTask.pThis->host_info.tx_rx_info.property.in2out.c_str())) {
                    sock->SocketRecv(rbf, rlen);
                    sock->SocketSend(rbf, rlen);
                }
                else
                {
                    sock->SocketRecv(NULL, 0);
                    memset(rbf, 'X', pTask.pThis->host_info.tx_rx_info.data.single_len);
                    sock->SocketSend(rbf, pTask.pThis->host_info.tx_rx_info.data.single_len);
                }
                break;
            default:
                std::cout << "Unrecognized configuration!!!" << std::endl;
        }
    }
}