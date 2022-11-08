#pragma once
#include "GraphicsCore/RasterizerState.h"
#include "Resources/Mesh.h"

#ifdef FOOTGRAPHICSENGINE_EXPORTS
#define GRAPHICSENGINE_DECLSPEC __declspec(dllexport)
#else
#define GRAPHICSENGINE_DECLSPEC __declspec(dllimport)
#endif

namespace GraphicsEngineSpace
{
	class Texture;

	class Mesh;

	struct Material;

	// ���� �������� ��� �����ϵ��� DLL Export�� ���ش�.
	class GRAPHICSENGINE_DECLSPEC ResourceManager
	{
	private:
		static std::shared_ptr<ResourceManager> instance;

	public:
		ResourceManager()
		{}
		~ResourceManager() {};

		static std::shared_ptr<ResourceManager> GetInstance();

		// ��� ���ҽ����� resourceID�� ���ؼ� ������ ���Դϴ�.
		// Mesh, Texture ���� �ο����� ID�� ������ �ֽ��ϴ�.
		static uint64 resourceID;

	private:
		std::unordered_map<uint64, std::shared_ptr<Texture>> textures;

		std::unordered_map<uint64, std::shared_ptr<Mesh>> meshes;

		std::unordered_map<uint64, std::shared_ptr<RenderingData::Material>> materials;

	public:
		virtual std::shared_ptr<Texture> GetTexture(const uint64& _resourceID);

		virtual	std::shared_ptr<Mesh> GetMesh(const uint64& _resourceID);

		virtual std::shared_ptr<RenderingData::Material> GetMaterial(const uint64& _resourceID);

		// �ؽ��ĸ� �ε��Ͽ� textures�� �����ϰ� resourceID�� return �մϴ�.
		virtual const uint64& LoadTexture(const std::string& textureName, const std::wstring& path);

		// ���͸����� mateirals�� �����ϰ� resourceID�� return �մϴ�.
		const uint64& AddMaterial(std::shared_ptr<RenderingData::Material> material);

		// �޽ø� meshes�� �����ϰ� resourceID�� return �մϴ�.
			// �޽��� ��� �������� ID�� �־��ݴϴ�.
			// �ߺ��� mesh ������ ���� ����..
		// TODO : ������ ���̺� ���� �̿��� Ư�� Mesh�� ��Ī�Ǵ� ID�� �̾Ƴ� �� �ֵ��� �ؾ� ���� Ű�� ���� Mesh�� ���� ���� ���� ��.
		template <typename T>
		const uint64& CreateMesh(std::vector<T> vertices, std::vector<unsigned int> indices, D3D_PRIMITIVE_TOPOLOGY topology, ComPtr<ID3D11RasterizerState> rasterizerState, uint64
		                   meshID);
	};

	template<typename T>
	inline const uint64& ResourceManager::CreateMesh(std::vector<T> vertices, std::vector<unsigned int> indices, D3D_PRIMITIVE_TOPOLOGY topology, ComPtr<ID3D11RasterizerState> rasterizerState, uint64
	                                           meshID)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

		mesh->SetPrimitiveTopology(topology);
		
		mesh->SetRenderState(rasterizerState);

		mesh->CreateVertexBuffer(vertices);
		mesh->CreateIndexBuffer(indices);
		mesh->SetIndexBufferSize(indices.size());
		mesh->SetStride(sizeof(T));

		mesh->SetResourceID(meshID);

		meshes.insert(std::make_pair(mesh->GetResourceID(), mesh));

		return mesh->GetResourceID();
	}

}