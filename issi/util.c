#include "util.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void stopHard(char* format, ...) {
    va_list args;
    va_start(args, format);
    char* newFormat = (char*)malloc(sizeof(char) * (strlen(format) + 17));
    strcpy(newFormat, "runtime error: ");
    strcat(newFormat, format);
    strcat(newFormat, "\n");
    fprintf(stderr, newFormat, args);
    free(newFormat);
    exit(0);
}