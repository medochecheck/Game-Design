#include <iostream>
#include <limits>
#include <string>
#include <stdexcept>

uint32_t string_to_uint32_t(const std::string &str) {
    uint32_t result = 0;
    for (char c : str) {
        if (!std::isdigit(c)) throw std::invalid_argument("Invalid input");
        uint32_t digit = c - '0';
        if (result > (std::numeric_limits<uint32_t>::max() - digit) / 10) 
            throw std::out_of_range("Number out of range");
        result = result * 10 + digit;
    }
    return result;
}

int main() {
    uint64_t sum = 0;
    const uint32_t STOP_NUMBER = 5312;
    std::string input;
    std::cout << "Enter positive integers (enter 5312 to stop):\n";
    while (true) {
        std::cin >> input;
        try {
            uint32_t number = string_to_uint32_t(input);
            if (number == STOP_NUMBER) break;
            sum += number;
        } catch (...) {
            std::cout << "Invalid input format\n";
        }
    }

    std::cout << "Sum of entered numbers: " << sum << "\n";
    return 0;
}