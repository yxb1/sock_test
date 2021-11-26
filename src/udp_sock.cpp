#include "udp_sock.h"

UdpSock::UdpSock(TaskInfo &info) {
    host_info = info;
    if((!strcmp("file", host_info.tx_rx_info.data.from.c_str()) && !strcmp("just_send", host_info.tx_rx_info.property.type.c_str())) 
    || (!strcmp("file", host_info.tx_rx_info.data.from.c_str()) && !strcmp("send_recv", host_info.tx_rx_info.property.type.c_str()) && !strcmp("server", host_info.socket_info.identify.c_str()))
    || (!strcmp("file", host_info.tx_rx_info.data.from.c_str()) && !strcmp("just_recv", host_info.tx_rx_info.property.type.c_str()))) {
        file = new FileHandle(info);
    }
    else
    {
        file = NULL;
    }

    if(!strcmp("file", host_info.tx_rx_info.data.from.c_str()) && !strcmp("send_recv", host_info.tx_rx_info.property.type.c_str()) && !strcmp("server", host_info.socket_info.identify.c_str()))
    {
        ser_file = true;
    }
    else
    {
        ser_file = false;
    }
}

UdpSock::~UdpSock() {
    close(sock_fd);
    if(file) delete file;
}

void UdpSock::SocketInit() {
    sock_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock_fd < 0) {
        std::cout << "Socket create fail. " << strerror(errno) << std::endl;
        exit(-1);
    }

    struct sockaddr_in bind_addr;
    memset(&bind_addr, 0, sizeof(bind_addr));
    if(!strcmp("server", host_info.socket_info.identify.c_str())) {
        bind_addr.sin_family = AF_INET;
        bind_addr.sin_port = htons(host_info.socket_info.ser_port);
        bind_addr.sin_addr.s_addr = inet_addr(host_info.socket_info.ser_ip.c_str());

        _addr.sin_family = AF_INET;
        _addr.sin_port = htons(host_info.socket_info.cli_port);
        _addr.sin_addr.s_addr = inet_addr(host_info.socket_info.cli_ip.c_str());
    }
    else
    {
        bind_addr.sin_family = AF_INET;
        bind_addr.sin_port = htons(host_info.socket_info.cli_port);
        bind_addr.sin_addr.s_addr = inet_addr(host_info.socket_info.cli_ip.c_str());

        _addr.sin_family = AF_INET;
        _addr.sin_port = htons(host_info.socket_info.ser_port);
        _addr.sin_addr.s_addr = inet_addr(host_info.socket_info.ser_ip.c_str());
    }

    int ret = bind(sock_fd, (sockaddr*)&bind_addr, sizeof(bind_addr));
    if(sock_fd < 0) {
        std::cout << "Bind fail" << strerror(errno) << std::endl;
        exit(-1);
    }
}

void UdpSock::SocketSend(char *ptr, int ss_len) {
    static char s_buff[MAX_BUFF_LEN] = {0};
    static uint32_t session_id = 0;
    int s_len = 0;

    if(host_info.tx_rx_info.data.single_len > MAX_BUFF_LEN) {
        std::cout << "Oversize when send, MAX_BUFF_LEN is " << MAX_BUFF_LEN << std::endl;
        exit(-1);
    }

    if(ptr == NULL) {
        if(file) {
            file->ReadFile(s_buff, s_len);
        }
        else
        {
            memset(s_buff, 'Y', host_info.tx_rx_info.data.single_len);
            s_buff[0] = session_id >> 24;
            s_buff[1] = session_id >> 16;
            s_buff[2] = session_id >> 8;
            s_buff[4] = (char)session_id;
        }    
    }
    else
    {
        memcpy(s_buff, ptr, ss_len);
    }

    int real_send = sendto(sock_fd, s_buff, host_info.tx_rx_info.data.single_len, 0, (struct sockaddr *)&_addr, sizeof(_addr));
    if(real_send < 0) {
        std::cout << "real_send is " << real_send << ". " << strerror(errno) << std::endl;
    }
    else
    {
        std::cout << "real_send is " << real_send << std::endl;
    }
}

int UdpSock::SocketRecv(char *ptr, int rr_len) {
    static char r_buff[MAX_BUFF_LEN] = {0};
    struct sockaddr_in src_addr;
	socklen_t src_addr_len = 0;

    int r_len = recvfrom(sock_fd, r_buff, MAX_BUFF_LEN, 0, (sockaddr*)&src_addr, &src_addr_len);
    if(r_len < 0) {
        std::cout << "real_recv is " << r_len << ". " << strerror(errno) << std::endl;
    }
    else
    {
        static uint32_t lost = 0;
        if(!strcmp("data", host_info.tx_rx_info.data.from.c_str())) {
            bool _first = true;
            static uint32_t last = 0;
            static uint32_t curr = 0;

            if(_first) {
                last = (r_buff[0] << 24) | (r_buff[1] << 16) | (r_buff[2] << 8) | r_buff[3];
                curr = last;
                _first = false;
            }
            else
            {
                curr = (r_buff[0] << 24) | (r_buff[1] << 16) | (r_buff[2] << 8) | r_buff[3];
                lost += (curr - last - 1);
                last = curr;
            }
            std::cout << "real_recv is " << r_len << "; lost is " << lost << std::endl;
        }
        else
        {
            std::cout << "real_recv is " << r_len << std::endl;
        }

        if(file && !ser_file) {
            file->WriteFile(r_buff, r_len);
        }

        if(ptr != NULL) {
            memcpy(ptr, r_buff, r_len);
            rr_len = r_len;
        }
    }
}
