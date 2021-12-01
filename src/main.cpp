#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include <memory.h>
#include <unistd.h>
#include <stdio.h>

#include "parse_xml.h"
#include "data_type.h"
#include "udp_sock.h"
#include "file_handle.h"
#include "task.h"

using namespace std;
TaskInfo test[10];
int main(int argc, const char *argv[]) {
    std::string path = "../xml/task_info.xml";
    if(argc > 1) {
        path = argv[1];
        std::cout << "Configure file path is " << path.c_str() << std::endl;
    }
    else
    {
        std::cout << "Error, please specify the configuration file path." << std::endl;
        exit(-1);
    }
    std::shared_ptr<ParseXML> ptr = make_shared<ParseXML>();
    uint8_t num = ptr->xml_parser(path.c_str(), test);
    std::shared_ptr<ThreadProcessor> task[10];
    for(int i = 0; i < num; i++) {
        task[i] = make_shared<ThreadProcessor>(test[i]);
        task[i]->task_start();
        usleep(50*1000);
    }
    while(true) {
        sleep(10);
    }
}