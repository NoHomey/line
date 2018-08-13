
#include <iostream>
#include <sstream>
#include "./core/Hasher/Hasher.h"

int main() {
    std::istringstream input{"It works!"};

    line::core::Hasher::Hash hash = line::core::Hasher::hash(input);

    hash.print(std::cout);

    std::cout << std::endl;

    return 0;
}