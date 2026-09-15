#ifndef STRING_H
#define STRING_H

#include <stddef.h>

/* 
 * Although called string.h, it also contains memory functions like memcpy, memset, etc.
 */


void *memcpy(void *dest, const void *src, size_t n);

#endif