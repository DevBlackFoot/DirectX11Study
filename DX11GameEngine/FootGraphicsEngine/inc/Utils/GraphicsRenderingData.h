#pragma once
#include "Utils/d3d11Utils.h"
#include "Utils/GraphicsTypedef.h"

// namespace 임시 material 겹쳐성..
namespace RenderingData
{
	// 함수 추가용 enum class
	enum class TextureMapType
	{
		ALBEDO,
		NORMAL,
		METALLIC,
		ROUGHNESS,
		AO,
		EMISSIVE,
		CUBE
	};

	struct Material
	{
		uint64 materialID;

		std::string materialName;

		uint64 albedoMap;

		uint64 normalMap;

		uint64 metallicMap;

		uint64 roughnessMap;

		uint64 AOMap;

		uint64 emissiveMap;

		uint64 cubeMap;

		float metallic;

		float roughness;
	};

	struct ObjectInfo
	{
		uint64 objectID;

		DirectX::SimpleMath::Matrix worldTM;

		DirectX::SimpleMath::Vector3 worldPos;

		DirectX::SimpleMath::Matrix finalBoneListMatrix[96];

		uint64 materialName;
	};
}