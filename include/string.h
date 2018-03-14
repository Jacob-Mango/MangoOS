#ifndef __STRING_H_
#define __STRING_H_

#include <common/types.h>

void atoi(char *str, int *a);

size_t strcrl(char *str, const char what, const char with);
size_t str_begins_with(const char *str, const char *with);
size_t str_backspace(char *str, char c);
size_t strcount(char *str, char c);
size_t strsplit(char *str, char delim);

char* toupper(char *str);
char* tolower(char *str);
char toupper(char str);
char tolower(char str);

size_t strcmp(const char *str1, const char *str2);
void strsplit(char *s1, char delim, size_t &count);
char *strcpy(char *s1, const char *s2);
size_t strlen(const char *str);
size_t strncpy(char *dest, char *src, size_t len);
char *strchr(char *str, int character);
void strspr(char *str, char *buff, size_t sep, short which);

#endif