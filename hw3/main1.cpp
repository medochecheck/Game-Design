#include <iostream>
#include <limits>
#include <string>
#include <stdexcept>

uint32_t string_to_uint32_t(const std::string &str) {
    if (str.empty()) {
        throw std::invalid_argument("Input string is empty");
    }

    uint32_t result = 0;
    for (char c : str) {
        if (!std::isdigit(c)) {
            throw std::invalid_argument("Invalid character in input string");
        }
        uint32_t digit = c - '0';
        if (result > (std::numeric_limits<uint32_t>::max() - digit) / 10) {
            throw std::out_of_range("Number is out of range for uint32_t");
        }
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
        bool isValid = true;

        // все ли символы в строке - цифры?
        for (char c : input) {
            if (!isdigit(c)) {
                isValid = false;
                break;
            }
        }

        if (isValid) {
            uint64_t number = stoull(input);

            if (number > std::numeric_limits<uint32_t>::max()) {
                std::cout << "Invalid input format\n";
                continue;
            }

            if (number == STOP_NUMBER) {
                break;
            }

            sum += number;
        } else {
            std::cout << "Invalid input format\n";
        }
    }

    std::cout << "Sum of entered numbers: " << sum << "\n";
    return 0;
}