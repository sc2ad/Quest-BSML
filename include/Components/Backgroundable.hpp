// using System;
// using System.Collections.Generic;
// using System.Linq;
// using UnityEngine;
// using UnityEngine.UI;

#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/UI/Image.hpp"

namespace BeatSaberMarkupLanguage::Components {
    class Backgroundable : UnityEngine::MonoBehaviour {
      private:
        static Dictionary<string, string> Backgrounds => new Dictionary<string, string>()
        {
            { "round-rect-panel", "RoundRectPanel" },
            { "panel-bottom", "PanelBottom" },
            { "panel-top", "PanelTop" },
            { "round-rect-panel-shadow", "RoundRectPanelShadow"}
        };

        static Dictionary<string, string> ObjectNames => new Dictionary<string, string>()
        {
            { "round-rect-panel", "MinScoreInfo" },
            { "panel-bottom", "BG" },
            { "panel-top", "HeaderPanel" },
            { "round-rect-panel-shadow", "Shadow"}
        };

      public:
        Image* background;

        void ApplyBackground(Il2CppString* name) {
            if (background != null)
                throw new Exception("Cannot add multiple backgrounds");

            if (!Backgrounds.TryGetValue(name, out string backgroundName))
                throw new Exception("Background type '" + name + "' not found");

            auto images = UnityEngine::Resources::FindObjectsOfTypeAll<Image*>();
            auto comp = images.Last(x => x.gameObject.name == ObjectNames[name] && x.sprite?.name == backgroundName);
            background = gameObject->AddComponent(comp);
            background.enabled = true;
        }
    }
}
