#pragma once

#include "../../../utils/types/Optional.thd"

namespace line {

namespace cli {

namespace common {

namespace funcs {

bool isDirectory(const char* directoryPath);

bool isRepository();

line::utils::types::Optional<std::size_t> readCommitsCounter();

}

}

}

}