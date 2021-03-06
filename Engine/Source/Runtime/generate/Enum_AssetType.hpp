#pragma once

#include <ReflectEnum.hpp>
#include <Resources.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::AssetType
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::AssetType>() { return 6; }

// string array
static const char* AssetTypeStrings[] =
{
    "Unknown",
	"Texture",
	"Model",
	"Shader",
	"Material",
	"Script"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::AssetType>()
{
    return AssetTypeStrings;
}

// index to enum
template<>
inline FishEngine::AssetType ToEnum<FishEngine::AssetType>(const int index)
{
    switch (index) {
    case 0: return FishEngine::AssetType::Unknown; break;
	case 1: return FishEngine::AssetType::Texture; break;
	case 2: return FishEngine::AssetType::Model; break;
	case 3: return FishEngine::AssetType::Shader; break;
	case 4: return FishEngine::AssetType::Material; break;
	case 5: return FishEngine::AssetType::Script; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::AssetType>(FishEngine::AssetType e)
{
    switch (e) {
    case FishEngine::AssetType::Unknown: return 0; break;
	case FishEngine::AssetType::Texture: return 1; break;
	case FishEngine::AssetType::Model: return 2; break;
	case FishEngine::AssetType::Shader: return 3; break;
	case FishEngine::AssetType::Material: return 4; break;
	case FishEngine::AssetType::Script: return 5; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::AssetType ToEnum<FishEngine::AssetType>(const std::string& s)
{
    if (s == "Unknown") return FishEngine::AssetType::Unknown;
	if (s == "Texture") return FishEngine::AssetType::Texture;
	if (s == "Model") return FishEngine::AssetType::Model;
	if (s == "Shader") return FishEngine::AssetType::Shader;
	if (s == "Material") return FishEngine::AssetType::Material;
	if (s == "Script") return FishEngine::AssetType::Script;
	
    abort();
}


} // namespace FishEngine
