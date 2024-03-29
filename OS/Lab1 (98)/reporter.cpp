#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include "employee.h"

int main(int argc, char* argv[]) {
    if (argc < 3 || argc >= 4)
    {
        std::cout << "Invalid arguments!\n";
        return 0;
    }
    std::ifstream fin(argv[0], std::ios::binary);
    std::ofstream fout(argv[1]);
    double perHour = std::atof(argv[2]);

    fout << "Report for file " << argv[0] << std::endl;
    employee* emp = new employee();
    while (fin.read((char*)emp, sizeof(employee)))
    {
        fout << std::setw(12) << std::left << emp->num;
        fout << std::setw(11) << std::left << emp->name;
        fout << std::setw(12) << std::left << emp->hours * perHour << std::endl;
    }

    return 0;
}
