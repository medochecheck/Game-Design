#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum ExitStatus {
    exit_ok = 0,            // штатное завершение
    null_filename,  // пустое имя файла
    err_open,       // не удалось открыть файл
    err_memory   // не удалось выделить память
};

enum PreprocessStatus {
    ok = 0,         // Штатное завершение
    err_null = 1    // Ошибка: передан NULL указатель
};

// Функция для получения размера файла
size_t count_fsize(const char* filename) {
    if (!filename) {
        printf("Filename iss NULL.\n");
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
//size_t preprocess(char* text_buf) {
enum PreprocessStatus preprocess(char* text_buf) {
    
    if (!text_buf) {
        return err_null;
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

// Функция для заполнения массива строк
void fill_string_array(char* text_buf, size_t str_count, char** text) {
    if (!text_buf || !text) return; // Проверяем указатели на NULL 
    
    
    size_t j = 0;
    text[0] = text_buf;

    // Разбиваем текст на строки по символу '\n'
    for (size_t i = 0; text_buf[i] != '\0'; ++i) {
        if (text_buf[i] == '\n') {
            text_buf[i] = '\0';  // Заменяем '\n' на '\0', чтобы завершить строку
            j++;
            if (j < str_count) {
                text[j] = &text_buf[i + 1];  // Указываем на начало следующей строки
            } else {
                break;
            }
        }
    }
}

// Функция для сравнения строк для сортировки
int compare_strings(const void* a, const void* b) {
    if (!a || !b) return 0;
    
    char* str1 = *((char**)a);
    char* str2 = *((char**)b);

    if (!str1 || !str2) return 0;
    return strcmp(str1, str2);  // Лексикографическое сравнение строк
}

// Функция для вывода текста
void print_text(char** text, size_t str_count) {
    if (!text) return;
    
    for (size_t i = 0; i < str_count; ++i) {
/*        if (strlen(text[i]) > 0) {  // Печатаем только непустые строки
            printf("%s\n", text[i]);
        }
    }
*/
        //if (text[i]) {printf("%s\n", text[i]);}
        if (text[i]) {
            printf("%s\n", text[i]);
        } else {
            printf("(null)\n");
        }
    }
}

int main() {
    // Получаем размер файла
    size_t file_size = count_fsize("yevgeniy_onegin.txt");
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
    FILE *input = fopen("yevgeniy_onegin.txt", "rb");
    if (!input) {
        printf("Error: Could not open file.\n");
        free(text_buf);
        return err_open;
    }
    fread(text_buf, 1, file_size, input);  // Считываем файл в буфер
    fclose(input);

    // Удаляем лишние переносы строк
    //size_t str_count = preprocess(text_buf);

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
