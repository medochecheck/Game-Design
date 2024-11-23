#include "lib.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
    // Получаем размер файла
    size_t file_size = count_fsize("onegin.txt");
    if (file_size == null_filename || file_size == err_open) {
        return (int)file_size;
    }

    // Выделяем память для текста
    char* text_buf = (char *)calloc(1, file_size + 1);  
    if (!text_buf) {
        printf("Error: Memory allocation failed.\n");
        return err_memory;
    }

    // Открываем файл и копируем его содержимое в буфер
    FILE *input = fopen("onegin.txt", "rb");
    if (!input) {
        printf("Error: Could not open file.\n");
        free(text_buf);
        return err_open;
    }
    fread(text_buf, 1, file_size, input);  // Считываем файл в буфер
    fclose(input);

    // Удаляем лишние переносы строк
    enum PreprocessStatus status = preprocess(text_buf);
    if (status == err_null) {
        printf("Error: text_buf is NULL.\n");
        free(text_buf);
        return err_memory;
    }

    size_t str_count = 0;
    for (size_t i = 0; text_buf[i] != '\0'; ++i) {
        if (text_buf[i] == '\n') {
            str_count++;
        }
    }

    // Выделяем память для массива строк
    char **text = (char**)calloc(str_count, sizeof(char*));
    if (!text) {
        printf("Error: Memory allocation for text array failed.\n");
        free(text_buf);
        return err_memory;
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

    return exit_ok;
}