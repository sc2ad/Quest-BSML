#pragma once

#include "extern/modloader/shared/modloader.hpp"
#include "extern/beatsaber-hook/shared/utils/logging.hpp"

static ModInfo modInfo;
const Logger& logger();

extern "C" void load();
