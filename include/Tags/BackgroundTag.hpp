#include "BSMLTag.hpp"
#include "Components/Backgroundable.hpp"

#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"

namespace BeatSaberMarkupLanguage::Tags {
    class BackgroundTag : BSMLTag {
      public:
        std::string Aliases[] = { "bg", "background", "div" };

        GameObject* CreateObject(Transform* parent)
        {
            GameObject* gameObject = GameObject::New_ctor();
            gameObject->set_name(il2cpp_utils::createcsstr("BSMLBackground"));
            gameObject->get_transform()->SetParent(parent, false);
            gameObject->AddComponent<UnityEngine::UI::ContentSizeFitter>();
            gameObject->AddComponent<Backgroundable>();

            RectTransform* rectTransform = (RectTransform*)gameObject->get_transform();
            rectTransform->anchorMin = Vector2(0, 0);
            rectTransform->anchorMax = Vector2(1, 1);
            rectTransform->sizeDelta = Vector2(0, 0);

            return gameObject;
        }
    };
}
