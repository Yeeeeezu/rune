#pragma once
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cctype>
#include <vector>
#include <algorithm>

inline void hex_dump(const uint8_t* data, size_t len,
                     size_t base, int width, bool color)
{
    const char* DIM   = color ? "\x1b[2m"    : "";
    const char* CYAN  = color ? "\x1b[36m"   : "";
    const char* GREEN = color ? "\x1b[32m"   : "";
    const char* DARK  = color ? "\x1b[2;34m" : "";
    const char* RST   = color ? "\x1b[0m"    : "";

    for (size_t i = 0; i < len; i += width) {
        size_t row = std::min((size_t)width, len - i);

        std::printf("%s%08zx%s  ", DIM, base + i, RST);

        for (int j = 0; j < width; j++) {
            if (j == width / 2) std::putchar(' ');
            if ((size_t)j < row) {
                uint8_t b = data[i + j];
                const char* c = b == 0 ? DARK : std::isprint(b) ? CYAN : DIM;
                std::printf("%s%02x%s ", c, b, RST);
            } else {
                std::fputs("   ", stdout);
            }
        }

        std::fputs(" |", stdout);
        for (size_t j = 0; j < row; j++) {
            uint8_t b = data[i + j];
            const char* c = std::isprint(b) ? GREEN : DIM;
            std::printf("%s%c%s", c, std::isprint(b) ? (char)b : '.', RST);
        }
        std::puts("|");
    }
}
