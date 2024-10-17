#include <iostream>
#include <limits>
#include <string>

using namespace std;

int main() {
    uint64_t sum = 0;
    const uint32_t STOP_NUMBER = 5312;
    string input;

    cout << "Enter positive integers (enter 5312 to stop):\n";

    while (true) {
        cin >> input;
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

            if (number > numeric_limits<uint32_t>::max()) {
                cout << "Invalid input format\n";
                continue;
            }

            if (number == STOP_NUMBER) {
                break;
            }

            sum += number;
        } else {
            cout << "Invalid input format\n";
        }
    }

    cout << "Sum of entered numbers: " << sum << "\n";
    return 0;
}