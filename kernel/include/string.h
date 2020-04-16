#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>

inline void* memset(void* buf, int c, int n);
inline void* memcpy(void* dest, const void* src, size_t n);
int strcmp(char *str1, char *str2);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);

int strlen(char *src);


#endif
