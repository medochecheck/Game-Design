#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Функция для получения размера файла
size_t count_fsize(const char* filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Could not open file.\n");
        return 0;
    }
    fseek(file, 0, SEEK_END);  // Перемещаем указатель в конец файла
    size_t size = ftell(file); // Получаем текущий размер файла
    fclose(file);
    return size;
}

// Заменяем несколько \n подряд на одно \n
size_t preprocess(char* text_buf) {
    size_t count = 0;
    int newline_flag = 0;  // Флаг, чтобы не записывать подряд несколько \n

    for (size_t i = 0; text_buf[i] != '\0'; ++i) {
        if (text_buf[i] == '\n') {
            if (!newline_flag) {
                text_buf[count++] = '\n';  // Записываем только одно \n
                newline_flag = 1;
            }
        } else {
            text_buf[count++] = text_buf[i];  // Записываем обычный символ
            newline_flag = 0;
        }
    }
    text_buf[count] = '\0';  // Завершаем строку
    return count;
}

// Функция для заполнения массива строк
void fill_string_array(char* text_buf, size_t str_count, char** text) {
    size_t j = 0;
    text[0] = text_buf;

    // Разбиваем текст на строки по символу '\n'
    for (size_t i = 0; text_buf[i] != '\0'; ++i) {
        if (text_buf[i] == '\n') {
            text_buf[i] = '\0';  // Заменяем '\n' на '\0', чтобы завершить строку
            j++;
            if (j < str_count) {
                text[j] = &text_buf[i + 1];  // Указываем на начало следующей строки
            }
        }
    }
}

// Функция для сравнения строк для сортировки
int compare_strings(const void* a, const void* b) {
    char* str1 = *(char**)a;
    char* str2 = *(char**)b;
    return strcmp(str1, str2);  // Лексикографическое сравнение строк
}

// Функция для вывода текста
void print_text(char** text, size_t str_count) {
    for (size_t i = 0; i < str_count; ++i) {
        if (strlen(text[i]) > 0) {  // Печатаем только непустые строки
            printf("%s\n", text[i]);
        }
    }
}

int main() {
    // Получаем размер файла
    size_t file_size = count_fsize("yevgeniy_onegin.txt");
    if (file_size == 0) return 1;

    // Выделяем память для текста
    char* text_buf = (char*)calloc(1, file_size + 1);  
    if (!text_buf) {
        printf("Error: Memory allocation failed.\n");
        return 1;
    }

    // Открываем файл и копируем его содержимое в буфер
    FILE* input = fopen("yevgeniy_onegin.txt", "rb");
    if (!input) {
        printf("Error: Could not open file.\n");
        free(text_buf);
        return 1;
    }
    fread(text_buf, 1, file_size, input);  // Считываем файл в буфер
    fclose(input);

    // Удаляем лишние переносы строк
    size_t str_count = preprocess(text_buf);

    // Выделяем память для массива строк
    char** text = (char**)calloc(str_count, sizeof(char*));
    if (!text) {
        printf("Error: Memory allocation for text array failed.\n");
        free(text_buf);
        return 1;
    }

    // Заполняем массив строк
    fill_string_array(text_buf, str_count, text);

    // Сортируем массив строк
    qsort(text, str_count, sizeof(char*), compare_strings);

    // Выводим отсортированные строки
    print_text(text, str_count);

    // Освобождаем память
    free(text);
    free(text_buf);

    return 0;
}
