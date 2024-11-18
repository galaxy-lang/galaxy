#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#ifdef _WIN32
    #define fopen_safe(file, path, mode) \
        ((fopen_s(&(file), path, mode) == 0) && (file != NULL))
#else
    #define fopen_safe(file, path, mode) \
        (((file) = fopen(path, mode)) != NULL)
#endif

#ifdef _WIN32
#define strdup _strdup
#endif

#endif // UTILS_H