#pragma once

#include <cstddef>

namespace line {

namespace cli {

namespace common {

namespace funcs {

bool isDirectory(const char* directoryPath);

bool isRepository();

std::size_t readCommitsCounter();

}

}

}

}