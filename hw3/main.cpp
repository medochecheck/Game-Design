#include <iostream>
#include <limits>
#include <string>

int main() {
    uint64_t sum = 0;
    const uint32_t STOP_NUMBER = 5312;
    std::string input;

    std::cout << "Enter positive integers (enter 5312 to stop):\n";

    while (true) {
        std::cin >> input;

        try {
            // Попытка преобразовать введенное значение в число
            uint64_t number = std::stoull(input);

            // Проверка на корректный диапазон (до uint32_t)
            if (number > std::numeric_limits<uint32_t>::max()) {
                std::cout << "Invalid input format\n";
                continue;
            }

            // Проверка на завершение ввода
            if (number == STOP_NUMBER) {
                break;
            }

            sum += number;
        } catch (const std::invalid_argument& e) {
            // Если ввод нельзя преобразовать в число
            std::cout << "Invalid input format\n";
        } catch (const std::out_of_range& e) {
            // Если число превышает допустимый диапазон
            std::cout << "Invalid input format\n";
        }
    }

    std::cout << "Sum of entered numbers: " << sum << "\n";
    return 0;
}