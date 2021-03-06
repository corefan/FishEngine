#include "Resources.hpp"

namespace FishEngine
{
	AssetType Resources::GetAssetType(Path const & ext)
	{
		//auto ext = path.extension();
		if (ext == ".jpg" || ext == ".png" || ext == ".jpeg" || ext == ".tga")
		{
			return AssetType::Texture;
		}
		else if (ext == ".obj" || ext == ".fbx")
		{
			return AssetType::Model;
		}
		else if (ext == ".shader")
		{
			return AssetType::Shader;
		}
		else if (ext == ".mat")
		{
			return AssetType::Material;
		}
		else if (ext == ".hpp" || ext == ".cpp")
		{
			return AssetType::Script;
		}
		return AssetType::Unknown;
	}
}
