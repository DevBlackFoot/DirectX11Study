#include "GraphicsPch.h"
#include "Manager/ResourceManager.h"
#include "Resources/Texture.h"

namespace GraphicsEngineSpace
{
	std::shared_ptr<ResourceManager> ResourceManager::instance = nullptr;

	uint64 ResourceManager::resourceID = 0;

	std::shared_ptr<ResourceManager> ResourceManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<ResourceManager>();

		return instance;
	}

	std::shared_ptr<Texture> ResourceManager::GetTexture(const uint64& _resourceID)
	{
		return textures.find(_resourceID) != textures.end() ? textures.at(_resourceID) : nullptr;
	}

	std::shared_ptr<Mesh> ResourceManager::GetMesh(const uint64& _resourceID)
	{
		return meshes.find(_resourceID) != meshes.end() ? meshes.at(_resourceID) : nullptr;
	}

	std::shared_ptr<RenderingData::Material> ResourceManager::GetMaterial(const uint64& _resourceID)
	{
		return materials.find(_resourceID) != materials.end() ? materials.at(_resourceID) : nullptr;
	}

	const uint64& ResourceManager::LoadTexture(const std::string& textureName, const std::wstring& path)
	{
		std::shared_ptr<Texture> newTexture = std::make_shared<Texture>();

		newTexture->SetResourceID(resourceID++);

		newTexture->Initialize(textureName, path);

		textures.insert({ newTexture->GetResourceID(), newTexture });

		return newTexture->GetResourceID(); 
	}

	const uint64& ResourceManager::AddMaterial(std::shared_ptr<RenderingData::Material> material)
	{
		material->materialID = resourceID++;	

		materials.insert(make_pair(material->materialID, material));

		return material->materialID;
	}
}