#include "util.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void _stop(const char* prefix, const char* format, va_list args) {
    char* newFormat = (char*)malloc(sizeof(char) * (strlen(format) + 17));
    strcpy(newFormat, prefix);
    strcat(newFormat, format);
    strcat(newFormat, "\n");
    vfprintf(stderr, newFormat, args);
    free(newFormat);
    exit(0);
}

void runtimeStop(const char* format, ...) {
    va_list args;
    va_start(args, format);
    _stop("runtime error: ", format, args);
}

void parsingStop(const char* format, ...) {
    va_list args;
    va_start(args, format);
    _stop("parsing error: ", format, args);
}

void dbgprintf(char* format, ...) {
#ifdef VERBOSE
    va_list args;
    va_start(args, format);
    vprintf(format, args);
#endif
}