#include <iostream>

int main() {
    int int1 = 8, int2 = 2002;
    char char1 = 'A', char2 = 'B';
    double double1 = 3.14526, double2 = 2.714582;

    std::cout << "int1 + int2 = " << int1 + int2 << std::endl;
    std::cout << "int1 + char1 = " << int1 + char1 << std::endl;
    std::cout << "char1 + char2 = " << char1 + char2 << std::endl;
    std::cout << "double1 + double2 = " << double1 + double2 << std::endl;
    std::cout << "double1 + char1 = " << double1 + char1 << std::endl;
    std::cout << "double1 + int1 = " << double1 + int1 << std::endl;

    return 0;
}
