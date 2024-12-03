#include "gstrdup.h"

/**
 * @brief Implements gstrdup(), a fork of strdup() for Galaxy Project.
 * 
 * This code implements a new more 'safer' version of strdup() for Galaxy Project.
 * It's not a big deal and its quite simple, btw, theres a thousand variations of the same code
 * around the web, so feel free to modify anything if u want.
 * 
 * @param String Returns the byte string pointer duplicated.
 * @return copy The copied string
 */

/**
 * idk if this size is the properly size for this implementation.
 */
#define MAX_GSTRDUP_STRING_LENGHT 1024 * 1024

char *gstrdup(const char *String) {

    size_t size;
    char *copy;

    /**
     * Check if the size of the string is not bigger than the
     * size defined and allocated, avoiding undue memory consumption.
     */

    if (size > MAX_GSTRDUP_STRING_LENGHT) {
        errno = EINVAL;
        return NULL;    
    }

    size = strlen(String) + 1;
    if ((copy = malloc(size)) == NULL) {
        return NULL;
    }
    (void)memcpy(copy, String, size);
    return copy;
}