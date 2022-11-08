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

	// 게임 엔진에서 사용 가능하도록 DLL Export를 해준다.
	class GRAPHICSENGINE_DECLSPEC ResourceManager
	{
	private:
		static std::shared_ptr<ResourceManager> instance;

	public:
		ResourceManager()
		{}
		~ResourceManager() {};

		static std::shared_ptr<ResourceManager> GetInstance();

		// 모든 리소스들은 resourceID를 통해서 관리할 것입니다.
		// Mesh, Texture 들은 부여받은 ID를 가지고 있습니다.
		static uint64 resourceID;

	private:
		std::unordered_map<uint64, std::shared_ptr<Texture>> textures;

		std::unordered_map<uint64, std::shared_ptr<Mesh>> meshes;

		std::unordered_map<uint64, std::shared_ptr<RenderingData::Material>> materials;

	public:
		virtual std::shared_ptr<Texture> GetTexture(const uint64& _resourceID);

		virtual	std::shared_ptr<Mesh> GetMesh(const uint64& _resourceID);

		virtual std::shared_ptr<RenderingData::Material> GetMaterial(const uint64& _resourceID);

		// 텍스쳐를 로드하여 textures에 저장하고 resourceID를 return 합니다.
		virtual const uint64& LoadTexture(const std::string& textureName, const std::wstring& path);

		// 머터리얼을 mateirals에 저장하고 resourceID를 return 합니다.
		const uint64& AddMaterial(std::shared_ptr<RenderingData::Material> material);

		// 메시를 meshes에 저장하고 resourceID를 return 합니다.
			// 메시의 경우 수동으로 ID를 넣어줍니다.
			// 중복된 mesh 생성을 막기 위해..
		// TODO : 데이터 테이블 등을 이용해 특정 Mesh와 매칭되는 ID를 뽑아낼 수 있도록 해야 같은 키를 가진 Mesh가 생성 되지 않을 것.
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