#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Windows.h>
#include <algorithm>
#include <cstdlib>
#include <locale>
#include <codecvt>

size_t length(char* str) {
    size_t i = 0;
    while (*(str + i) != 0) {
        i++;
    }
    return i;
}

void stringcopy(char* from, char* to) {
    size_t i = 0;
    while (*(from + i) != 0) {
        *(to + i) = *(from + i);
        i++;
    }
    to[i] = 0;
}

int strcompare(char *from, char *to) {
    size_t i = 0;
    while (*(from + i) != 0 && *(to + i) != 0) {
        if (*(from + i) != *(to + i)) {
            return *(from + i) - *(to + i);
        }
        i++;
    }
    return *(from + i) - *(to + i);
}

std::string trim(const std::string& str) 
{
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);

}

int main() {
    std::setlocale(LC_ALL, "Russian");
    std::ifstream file("yevgeniy_onegin.txt");
    if (!file) {
        std::cerr << "Failed to open the file.\n";
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;
    char str[128];
    size_t n = 0;

    while (std::getline(file, line)) 
    {
        std::string trimmed_line = trim(line);
        if (!trimmed_line.empty()) {
            lines.push_back(trimmed_line);
        }
    }

    file.close();

    std::ifstream input("yevgeniy_onegin.txt");
    if (!input) {
        std::cerr << "Failed to reopen the file.\n";
        return 1;
    }

    while (input >> str) {
        std::cout << str << " " << length(str) << "\n";
        n++;
    }
    input.clear();
    input.seekg(0);

    char** txt = (char**)calloc(n, sizeof(char*));
    int i = 0;
    while (input >> str) {
        txt[i] = (char*)calloc(length(str) + 1, sizeof(char));
        stringcopy(str, txt[i]);
        i++;
    }
    input.close();

    std::cout << "Words from the file:\n";
    for (i = 0; i < n; ++i) {
        std::cout << txt[i] << std::endl;
    }

    for (i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (strcompare(txt[i], txt[j]) > 0) {
                char* temp = txt[i];
                txt[i] = txt[j];
                txt[j] = temp;
            }
        }

    }

    std::cout << "Sorted words:\n";
    for (i = 0; i < n; ++i) {
        std::cout << txt[i] << std::endl;
    }

    for (i = 0; i < n; ++i) {
        free(txt[i]);
    }
    free(txt);

    std::sort(lines.begin(), lines.end());

    std::cout << "Sorted lines:\n";
    for (const auto& sorted_line : lines) {
        std::cout << sorted_line << "\n";
    }

    return 0;
}