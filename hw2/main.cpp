#include <iostream>

int main() {
    int int1 = 8, int2 = 2002;
    char char1 = 'A', char2 = 'B'; // A = 65, B = 66
    double double1 = 3.14526, double2 = 2.714582;

    std::cout << "int1 + int2 = " << int1 + int2 << std::endl; // int1 + int2 = 8 + 2002 = 2010 (Оба числа целые (int), так что просто сложение)
    std::cout << "int1 + char1 = " << int1 + char1 << std::endl; // int1 + char1 = 8 + 65 = 73 (char1 - это символ 'A', его значение в таблице - 65. При сложении с int, символ преобразуется в число)
    std::cout << "char1 + char2 = " << char1 + char2 << std::endl; // char1 + char2 = 65 + 66 = 131 (Тут оба символа преобразуются в их числовые значения: 'A' = 65 и 'B' = 66. При сложении выводится сумма этих значений)
    std::cout << "double1 + double2 = " << double1 + double2 << std::endl; // double1 + double2 = 3.14526 + 2.714582 = 5.859842 (Оба числа вещественные (double), так что результат будет с плавающей запятой)
    std::cout << "double1 + char1 = " << double1 + char1 << std::endl; // double1 + char1 = 3.14526 + 65 = 68.14526 (char1 ('A') преобразуется в число 65. При сложении с double (3.14526) результат тоже вещественный)
    std::cout << "double1 + int1 = " << double1 + int1 << std::endl; // double1 + int1 = 3.14526 + 8 = 11.14526 (int1 (целое число) преобразуется в вещественное число, и результат также вещественный, потому что один из операндов double)

    return 0;
}

