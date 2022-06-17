#ifndef LAB5__98__EMPLOYEE_H
#define LAB5__98__EMPLOYEE_H

class Employee{
public:
    Employee() : id(), name(), hours() {}

    friend std::fstream& operator<<(std::fstream& fs, const Employee& emp);
    friend std::fstream& operator>>(std::fstream& fs, Employee& emp);

    int id;
    char name[10];
    double hours;
};

std::fstream &operator<<(std::fstream &fs, const Employee &emp)
{
    fs.write((const char*)&emp, sizeof(Employee));
    return fs;
}

std::fstream &operator>>(std::fstream &fs, Employee &emp)
{
    fs.read((char*)&emp, sizeof(Employee));
    return fs;
}

#endif //LAB5__98__EMPLOYEE_H