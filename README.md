# cccrypt

Geometry Dash CCGameManager.dat encoder/decoder written in C++

## Note

The purpose of this project is to provide a way to decode and encode this file,
mostly useful for fixing up bugs with the save file, or similar.

This project is not intended to be used for cheating, hacking, or any other malicious purposes.

## Usage

cccrypt allows a CCGameManager.dat to be decoded into XML, modified and then re-encoded into a CCGameManager.dat
with your changes.

To decode on Linux: `cccrypt -i ./CCGameManager.dat -o ./CCGameManager.xml -d`

You can then modify this file like any normal file. When you're done, you encode it.

To encode on Linux: `cccrypt -i ./CCGameManager.xml -o ./CCGameManager.dat -e`

Be careful when editing the file, in some cases Geometry Dash may reject the changes
so please back up the original file.

For more command line parameters, execute `cccrypt -h` from the command line.

## Compile

To compile, you can use the included CMakeLists.txt file.

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Compatibility

It should build and work perfectly fine on Windows and Linux (as the game on Linux runs under WINE). As of now, it does **not** work on macOS due to
differences in the save data.

## Dependencies

- zlib (for gzip compression/decompression)
- C++20 compiler

## License

Copyright (C) 2023-2024 Jacob Nilsson

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.