#ifndef _PARSE_XML_H_
#define _PARSE_XML_H_

#include <string>
#include "tinyxml.h"
#include "data_type.h"

class ParseXML
{
public:
    ParseXML() = default;
    ~ParseXML() = default;
    int xml_parser(const char *file, TaskInfo *service_info);
};

#endif
