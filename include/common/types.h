#ifndef __MANGOOS__COMMON__TYPES_H
#define __MANGOOS__COMMON__TYPES_H

typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int int64_t;
typedef unsigned long long int uint64_t;

typedef const char *string;
typedef uint32_t size_t;

#ifndef __cplusplus
typedef unsigned char bool;
static const bool False = 0;
static const bool True = 1;
#endif

#endif