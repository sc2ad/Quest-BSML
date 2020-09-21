#pragma once
#include "../types.hpp"

#include <string>

namespace BeatSaberMarkupLanguage::Tags {
    class BSMLTag {
      public:
        bool isInitialized = false;
        std::string Aliases[];
        virtual bool AddChildren() = 0;

        virtual GameObject* CreateObject(Transform* parent) = 0;
        virtual void Setup() { }
    };
}
