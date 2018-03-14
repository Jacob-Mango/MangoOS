#include <string.h>

void atoi(char *str, int *a)
{
    int k = 0;
    while (*str)
    {
        k = (k << 3) + (k << 1) + (*str) - '0';
        str++;
    }
    *a = k;
}

size_t strcrl(char *str, const char what, const char with)
{
    size_t i = 0;
    while (str[i] != 0)
    {
        if (str[i] == what)
            str[i] = with;
        i++;
    }
    return i;
}

size_t strcount(char *str, char c)
{
    size_t i = 0;
    while (*str--)
        if (*str == c)
            i++;
    return i;
}

size_t str_backspace(char *str, char c)
{
    size_t i = strlen(str);
    i--;
    while (i)
    {
        i--;
        if (str[i] == c)
        {
            str[i + 1] = 0;
            return 1;
        }
    }
    return 0;
}

size_t strsplit(char *str, char delim)
{
    size_t n = 0;
    uint32_t i = 0;
    while (str[i])
    {
        if (str[i] == delim)
        {
            str[i] = 0;
            n++;
        }
        i++;
    }
    n++;
    return n;
}

size_t str_begins_with(const char *str, const char *with)
{
    size_t j = strlen(with);
    size_t i = 0;
    size_t ret = 1;
    while (with[j] != 0)
    {
        if (str[i] != with[i])
        {
            ret = 0;
            break;
        }
        j--;
        i++;
    }
    return ret;
}

char *toupper(char *str)
{
    char *p = str;
    while (*p != 0)
    {
        if (*p > 96 && *p < 123)
        {
            *p -= 32;
        }
        ++p;
    }
    return p;
}

char *tolower(char *str)
{
    char *p = str;
    while (*p != 0)
    {
        if (*p > 64 && *p < 91)
        {
            *p += 32;
        }
        ++p;
    }
    return p;
}

char toupper(char c)
{
    if (c > 96 && c < 123)
    {
        c -= 32;
    }
    return c;
}

char tolower(char c)
{
    if (c > 64 && c < 91)
    {
        c += 32;
    }
    return c;
}

size_t strcmp(const char *str1, const char *str2)
{
    int res = 0;
    while (!(res = *(unsigned char *)str1 - *(unsigned char *)str2) && *str2)
        ++str1, ++str2;
    if (res < 0)
        res = -1;
    if (res > 0)
        res = 1;
    return res;
}

void strsplit(char *s1, char delim, int &count)
{
    int i = 0;
    int j = 0;
    while (s1[j] != '\0')
    {
        while (s1[i] != delim)
            i++;
        s1[i] = '\0';
        count++;
    }
}

char *strcpy(char *s1, const char *s2)
{
    char *s1_p = s1;
    while (*s1++ = *s2++);
    return s1_p;
}

size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len++]);
    return len;
}

size_t strncpy(char *dest, char *src, size_t len)
{
    char *d, *s;
    size_t val = 0;
    d = dest;
    s = src;
    while (*s != '\0' && len != 0)
    {
        *d++ = *s++;
        len--;
        val++;
    }
    *d++ = '\0';
    return val;
}

char *strchr(char *str, int character)
{
    do
    {
        if (*str == character)
            return (char *)str;
    }
    while (*str++);
    return 0;
}

void strspr(char *str, char *buff, size_t sep, short which)
{
    short counter = 0;
    char *backup = str;
    bool found = false;
    do
    {
        if (*str == sep)  {
            counter++;
            continue;
        }
        if (counter == which)
            found = true;
    }
    while (*str++);
    if (!found)
        return;
    *backup += counter;
    buff = backup;
}