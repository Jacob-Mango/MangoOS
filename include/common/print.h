#ifndef __PRINT_H
#define __PRINT_H

#include <common/types.h>

class Console
{
  public:
    static void Initialize();

    static void Print(const char *s);

    static void Hex(uint8_t key);
    static void Hex16(uint16_t key);
    static void Hex32(uint32_t key);

    static void Set(uint16_t x, uint16_t y, const char c);

    static void SetCursor(uint16_t x, uint16_t y);
};

void printf(const char *str);

void printfHex(uint8_t key);
void printfHex16(uint16_t key);
void printfHex32(uint32_t key);

#endif