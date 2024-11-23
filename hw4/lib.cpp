#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Функция для получения размера файла
size_t count_fsize(const char* filename) {
    if (!filename) {
        printf("Filename is NULL.\n");
        return null_filename;
    }
    
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Could not open file.\n");
        return err_open;
    }
    fseek(file, 0, SEEK_END);  // Перемещаем указатель в конец файла
    size_t size = ftell(file); // Получаем текущий размер файла
    fclose(file);
    return size;
}

// Заменяем несколько \n подряд на одно \n
enum PreprocessStatus preprocess(char* text_buf) {
    if (!text_buf) {
        return err_null;  // Если указатель NULL
    }
    
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
    return ok; 
}

// Заполнение массива строк
enum ExitStatus fill_string_array(char* text_buf, size_t str_count, char** text) {
    if (!text_buf || !text) {
        fprintf(stderr, "Error: NULL pointer passed to fill_string_array.\n");
        return err_memory;
    }

    size_t j = 0;
    text[j] = text_buf; // Указываем на начало первой строки
    for (size_t i = 0; text_buf[i] != '\0'; ++i) {
        if (text_buf[i] == '\n') {
            text_buf[i] = '\0'; // Завершаем текущую строку
            j++;
            if (j < str_count) {
                text[j] = &text_buf[i + 1]; // Указываем на начало следующей строки
            } else {
                fprintf(stderr, "Warning: Exceeded maximum number of strings (%zu).\n", str_count);
                return err_memory; 
            }
        }
    }

    for (size_t k = j + 1; k < str_count; ++k) {
        text[k] = NULL;
    }

    return exit_ok;
}

// Сравнение строк для сортировки
int compare_strings(const void* a, const void* b) {
    if (!a || !b) return 0;
    
    char* str1 = *((char**)a);
    char* str2 = *((char**)b);

    if (!str1 || !str2) return 0;
    return strcmp(str1, str2); 
}

// Вывод текста
void print_text(char** text, size_t str_count) {
    if (!text) { 
        fprintf(stderr, "Error: text is NULL.\n");
        return; 
    }

    for (size_t i = 0; i < str_count; ++i) {
        if (text[i]) {  
            printf("%s\n", text[i]);
        } else {
            printf("(null)\n");
        }
    }
}