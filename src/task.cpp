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
    if(!strcmp("s", pTask.pThis->host_info.tx_rx_info.unit.c_str())) {
        pTask.pThis->timer_type = 1;
    }
    else if(!strcmp("ms", pTask.pThis->host_info.tx_rx_info.unit.c_str())) {
        pTask.pThis->timer_type = 2;
    }
    else if(!strcmp("us", pTask.pThis->host_info.tx_rx_info.unit.c_str())) {
        pTask.pThis->timer_type = 3;
    }
    else {
        std::cout << "Only s/ms/us time unit is supported" << std::endl;
        exit(-1);
    }

    uint32_t type = 0;
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

    int32_t rlen = 0;
    static char rbf[MAX_BUFF_LEN] = {0};
    if(!strcmp("udp", pTask.pThis->host_info.socket_info.protocol.c_str())) {
        std::shared_ptr<UdpSock> sock = make_shared<UdpSock>(pTask.pThis->host_info);
        sock->SocketInit();

        while(true) {
            switch(type) {
                case 1:
                    sock->SocketSend(NULL, 0);
                    pTask.pThis->task_sleep();
                    break;
                case 2:
                    sock->SocketRecv(NULL, rlen);
                    break;
                case 3:
                    sock->SocketSend(NULL, 0);
                    pTask.pThis->task_sleep();
                    sock->SocketRecv(NULL, rlen);
                    break;
                case 4:
                    if(!strcmp("opposite", pTask.pThis->host_info.tx_rx_info.property.in2out.c_str())) {
                        sock->SocketRecv(rbf, rlen);
                        sock->SocketSend(rbf, rlen);
                    }
                    else
                    {
                        sock->SocketRecv(NULL, rlen);
                        memset(rbf, 'D', pTask.pThis->host_info.tx_rx_info.data.single_len);
                        sock->SocketSend(rbf, pTask.pThis->host_info.tx_rx_info.data.single_len);
                    }
                    break;
                default:
                    std::cout << "Unrecognized configuration!!!" << std::endl;
            }
        }
    }
    else
    {
        std::shared_ptr<TcpSock> sock = make_shared<TcpSock>(pTask.pThis->host_info);
        sock->SocketInit();

        while(true) {
            switch(type) {
                case 1:
                    sock->SocketSend(NULL, 0);
                    pTask.pThis->task_sleep();
                    break;
                case 2:
                    sock->SocketRecv(NULL, rlen);
                    break;
                case 3:
                    sock->SocketSend(NULL, 0);
                    pTask.pThis->task_sleep();
                    sock->SocketRecv(NULL, rlen);
                    break;
                case 4:
                    if(!strcmp("opposite", pTask.pThis->host_info.tx_rx_info.property.in2out.c_str())) {
                        sock->SocketRecv(rbf, rlen);
                        sock->SocketSend(rbf, rlen);
                    }
                    else
                    {
                        sock->SocketRecv(NULL, rlen);
                        memset(rbf, 'C', pTask.pThis->host_info.tx_rx_info.data.single_len);
                        sock->SocketSend(rbf, pTask.pThis->host_info.tx_rx_info.data.single_len);
                    }
                    break;
                default:
                    std::cout << "Unrecognized configuration!!!" << std::endl;
            }
        }
    }
}

void ThreadProcessor::task_sleep() {
    switch(timer_type) {
        case 1:
            sleep(host_info.tx_rx_info.interval);
            break;
        case 2:
            usleep(host_info.tx_rx_info.interval*1000);
            break;
        case 3:
            usleep(host_info.tx_rx_info.interval);
            break;
        default:
            std::cout << "Timer type error." << std::endl;
    }
}