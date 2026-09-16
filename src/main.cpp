#include "dump.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

static void usage()
{
    std::fputs(R"(
  rune — hex dump

  usage:
    rune [file] [flags]
    cat file | rune [flags]

  flags:
    -w <n>    bytes per row (default: 16)
    -s <n>    skip first n bytes
    -n <n>    read at most n bytes
    -C        disable color

  examples:
    rune file.bin
    rune -w 8 -n 256 file.bin
    cat /dev/urandom | rune -n 64

)", stderr);
}

int main(int argc, char* argv[])
{
    int    width  = 16;
    size_t skip   = 0;
    size_t maxlen = SIZE_MAX;
    bool   color  = true;
    const char* path = nullptr;

    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        if      (a == "-w") width  = std::stoi(argv[++i]);
        else if (a == "-s") skip   = std::stoull(argv[++i]);
        else if (a == "-n") maxlen = std::stoull(argv[++i]);
        else if (a == "-C") color  = false;
        else if (a == "-h" || a == "--help") { usage(); return 0; }
        else if (a[0] != '-') path = argv[i];
        else { std::fprintf(stderr, "  unknown flag: %s\n", argv[i]); return 1; }
    }

    std::vector<uint8_t> buf;

    if (path) {
        std::ifstream f(path, std::ios::binary | std::ios::ate);
        if (!f) { std::fprintf(stderr, "  cannot open: %s\n", path); return 1; }
        size_t fsz = f.tellg();
        size_t start = std::min(skip, fsz);
        size_t len   = std::min(fsz - start, maxlen);
        f.seekg(start);
        buf.resize(len);
        f.read(reinterpret_cast<char*>(buf.data()), len);
        buf.resize(f.gcount());
    } else {
        // drain skip bytes
        for (size_t done = 0; done < skip; ) {
            char tmp[4096];
            size_t want = std::min(sizeof(tmp), skip - done);
            size_t got  = std::fread(tmp, 1, want, stdin);
            if (!got) break;
            done += got;
        }
        // read up to maxlen
        char chunk[65536];
        while (buf.size() < maxlen) {
            size_t want = std::min(sizeof(chunk), maxlen - buf.size());
            size_t got  = std::fread(chunk, 1, want, stdin);
            if (!got) break;
            buf.insert(buf.end(), chunk, chunk + got);
        }
    }

    hex_dump(buf.data(), buf.size(), skip, width, color);
    return 0;
}
