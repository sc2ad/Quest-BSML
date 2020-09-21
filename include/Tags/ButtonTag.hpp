#pragma once

using BeatSaberMarkupLanguage.Components;
using System.Linq;
using TMPro;
using UnityEngine;
using UnityEngine.UI;
using static BeatSaberMarkupLanguage.Components.Strokable;

#include "BSMLTag.h"
#include "extern/questui/unity-helper.hpp"

namespace BeatSaberMarkupLanguage::Tags {
    class ButtonTag : BSMLTag {
      public:
        Aliases = { "button" };

        GameObject CreateObject(Transform parent) {
            Button button = UnityHelper::MonoBehaviour.Instantiate(Resources.FindObjectsOfTypeAll<Button>().Last(x => (x.name == (parent.GetComponent<StartMiddleEndButtonsGroup>() == null ? "PlayButton" : "CreditsButton"))), parent, false);
            button.name = "BSMLButton";
            button.interactable = true;
            Polyglot.LocalizedTextMeshProUGUI localizer = button.GetComponentInChildren<Polyglot.LocalizedTextMeshProUGUI>();
            if (localizer != null)
                GameObject.Destroy(localizer);
            ExternalComponents externalComponents = button.gameObject.AddComponent<ExternalComponents>();
            externalComponents.components.Add(button.GetComponentInChildren<TextMeshProUGUI>());

            HorizontalLayoutGroup horiztonalLayoutGroup = button.GetComponentInChildren<HorizontalLayoutGroup>();
            if (horiztonalLayoutGroup != null)
                externalComponents.components.Add(horiztonalLayoutGroup);

            Image glowImage = button.gameObject.GetComponentsInChildren<Image>(true).Where(x => x.gameObject.name == "Glow").FirstOrDefault();
            if (glowImage != null)
            {
                Glowable glowable = button.gameObject.AddComponent<Glowable>();
                glowable.image = glowImage;
                glowable.SetGlow("none");
            }

            Image strokeImage = button.gameObject.GetComponentsInChildren<Image>(true).Where(x => x.gameObject.name == "Stroke").FirstOrDefault();
            if (strokeImage != null)
            {
                Strokable strokable = button.gameObject.AddComponent<Strokable>();
                strokable.image = strokeImage;
                strokable.SetType(StrokeType.Regular);
            }

            return button.gameObject;
        }
    }
}
