#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

// Функция для очистки строки от лишних пробелов в начале и в конце
std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

int main() {
    std::ifstream file("yevgeniy_onegin.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open the file.\n";
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;

    // Чтение строк из файла и очистка от пустых строк
    while (std::getline(file, line)) {
        std::string trimmed_line = trim(line);
        if (!trimmed_line.empty()) {
            lines.push_back(trimmed_line);
        }
    }

    file.close();

    // Сортировка строк в лексикографическом порядке
    std::sort(lines.begin(), lines.end());

    // Вывод отсортированных строк
    std::cout << "Sorted lines:\n";
    for (const auto& sorted_line : lines) {
        std::cout << sorted_line << "\n";
    }

    return 0;
}