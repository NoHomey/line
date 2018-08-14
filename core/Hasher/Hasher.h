#pragma once

#include <utility>
#include <iostream>
#include <openssl/sha.h>
#include "../../utils/types/ComparsionResult.h"

namespace line {

namespace core {

class Hasher {
public:
    class Hash;

public:
    static Hash hash(std::istream& input);

    static Hash hashFile(const char* filePath);

public:
class Hash {
public:
    Hash() noexcept = delete;

    Hash(unsigned char* hash) noexcept;

    Hash(const Hash& other) noexcept = default;

    Hash(Hash&& other) noexcept = default;

    ~Hash() noexcept = default;

    Hash& operator=(const Hash& other) noexcept = default;

    Hash& operator=(Hash&& other) noexcept = default;

public:
    const unsigned char* hash() const noexcept;

    bool operator==(const Hash& other) const noexcept;

    bool operator!=(const Hash& other) const noexcept;

    line::utils::types::ComparsionResult compare(const Hash& other) const noexcept;

    void print(std::ostream& output);

private:
    static char bitsToHex(unsigned char bits) noexcept;

    static std::pair<char, char> toHex(unsigned char data) noexcept;

private:
    unsigned char hashCode[SHA256_DIGEST_LENGTH];
};

private:
    static const unsigned int inputBufferSize = 4096;

    static unsigned char hashCode[SHA256_DIGEST_LENGTH];

    static char inputBuffer[inputBufferSize];
};

}

}