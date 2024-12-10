/* cccrypt
 * Simple program encoding and decoding Geometry Dash CCGameManager.dat files
 *
 * Copyright (C) Jacob Nilsson 2023-2024
 * Licensed under the MIT License.
 */
#pragma once

#include <string_view>
#include <vector>
#include <cstdint>

namespace CryptManager {
	using byte = uint8_t;
    using bytes = std::vector<byte>;

    bytes apply_xor(const bytes& data, const byte key);
    std::string encode_base64(const bytes& data);
    bytes decode_base64(const std::string_view& data);
    bytes compress_gzip(const bytes& data);
    bytes decompress_gzip(const bytes& data);
} // namespace CryptManager
