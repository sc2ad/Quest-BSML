#include "../include/main.hpp"

#include <dlfcn.h>
#include "../include/types.hpp"
#include "../extern/beatsaber-hook/shared/utils/utils.h"
#include "../extern/beatsaber-hook/shared/utils/il2cpp-utils.hpp"

static ModInfo modInfo;

const Logger& logger() {
    static const Logger& logger(modInfo);
    return logger;
}

extern "C" void setup(ModInfo& info) {
    info.id      = ID;
    info.version = VERSION;
    modInfo      = info;
    logger().info("Leaving setup!");
}

// example hooks from TrickSaber
MAKE_HOOK_OFFSETLESS(Saber_Start, void, Il2CppObject* self) {
    Saber_Start(self);
    int saberType = CRASH_UNLESS(il2cpp_utils::GetPropertyValue<int>(self, "saberType"));
    logger().debug("SaberType: %i", saberType);
}

MAKE_HOOK_OFFSETLESS(Saber_ManualUpdate, void, Il2CppObject* self) {
    Saber_ManualUpdate(self);
}

MAKE_HOOK_OFFSETLESS(OVRInput_Update, void, Il2CppObject* self) {
    logger().debug("OVRInput_Update!");
    OVRInput_Update(self);
}

MAKE_HOOK_OFFSETLESS(FixedUpdate, void, Il2CppObject* self) {
    FixedUpdate(self);
}

extern "C" void load() {
    logger().info("Installing hooks...");
    INSTALL_HOOK_OFFSETLESS(Saber_ManualUpdate, il2cpp_utils::FindMethod("", "Saber", "ManualUpdate"));
    INSTALL_HOOK_OFFSETLESS(Saber_Start, il2cpp_utils::FindMethod("", "Saber", "Start"));

    INSTALL_HOOK_OFFSETLESS(FixedUpdate, il2cpp_utils::FindMethod("", "OculusVRHelper", "FixedUpdate"));
    // INSTALL_HOOK_OFFSETLESS(LateUpdate, il2cpp_utils::FindMethod("", "VRPlatformHelper", "LateUpdate"));
    logger().info("Installed all hooks!");
}
