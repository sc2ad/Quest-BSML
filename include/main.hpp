#pragma once

#include "extern/modloader/shared/modloader.hpp"
#include "extern/beatsaber-hook/shared/utils/logging.hpp"

const Logger& logger();

extern "C" void load();
