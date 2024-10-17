#include <iostream>
#include <limits>
#include <string>


enum ExitCode {
    SUCCESS = 0,
    OVERFLOW_ERROR = 1,
    INVALID_INPUT_ERROR = 2
};

int main() {
    uint32_t sum = 0;
    const uint32_t STOP_NUMBER = 5312;
    std::string input;

    std::cout << "Enter positive integers (enter 5312 to stop):\n";

    while (true) {
        std::cin >> input;

        try {
            // Преобразуем строку в uint32_t сразу 
            uint32_t number = stoul(input);

            if (number == STOP_NUMBER) {
                break;
            }

            // Проверка на переполнение суммы
            if (sum > std::numeric_limits<uint32_t>::max() - number) {
                std::cout << "Sum exceeds maximum allowed value. Exiting.\n";
                return OVERFLOW_ERROR;
            }

            sum += number;
        } catch (const std::invalid_argument& e) {
            std::cout << "Invalid input format\n";
        } catch (const std::out_of_range& e) {
            std::cout << "Number is out of range\n";
        }
    }

    std::cout << "Sum of entered numbers: " << sum << "\n";
    return SUCCESS;
}