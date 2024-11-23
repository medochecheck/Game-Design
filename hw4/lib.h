#ifndef LIB_H
#define LIB_H
#include <stddef.h>  

enum ExitStatus {
    exit_ok = 0,
    null_filename,
    err_open,
    err_memory
};

enum PreprocessStatus {
    ok = 0,
    err_null = 1
};

size_t count_fsize(const char* filename);
enum PreprocessStatus preprocess(char* text_buf);
enum ExitStatus fill_string_array(char* text_buf, size_t str_count, char** text);
int compare_strings(const void* a, const void* b);
void print_text(char** text, size_t str_count);

#endif 