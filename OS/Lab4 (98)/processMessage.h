//
// Created by amayn on 12.06.2022.
//

#include <string>
#include <cstring>
#include <fstream>

#ifndef LAB4__98__PROCESSMESSAGE_H
#define LAB4__98__PROCESSMESSAGE_H

#endif //LAB4__98__PROCESSMESSAGE_H

class ProcessMessage{
public:
    ProcessMessage() : message() {};
    ProcessMessage(std::string& str)
    {
        strcpy(message, str.c_str());
    }

    char* getString()
    {
        return message;
    }

    friend std::ofstream& operator<<(std::ofstream& ofs, const ProcessMessage& processMessage);
    friend std::ifstream& operator>>(std::ifstream& ifs, ProcessMessage& processMessage);

private:
    char message[20];
};

std::ofstream &operator<<(std::ofstream &ofs, const ProcessMessage &processMessage)
{
    ofs.write((const char*)&processMessage, sizeof(ProcessMessage));
    return ofs;
}

std::ifstream &operator>>(std::ifstream &ifs, ProcessMessage &processMessage)
{
    ifs.read((char*)& processMessage, sizeof(ProcessMessage));
    return ifs;
}
