#include <common/print.h>

static uint16_t *VideoMemory;

static uint16_t x;
static uint16_t y;

void Console::Initialize()
{
    VideoMemory = (uint16_t *)0xb8000;
}

void Console::SetCursor(uint16_t nx, uint16_t ny)
{
    x = nx;
    y = ny;
}

void Console::Set(uint16_t x, uint16_t y, const char c)
{
    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | c;
}

void Console::Print(const char *str)
{
    for (int i = 0; str[i] != '\0'; ++i)
    {
        switch (str[i])
        {
        case '\n':
            x = 0;
            y++;
            break;
        default:
            Set(x, y, str[i]);
            x++;
            break;
        }

        if (x >= 80)
        {
            x = 0;
            y++;
        }

        if (y >= 25)
        {
            for (uint16_t yi = 0; yi < 24; yi++)
                for (uint16_t xi = 0; xi < 80; xi++)
                    VideoMemory[80 * (yi) + xi] = VideoMemory[80 * (yi + 1) + xi];

            for (uint16_t xi = 0; xi < 80; xi++)
                Set(xi, 24, ' ');

            x = 0;
            y = y - 1;
        }
    }
}

void Console::Hex(uint8_t key)
{
    char *foo = "00";
    char *hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    Print(foo);
}

void Console::Hex16(uint16_t key)
{
    Hex((key >> 8) & 0xFF);
    Hex(key & 0xFF);
}

void Console::Hex32(uint32_t key)
{
    Hex((key >> 24) & 0xFF);
    Hex((key >> 16) & 0xFF);
    Hex((key >> 8) & 0xFF);
    Hex(key & 0xFF);
}

void printf(const char *str)
{
    Console::Print(str);
}

void printfHex(uint8_t key)
{
    Console::Hex(key);
}

void printfHex16(uint16_t key)
{
    Console::Hex16(key);
}

void printfHex32(uint32_t key)
{
    Console::Hex32(key);
}