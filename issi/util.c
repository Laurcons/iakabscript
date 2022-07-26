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
    vfprintf(stderr, newFormat, args);
    free(newFormat);
    exit(0);
}

void dbgprintf(char* format, ...) {
#ifdef VERBOSE
    va_list args;
    va_start(args, format);
    vprintf(format, args);
#endif
}