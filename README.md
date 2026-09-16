# rune

hex dump with color. address column, hex bytes, ASCII.

```
rune file.bin
```
```
  00000000  89 50 4e 47  0d 0a 1a 0a  00 00 00 0d  49 48 44 52  |.PNG........IHDR|
  00000010  00 00 00 10  00 00 00 10  08 02 00 00  00 90 91 68  |...............h|
```

zero bytes are dimmed blue. printable bytes are cyan in hex, green in ASCII.

## usage

```
rune [file] [flags]
cat file | rune [flags]

  -w <n>    bytes per row (default: 16)
  -s <n>    skip first n bytes
  -n <n>    read at most n bytes
  -C        disable color
```

## examples

```
rune binary.dat
rune -w 8 -n 256 file.bin
rune -s 512 archive.bin
cat /dev/urandom | rune -n 128
```

## build

```
cmake -B build && cmake --build build
```

C++20, no dependencies.
