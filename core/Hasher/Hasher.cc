#include "./Hasher.h"
#include <cassert>
#include <cstring>
#include <fstream>
#include "../../utils/funcs/integerToComparsionResult.thd"

unsigned char line::core::Hasher::hashCode[SHA256_DIGEST_LENGTH];

char line::core::Hasher::inputBuffer[inputBufferSize];

char line::core::Hasher::Hash::hexHashCode[hexHashCodeLength + 1];

line::core::Hasher::Hash line::core::Hasher::hash(std::istream& input) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    input.exceptions(std::istream::badbit);
    while(input) {
        input.read(inputBuffer, inputBufferSize);
        std::size_t readCount = input.gcount();
        if(readCount) {
            SHA256_Update(&sha256, inputBuffer, readCount);
        }
    }
    SHA256_Final(hashCode, &sha256);
    return {hashCode};
}

line::core::Hasher::Hash line::core::Hasher::hashFile(const char* filePath) {
    std::ifstream input;
    input.exceptions(std::ifstream::badbit);
    input.open(filePath);
    return hash(input);
}

line::core::Hasher::Hash::Hash(unsigned char* hash) noexcept {
    assert(hash);
    std::memcpy(hashCode, hash, SHA256_DIGEST_LENGTH);
}

line::core::String::StringSlice line::core::Hasher::Hash::toHexHashCode(const Hash& hash) noexcept {
    for(unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        std::pair<char, char> hex = toHex(hash.hashCode[i]);
        hexHashCode[2 * i] = hex.first;
        hexHashCode[2 * i + 1] = hex.second;
    }
    hexHashCode[hexHashCodeLength] = '\0';
    return {hexHashCode, hexHashCodeLength};
}

bool line::core::Hasher::Hash::isValidHexHashCode(const line::core::String::StringSlice& hexHashCode) noexcept {
    if(hexHashCode.count != hexHashCodeLength) {
        return false;
    }
    for(unsigned int i = 0; i < hexHashCodeLength; i++) {
        if(!isHexSymbol(hexHashCode.beginning[i])) {
            return false;
        }
    }
    return true;
}

line::core::Hasher::Hash line::core::Hasher::Hash::fromHexHashCode(const line::core::String::StringSlice& hexHashCode) noexcept {
    assert(hexHashCode.count == hexHashCodeLength);
    for(unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        line::core::Hasher::hashCode[i] =
            (hexDigitToBits(hexHashCode.beginning[2 * i]) << 4)
            |
            hexDigitToBits(hexHashCode.beginning[2 * i + 1]);
    }
    return {line::core::Hasher::hashCode};
}

const unsigned char* line::core::Hasher::Hash::hash() const noexcept {
    return hashCode;
}

bool line::core::Hasher::Hash::operator==(const Hash& other) const noexcept {
    return compare(other) == line::utils::types::ComparsionResult::Equal;
}

bool line::core::Hasher::Hash::operator!=(const Hash& other) const noexcept {
    return !(*this == other);
}

line::utils::types::ComparsionResult line::core::Hasher::Hash::compare(const Hash& other) const noexcept {
    return line::utils::funcs::integerToComparsionResult(std::memcmp(hashCode, other.hashCode, SHA256_DIGEST_LENGTH));
}

 char line::core::Hasher::Hash::bitsToHex(unsigned char bits) noexcept {
    assert(bits < 16);
    return bits < 10 ? ('0' + bits) : ('a' + (bits - 10));
 }

std::pair<char, char> line::core::Hasher::Hash::toHex(unsigned char data) noexcept {
    return {bitsToHex((data >> 4) & 15), bitsToHex(data & 15)};
}

bool line::core::Hasher::Hash::isDigit(char c) noexcept {
    return (c >= '0') && (c <= '9');
}

bool line::core::Hasher::Hash::isHexSymbol(char c) noexcept {
    return isDigit(c) || ((c >= 'a') && (c <=  'f'));
}

unsigned char line::core::Hasher::Hash::hexDigitToBits(char hexDigit) noexcept {
    assert(isHexSymbol(hexDigit));
    return isDigit(hexDigit) ? (hexDigit - '0') : ((hexDigit - 'a') + 10);
}