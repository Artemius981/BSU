#include <iostream>
#include <fstream>
#include <cstdlib>
#include "employee.h"

int main(int argc, char* argv[]) {
    if (argc < 2 || argc >= 3)
    {
        std::cout << "Invalid arguments!\n";
        return 0;
    }
    std::ofstream fout(argv[0], std::ios::binary);

    int n = std::atoi(argv[1]);
    employee emp;
    for (int i = 0; i < n; i++)
    {
        std::cout << "Enter employee's id:" << std::endl;
        std::cin >> emp.num;
        std::cout << "Enter employee's name:" << std::endl;
        std::cin.ignore();
        std::cin.getline(emp.name, 10);
        std::cout << "Enter employee's working hours:" << std::endl;
        std::cin >> emp.hours;

        fout.write((char*)&emp, sizeof(employee));
    }
    return 0;
}
