/* cccrypt
 * Simple program encoding and decoding Geometry Dash CCGameManager.dat files
 *
 * Copyright (C) Jacob Nilsson 2023-2024
 * Licensed under the MIT License.
 */
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include "CryptManager.hpp"

std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data) {
    const auto& encoded = CryptManager::encode_base64(CryptManager::compress_gzip(data));
    return CryptManager::apply_xor(CryptManager::bytes{encoded.begin(), encoded.end()}, 11);
}

std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data) {
    const auto& decoded = CryptManager::apply_xor(data, 11);
    return CryptManager::decompress_gzip(CryptManager::decode_base64(std::string(decoded.begin(), decoded.end())));
}

void help_out() {
#ifdef _WIN32
    std::cout << "usage: cccrypt.exe [/i input] [/o output] [/e] [/d] [/h]\n";
#else
    std::cout << "usage: cccrypt [-i|--input input] [-o|--output output] [-e|--encrypt] [-d|--decrypt] [-h|--help]\n";
#endif
}

int main(int argc, char** argv) {
    enum class Mode {
        Encrypt = false,
        Decrypt = true,
    };

    std::string inf{};
    std::string outf{};

    Mode mode = Mode::Decrypt;

    for (int i{1}; i < argc; i++) {
        const std::string arg{argv[i]};

        if (arg == "-d" || arg == "--decrypt" || arg == "/d") {
            mode = Mode::Decrypt;
        } else if (arg == "-e" || arg == "--encrypt" || arg == "/e") {
            mode = Mode::Encrypt;
        } else if (arg == "-i" || arg == "--input" || arg == "/i") {
            inf = argv[++i];
        } else if (arg == "-o" || arg == "--output" || arg == "/o") {
            outf = argv[++i];
        } else if (arg == "-h" || arg == "--help" || arg == "/h") {
            help_out();
            return EXIT_SUCCESS;
        } else {
            help_out();
            return EXIT_FAILURE;
        }
    }

    if (!inf.compare("")) {
        std::cerr << "cccrypt: no input file was specified.\n";
        return EXIT_FAILURE;
    }

    std::ifstream file(inf, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "cccrypt: failed to open input file\n";
        return EXIT_FAILURE;
    }

    std::vector<uint8_t> data{};

    data = (mode == Mode::Decrypt ?
        decrypt({std::istreambuf_iterator<char>(file), {}})
        : encrypt({std::istreambuf_iterator<char>(file), {}})
        );

    file.close();

    if (outf.empty()) {
        for (uint8_t it : data) {
            std::cout << it;
        }
    } else {
        std::ofstream output;

        output.open(outf);
        output.write(reinterpret_cast<const char*>(data.data()), static_cast<long>(data.size()));
        output.close();
    }

    return EXIT_SUCCESS;
}
