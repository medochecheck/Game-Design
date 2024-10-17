#include <iostream>
#include <limits>
#include <string>

using namespace std;

int main() {
    uint32_t sum = 0;
    const uint32_t STOP_NUMBER = 5312;
    string input;

    cout << "Enter positive integers (enter 5312 to stop):\n";

    while (true) {
        cin >> input;

        try {
            // Преобразуем строку в uint32_t сразу 
            uint32_t number = stoul(input);

            if (number == STOP_NUMBER) {
                break;
            }

            // Проверка на переполнение суммы
            if (sum > numeric_limits<uint32_t>::max() - number) {
                cout << "Sum exceeds maximum allowed value. Exiting.\n";
                return 1;
            }

            sum += number;
        } catch (const invalid_argument& e) {
            cout << "Invalid input format\n";
        } catch (const out_of_range& e) {
            cout << "Number is out of range\n";
        }
    }

    cout << "Sum of entered numbers: " << sum << "\n";
    return 0;
}