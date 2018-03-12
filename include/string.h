#ifndef __STRING_H_
#define __STRING_H_

#include <common/types.h>

extern size_t strlen(const char* str);
extern size_t strcmp(const char* str1, const char* str2);

extern void atoi(char *str, int *a);

extern size_t strcrl(char* str, const char what, const char with);
extern size_t str_begins_with(const char* str, const char* with);
extern size_t str_backspace(char* str, char c);
extern size_t strcount(char* str, char c);
extern size_t strsplit(char* str, char delim);

#endif