#include "./Hasher.h"
#include <cassert>
#include <cstring>
#include "../../utils/funcs/integerToComparsionResult.thd"

unsigned char line::core::Hasher::hashCode[SHA256_DIGEST_LENGTH];

char line::core::Hasher::inputBuffer[inputBufferSize];

line::core::Hasher::Hash line::core::Hasher::hash(std::istream& input) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
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

line::core::Hasher::Hash::Hash(unsigned char* hash) noexcept {
    assert(hash);
    std::memcpy(hashCode, hash, SHA256_DIGEST_LENGTH);
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

void line::core::Hasher::Hash::print(std::ostream& output) {
    for(unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        std::pair<char, char> hex = toHex(hashCode[i]);
        output << hex.first << hex.second;
    }
}

 char line::core::Hasher::Hash::bitsToHex(unsigned char bits) noexcept {
    assert(bits < 16);
    return bits < 10 ? ('0' + bits) : ('a' + (bits - 10));
 }

std::pair<char, char> line::core::Hasher::Hash::toHex(unsigned char data) noexcept {
    return {bitsToHex((data >> 4) & 15), bitsToHex(data & 15)};
}