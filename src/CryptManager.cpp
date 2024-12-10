/* cccrypt
 * Simple program encoding and decoding Geometry Dash CCGameManager.dat files
 *
 * Copyright (C) Jacob Nilsson 2023-2024
 * Licensed under the MIT License.
 */
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

#include <zlib.h>

#include "CryptManager.hpp"

CryptManager::bytes CryptManager::apply_xor(const bytes& data, uint8_t key) {
    bytes decrypted;
    decrypted.reserve(data.size());

    for (uint8_t byte : data) {
        decrypted.push_back(byte ^ key);
    }

    return decrypted;
}

std::string CryptManager::encode_base64(const bytes& data) {
    static constexpr std::string_view base64_a = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    std::stringstream ret;
    size_t len = data.size();
    size_t i = 0;

    while (i < len) {
        uint32_t a = i < len ? static_cast<uint8_t>(data[i++]) : 0;
        uint32_t b = i < len ? static_cast<uint8_t>(data[i++]) : 0;
        uint32_t c = i < len ? static_cast<uint8_t>(data[i++]) : 0;

        uint32_t triple = (a << 16) + (b << 8) + c;

        ret << base64_a[(triple >> 18) & 0x3F]
                << base64_a[(triple >> 12) & 0x3F]
                << base64_a[(triple >> 6) & 0x3F]
                << base64_a[triple & 0x3F];
    }

    size_t pad = len % 3;
    if (pad) {
        ret.seekp(-static_cast<int>(3 - pad), std::ios_base::end);
        for (size_t j = 0; j < 3 - pad; ++j) {
            ret << '=';
        }
    }

    return ret.str();
}

CryptManager::bytes CryptManager::decode_base64(const std::string_view& data) {
    static constexpr std::string_view base64_a = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    bytes ret;
    int three{};
    uint32_t four{};

    for (char c : data) {
        if (std::isspace(c)) {
            continue;
        }

        if ((c == '=') || (c == '\0')) {
            break;
        }

        if (std::isalnum(c) || (c == '-') || (c == '_')) {
            four = (four << 6) | base64_a.find(c);
            three += 6;

            if (three >= 8) {
                three -= 8;
                ret.push_back(static_cast<uint8_t>((four >> three) & 0xFF));
            }
        } else {
            throw std::runtime_error{"invalid string"};
        }
    }

    return ret;
}

CryptManager::bytes CryptManager::compress_gzip(const bytes& data) {
    z_stream zs{};

    if (deflateInit2(&zs, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 16 + MAX_WBITS, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        throw std::runtime_error{"failed to deflate"};
    }

    zs.next_in = const_cast<Bytef*>(data.data());
    zs.avail_in = static_cast<uInt>(data.size());

    int status;
    bytes buffer(4096);
    bytes ret;

    do {
        zs.next_out = buffer.data();
        zs.avail_out = static_cast<uInt>(buffer.size());

        status = deflate(&zs, Z_FINISH);

        if (ret.size() < zs.total_out) {
            std::copy(buffer.begin(), buffer.begin() + static_cast<long>(zs.total_out - ret.size()), std::back_inserter(ret));
        }

    } while (status == Z_OK);

    deflateEnd(&zs);

    if (status != Z_STREAM_END) {
        throw std::runtime_error{"failed to compress"};
    }

    return ret;
}

CryptManager::bytes CryptManager::decompress_gzip(const bytes& data) {
    z_stream zs{};

    if (inflateInit2(&zs, 16 + MAX_WBITS) != Z_OK) {
        throw std::runtime_error{"failed to inflate"};
    }

    zs.next_in = const_cast<Bytef*>(data.data());
    zs.avail_in = static_cast<uInt>(data.size());

    int status;
    bytes buffer(4096);
    bytes ret;

    do {
        zs.next_out = buffer.data();
        zs.avail_out = static_cast<uInt>(buffer.size());

        status = inflate(&zs, Z_NO_FLUSH);

        if (ret.size() < zs.total_out) {
            std::copy(buffer.begin(), buffer.begin() + static_cast<long>(zs.total_out - ret.size()), std::back_inserter(ret));
        }

    } while (status == Z_OK);

    inflateEnd(&zs);

    if (status != Z_STREAM_END) {
        throw std::runtime_error{"failed to decompress"};
    }

    return ret;
}