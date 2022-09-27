#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdio.h>

#define LINE_SIZE 27
#define MAX_FILE_NAME 500

FILE* open_file(const char *path);
char* get_file_name();

#endif // !FILE_HANDLER_H
