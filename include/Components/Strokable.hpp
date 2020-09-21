// using System.Linq;
// using UnityEngine;
// using UnityEngine.UI;

#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/UI/Image.hpp"

namespace BeatSaberMarkupLanguage::Components {
    class Strokable : UnityEngine::MonoBehaviour {
      public:
        Image* image;

        void SetType(StrokeType strokeType) {
            if (image == null)
                return;
            switch (strokeType)
            {
                case StrokeType.None:
                    image.enabled = false;
                    break;
                case StrokeType.Clean:
                    image.enabled = true;
                    image.sprite = Resources.FindObjectsOfTypeAll<Sprite>().Last(x => x.name == "RoundRectSmallStroke");
                    break;
                case StrokeType.Regular:
                    image.enabled = true;
                    image.sprite = Resources.FindObjectsOfTypeAll<Sprite>().Last(x => x.name == "RoundRectBigStroke");
                    break;
            }
        }

        void SetColor(string strokeColor) {
            if (image == null)
                return;
            if (!ColorUtility.TryParseHtmlString(strokeColor, out Color color))
                Logger.log.Warn($"Invalid color: {strokeColor}");
            image.color = color;
            image.enabled = true;
        }

        enum StrokeType {
            None, Clean, Regular
        }
    }
}
