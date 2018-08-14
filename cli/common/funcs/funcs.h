#pragma once

#include <cstddef>

namespace line {

namespace cli {

namespace common {

namespace funcs {

bool isDirectory(const char* directoryPath);

bool isRepository();

bool readCommitsCounter(std::size_t& commitsCounter);

}

}

}

}