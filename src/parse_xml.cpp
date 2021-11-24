#include "parse_xml.h"

int ParseXML::xml_parser(const char *file, TaskInfo *task_info) {
    uint8_t index = 0; 
    TiXmlDocument doc;
    if (!doc.LoadFile(file)) {
        std::cout << "Load XML file failed. " << doc.ErrorDesc() << std::endl;
        exit(-1);
    }

    TiXmlElement *root = doc.FirstChildElement();
    if (root->NoChildren()) {
        std::cout << "XML file format error, please check it" << std::endl;
        exit(-1);
    }

    TiXmlElement *msg = root->FirstChildElement();
    while(msg) {
        if(!msg->NoChildren()) {
            TiXmlElement *next = msg->FirstChildElement();
            while(next) {
                if(!strcmp("protocol", next->Value())) {
                    task_info[index].socket_info.protocol = next->GetText();
                    //std::cout << "identify: " << next->Attribute("identify") << std::endl;
                    if(!strcmp("server", next->Attribute("identify"))) {
                        task_info[index].socket_info.identify = "server";
                    }else{
                        task_info[index].socket_info.identify = "client";
                    }
                }else if(!strcmp("src_ip", next->Value())) {
                    task_info[index].socket_info.src_ip = next->GetText();
                }else if(!strcmp("src_port", next->Value())) {
                    task_info[index].socket_info.src_port = static_cast<uint32_t>(std::stoi(next->GetText()));
                }else if(!strcmp("dst_ip", next->Value())) {
                    task_info[index].socket_info.dst_ip = next->GetText();
                }else if(!strcmp("dst_port", next->Value())) {
                    task_info[index].socket_info.dst_port = static_cast<uint32_t>(std::stoi(next->GetText()));
                }else if(!strcmp("interval", next->Value())) {
                    task_info[index].tx_rx_info.interval = static_cast<uint32_t>(std::stoi(next->GetText()));
                    task_info[index].tx_rx_info.unit = next->Attribute("unit");
                }else if(!strcmp("data_info", next->Value())) {
                    task_info[index].tx_rx_info.data.from = next->GetText();
                    task_info[index].tx_rx_info.data.single_len = static_cast<uint32_t>(std::stoi(next->Attribute("len")));
                    //std::cout << "single_len: " << task_info[index].tx_rx_info.data.single_len << std::endl;
                    if(!strcmp("file", next->GetText())) {
                        task_info[index].tx_rx_info.data.path = next->Attribute("file_path");
                    }else{
                        task_info[index].tx_rx_info.data.path = "";
                    }
                }else if(!strcmp("fun_property", next->Value())) {
                    task_info[index].tx_rx_info.property.type = next->GetText();
                    if(!strcmp("send_recv", next->GetText())) {
                        task_info[index].tx_rx_info.property.in2out = next->Attribute("dt_src");
                    }else{
                        task_info[index].tx_rx_info.property.in2out = "";
                    }
                }else {
                    std::cout << "Programmer doesn't use " << next->GetText() << " now" << std::endl;
                }
                next = next->NextSiblingElement();
            }
        }

        if(strcmp("task_info", msg->Value())) {
            std::cout << msg->Value() << " : " << msg->GetText() << std::endl;
        }else {
            index++;
        }
        msg = msg->NextSiblingElement();
    }

    for(int i = 0; i < index; i++)
    {
        std::cout << "----------------------------------" << std::endl;
        std::cout << "[---" << i << "---]" << "protocol: " << task_info[i].socket_info.protocol << std::endl;
        std::cout << "[---" << i << "---]" << "identify: " << task_info[i].socket_info.identify << std::endl;
        std::cout << "[---" << i << "---]" << "src_ip: " << task_info[i].socket_info.src_ip << std::endl;
        std::cout << "[---" << i << "---]" << "src_port: " << task_info[i].socket_info.src_port << std::endl;
        std::cout << "[---" << i << "---]" << "dst_ip: " << task_info[i].socket_info.dst_ip << std::endl;
        std::cout << "[---" << i << "---]" << "dst_port: " << task_info[i].socket_info.dst_port << std::endl;
        std::cout << "[---" << i << "---]" << "interval: " << task_info[i].tx_rx_info.interval << std::endl;
        std::cout << "[---" << i << "---]" << "unit: " << task_info[i].tx_rx_info.unit << std::endl; 
        std::cout << "[---" << i << "---]" << "data.single_len: " << task_info[i].tx_rx_info.data.single_len << std::endl;
        std::cout << "[---" << i << "---]" << "data.from: " << task_info[i].tx_rx_info.data.from << std::endl;
        std::cout << "[---" << i << "---]" << "data.path: " << task_info[i].tx_rx_info.data.path << std::endl;
        std::cout << "[---" << i << "---]" << "property.type: " << task_info[i].tx_rx_info.property.type << std::endl;
        std::cout << "[---" << i << "---]" << "property.in2out: " << task_info[i].tx_rx_info.property.in2out << std::endl;
    }

    return index;
}