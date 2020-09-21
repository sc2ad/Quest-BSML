// using System;
// using System.Collections;
// using System.Collections.Generic;
// using System.IO;
// using System.Linq;
// using System.Reflection;
// using UnityEngine;
// using UnityEngine.Networking;
// using UnityEngine.UI;

#include "main.hpp"

#include "extern/codegen/include/System/Activator.hpp"
#include "extern/codegen/include/System/Func_2.hpp"
#include "extern/codegen/include/System/Collections/Generic/List_1.hpp"
#include "extern/codegen/include/System/IO/File.hpp"
#include "extern/codegen/include/System/IO/Stream.hpp"
#include "extern/codegen/include/System/IO/StreamReader.hpp"
#include "extern/codegen/include/System/Linq/Enumerable.hpp"
#include "extern/codegen/include/System/Reflection/Assembly.hpp"
#include "extern/codegen/include/System/Reflection/BindingFlags.hpp"
#include "extern/codegen/include/System/Reflection/PropertyInfo.hpp"
#include "extern/codegen/include/System/String.hpp"
#include "extern/codegen/include/System/Type.hpp"
#include "extern/codegen/include/UnityEngine/ImageConversion.hpp"
#include "extern/codegen/include/UnityEngine/Material.hpp"
#include "extern/codegen/include/UnityEngine/Rect.hpp"
#include "extern/codegen/include/UnityEngine/Resources.hpp"
#include "extern/codegen/include/UnityEngine/Sprite.hpp"
#include "extern/codegen/include/UnityEngine/SpriteMeshType.hpp"
#include "extern/codegen/include/UnityEngine/Texture2D.hpp"
#include "extern/codegen/include/UnityEngine/Networking/UnityWebRequest.hpp"
#include "extern/codegen/include/UnityEngine/UI/Image.hpp"

#include "extern/beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "extern/beatsaber-hook/shared/utils/il2cpp-utils.hpp"

// `defer { statement; };` causes `statement` to be executed when the current scope is exitted
#ifndef defer
struct defer_dummy {};
template <class F> struct deferrer { F f; ~deferrer() { f(); } };
template <class F> deferrer<F> operator*(defer_dummy, F f) { return {f}; }
#define DEFER_(LINE) zz_defer##LINE
#define DEFER(LINE) DEFER_(LINE)
#define defer auto DEFER(__LINE__) = defer_dummy{} *[&]()
#endif // defer

namespace BeatSaberMarkupLanguage
{
    class Utilities {
      private:
        static UnityEngine::Sprite* _editIcon;

      public:
        static UnityEngine::Sprite* EditIcon() {
            if (!_editIcon) {
                auto images = UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::UI::Image*>();
                for (il2cpp_array_size_t i = 0; i < images->Length(); i++) {
                    auto* img = images->values[i];
                    auto* sprite = img->get_sprite();
                    if (!sprite) continue;
                    Il2CppString* name = sprite->get_name();
                    if (to_utf8(csstrtostr(name)) == "EditIcon") {
                        _editIcon = sprite;
                        break;
                    }
                }
            }
            return _editIcon;
        }

        static Il2CppString* GetResourceContent(System::Reflection::Assembly* assembly, std::string_view resource) {
            Il2CppString* res = il2cpp_utils::createcsstr(resource);
            Il2CppString* content;
            // equivalent of "using"
            { auto* stream = assembly->GetManifestResourceStream(res); defer { stream->Dispose(); };
                { auto* reader = System::IO::StreamReader::New_ctor(stream); defer { reader->Dispose(); };
                    content = reader->ReadToEnd();
                }
            }
            return content;
        }

        #define typeof(T) il2cpp_utils::GetSystemType(il2cpp_utils::il2cpp_type_check::il2cpp_arg_type<T>::get())

        template<class T>
        static System::Collections::Generic::List_1<T> GetListOfType(::Array<::Il2CppObject*>* constructorArgs) {
            auto* objects = System::Collections::Generic::List_1<T>::New_ctor();
            auto* typeofT = typeof(T);
            auto* types = System::Reflection::Assembly::GetAssembly(typeofT)->GetTypes();
            for (il2cpp_array_size_t i = 0; i < types.Length(); i++) {
                auto* type = types->values[i];
                if (type->get_IsClass() && !type->get_IsAbstract() && type->IsSubclassOf(typeofT)) {
                    objects->Add((T)System::Activator::CreateInstance(type, constructorArgs));
                }
            }
            return objects;
        }

        //yoinked from https://answers.unity.com/questions/530178/how-to-get-a-component-from-an-object-and-add-it-t.html
        template<class T>
        static T* GetCopyOf(/*this*/ UnityEngine::Component* comp, T* other) {
            static_assert(std::is_base_of_v<UnityEngine::Component, T>);
            System::Type* type = comp->GetType();
            if (type != other->GetType())
                return nullptr; // type mismatch

            // TODO: write this using il2cpp reflection instead
            using System::Reflection::BindingFlags;
            BindingFlags flags = BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance;
            auto* pinfos = type->GetProperties(flags);
            for (auto* pinfo : pinfos) {
                if (pinfo->get_CanWrite() && to_utf8(csstrtostr(pinfo->get_Name())) != "name") {
                    try {
                        pinfo->SetValue(comp, pinfo->GetValue(other, nullptr), nullptr);
                    } catch {
                        // "In case of NotImplementedException being thrown. For some reason specifying that exception didn't 
                        // seem to catch it, so I didn't catch anything specific."
                    }
                }
            }

            auto* finfos = type->GetFields(flags);
            for (auto* finfo : finfos)
                finfo->SetValue(comp, finfo->GetValue(other));

            return (T*)comp;
        }

        template<class T>
        static T* AddComponent(/*this*/ UnityEngine::GameObject* go, T* toAdd) {
            static_assert(std::is_base_of_v<UnityEngine::Component, T>);
            return GetCopyOf(go->AddComponent<T*>(), toAdd);
        }
        //end of yoink

        // https://stackoverflow.com/a/3418285/7376471
        static void replaceAll(std::string& str, const std::string& from, const std::string& to) {
            if (from.empty()) return;
            size_t start_pos = 0;
            while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
                str.replace(start_pos, from.length(), to);
                start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
            }
        }

        static std::string EscapeXml(std::string_view source) {
            std::string str(source);
            replaceAll(str, "\"", "&quot;");
            replaceAll(str, "&", "&amp;");
            replaceAll(str, "'", "&apos;");
            replaceAll(str, "<", "&lt;");
            replaceAll(str, ">", "&gt;");
            return str;
        }

        class ImageResources {
          private:
            static UnityEngine::Material*  noGlowMat;
            static UnityEngine::Sprite* _blankSprite;
            static UnityEngine::Sprite* _whitePixel;

          public:
            static UnityEngine::Material*  NoGlowMat() {
                if (!noGlowMat) {
                    auto* mats = UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::Material*>();
                    UnityEngine::Material* origNoGlowMat = nullptr;
                    for (il2cpp_array_size_t i = 0; i < mats->Length(); i++) {
                        auto* mat = mats->values[i];
                        if (to_utf8(csstrtostr(mat->get_name())) == "UINoGlow") {
                            origNoGlowMat = mat;
                            break;
                        }
                    }
                    noGlowMat = UnityEngine::Material::New_ctor(origNoGlowMat);
                    noGlowMat->set_name(il2cpp_utils::createcsstr("UINoGlowCustom"));
                }

                return noGlowMat;
            }

            static UnityEngine::Sprite* BlankSprite() {
                if (!_blankSprite) {
                    static auto getBlackTexture = (function_ptr_t<UnityEngine::Texture2D*>)CRASH_UNLESS(
                        il2cpp_functions::resolve_icall("UnityEngine.Texture2D::get_blackTexture"));
                    
                    _blankSprite = UnityEngine::Sprite::Create(getBlackTexture(), UnityEngine::Rect(),
                        UnityEngine::Vector2::get_zero(), 100.0f, 0u, UnityEngine::SpriteMeshType::Tight,
                        UnityEngine::Vector4::get_zero(), false);
                }

                return _blankSprite;
            }

            static UnityEngine::Sprite* WhitePixel() {
                if (!_whitePixel) {
                    auto* images = UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::UI::Image*>();
                    for (il2cpp_array_size_t i = 0; i < images->Length(); i++) {
                        auto* img = images->values[i];
                        auto* sprite = img->get_sprite();
                        if (!sprite) continue;
                        Il2CppString* name = sprite->get_name();
                        if (to_utf8(csstrtostr(name)) == "WhitePixel") {
                            _whitePixel = sprite;
                            break;
                        }
                    }
                }

                return _whitePixel;
            }
        };

        static UnityEngine::Texture2D* FindTextureInAssembly(std::string_view path) {
            try {
                auto [assembly, newPath] = AssemblyFromPath(path);
                if (assembly->GetManifestResourceNames()->Contains(newPath))
                    return LoadTextureRaw(GetResource(assembly, newPath));
            } catch {
                logger().error("Unable to find texture in assembly! (You must prefix path with 'assembly name:' if the assembly "
                    "and root namespace don't have the same name)");
            }
            return nullptr;
        }

        static UnityEngine::Sprite* FindSpriteInAssembly(std::string_view path) {
            try {
                auto [assembly, newPath] = AssemblyFromPath(path);
                if (assembly->GetManifestResourceNames()->Contains(newPath))
                    return LoadSpriteRaw(GetResource(assembly, newPath));
            } catch (std::runtime_error& ex) {
                logger().error("Unable to find UnityEngine::Sprite* in assembly! (You must prefix path with 'assembly name:' if "
                    "the assembly and root namespace don't have the same name) Exception: %s", ex.what());
            }
            return nullptr;
        }

        static auto Split(std::string_view str, char sep) {
            std::vector<std::string> ret;
            size_t start = 0, end;
            do {
                end = str.find_first_of(sep, start);
                ret.append(str.substr(start, end))
                start = end + 1;
            }
            while (end != std::string_view::npos);
            return ret;
        }

        static std::pair<System::Reflection::Assembly*, Il2CppString*> AssemblyFromPath(std::string_view inputPath) {
            auto parameters = Split(inputPath, ':');
            System::Reflection::Assembly* assembly;
            std::string_view path;
            switch (parameters.size())
            {
                case 1:
                    path = parameters[0];
                    assembly = System::Reflection::Assembly::Load(path.substr(0, path.find_first_of('.')));
                    break;
                case 2:
                    path = parameters[1];
                    assembly = System::Reflection::Assembly::Load(parameters[0]);
                    break;
                default:
                    throw std::runtime_error(string_format("Could not process resource path '%s'", inputPath.data()).c_str());
            }
            return make_pair(assembly, il2cpp_utils::createcsstr(path));
        }

        static UnityEngine::Texture2D* LoadTextureRaw(::Array<uint8_t>* file) {
            if (file->Length() > 0) {
                auto* Tex2D = UnityEngine::Texture2D::New_ctor(2, 2);
                if (UnityEngine::ImageConversion::LoadImage(Tex2D, file))
                    return Tex2D;
            }
            return nullptr;
        }

        static UnityEngine::Sprite* LoadSpriteRaw(::Array<uint8_t>* image, float PixelsPerUnit = 100.0f) {
            return LoadSpriteFromTexture(LoadTextureRaw(image), PixelsPerUnit);
        }

        static UnityEngine::Sprite* LoadSpriteFromTexture(UnityEngine::Texture2D* SpriteTexture, float PixelsPerUnit = 100.0f) {
            if (SpriteTexture)
                return UnityEngine::Sprite::Create(SpriteTexture,
                    UnityEngine::Rect(0, 0, SpriteTexture->get_width(), SpriteTexture->get_height()),
                    UnityEngine::Vector2::get_zero(), PixelsPerUnit,
                    // remaining args are defaults
                    0u, UnityEngine::SpriteMeshType::Tight, UnityEngine::Vector4::get_zero(), false);
            return nullptr;
        }

        static ::Array<uint8_t>* GetResource(System::Reflection::Assembly* assembly, Il2CppString* ResourceName) {
            System::IO::Stream* stream = assembly->GetManifestResourceStream(ResourceName);
            il2cpp_functions::CheckS_GlobalMetadata();
            auto* data = (::Array<uint8_t>)il2cpp_functions::array_new_specific(
                il2cpp_functions::defaults->byte_class, stream->Length());
            stream->Read(data, 0, stream->Length());
            return data;
        }

        // public static IEnumerable<T> SingleEnumerable<T>(this T item) => Enumerable.Empty<T>().Append(item);

        // public static IEnumerable<T?> AsNullable<T>(this IEnumerable<T> seq) where T : struct => seq.Select(v => new T?(v));

        // public static T? AsNullable<T>(this T item) where T : struct => item;

        // port of System::IO::File::ReadAllBytes, which was stripped from our .so's
        static ::Array<uint8_t>* ReadAllBytes(Il2CppString* path) {
			::Array<uint8_t>* result = nullptr;
            { auto* fileStream = System::IO::File::OpenRead(path); defer { fileStream->Dispose(); };
				int64_t length = fileStream->get_Length();
				if (length > 2147483647L)
                    throw std::runtime_error("Reading more than 2GB with this call is not supported");
				int num = 0;
				int i = (int)length;
				auto* array = (::Array<uint8_t>)il2cpp_functions::array_new_specific(
                    il2cpp_functions::defaults->byte_class, stream->Length());
				while (i > 0) {
					int num2 = fileStream->Read(array, num, i);
					if (num2 == 0)
                        throw std::runtime_error("Unexpected end of stream");
					num += num2;
					i -= num2;
				}
				result = array;
			}
			return result;
		}

        typedef std::function<void(::Array<uint8_t>*)> FileDataCallback;

        /// <summary>
        /// Get data from either a resource path, a file path, or a url
        /// </summary>
        /// <param name="location">Resource path, file path, or url. May need to prefix resource paths with 'AssemblyName:'</param>
        /// <param name="callback">Received data</param>
        static void GetData(std::string_view location, FileDataCallback callback) {
            try {
                if (location.starts_with("http://") || location.starts_with("https://")) {
                    SendWebRequest(location, callback);
                } else {
                    auto* locationStr = il2cpp_utils::createcsstr(location);
                    if (System::IO::File::Exists(locationStr)) {
                        if (callback) callback(ReadAllBytes(locationStr));
                    } else {
                        auto [assembly, newPath] = AssemblyFromPath(location);
                        if (callback) callback(GetResource(assembly, newPath));
                    }
                }
            } catch {
                logger().error("Error getting data from '%s': either invalid path or file does not exist.", location.data());
            }
        }

        static void WebRequestComplete(FileDataCallback* callback, UnityEngine::Networking::UnityWebRequestAsyncOperation* op) {
            auto* www = op->get_webRequest();
            if (www->get_isNetworkError() || www->get_isHttpError()) {
                logger().error("Error getting data from %s, Message: %s", to_utf8(csstrtostr(www->GetUrl())).data(),
                    to_utf8(csstrtostr(www->get_error())).data());
            } else if (*callback) {
                (*callback)(www->get_downloadHandler()->get_data());
            }
            op->Finalize();
        }

        static bool SendWebRequest(std::string_view url, FileDataCallback callback) {
            auto* webReq = UnityEngine::Networking::UnityWebRequest::Get(il2cpp_utils::createcsstr(url));
            auto* asyncOp = www->SendWebRequest();

            auto* method = RET_0_UNLESS(il2cpp_utils::FindMethodUnsafe(asyncOp, "add_completed", 1));
            auto* action = RET_0_UNLESS(il2cpp_utils::MakeAction(method, 0, new FileDataCallback(callback), WebRequestComplete));

            RET_0_UNLESS(il2cpp_utils::RunMethod(asyncOp, method, action));
            return true;
        }
    };
}
