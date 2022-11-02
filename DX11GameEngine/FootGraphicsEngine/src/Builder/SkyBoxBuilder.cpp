#include "GraphicsPch.h"
#include "Builder/SkyBoxBuilder.h"

#include "Object/IDXObject.h"
#include "Object/SkyBox.h"
#include "GraphicsCore/RasterizerState.h"
#include "Manager/ResourceManager.h"

using namespace DirectX::SimpleMath;

namespace GraphicsEngineSpace
{
	std::shared_ptr<IDXObject> SkyBoxBuilder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName,
		uint64 objectID)
	{
		// 들어온 오브젝트 확인
		if(std::dynamic_pointer_cast<SkyBox>(pDXObj) == nullptr)
			return nullptr;

		pDXObj->SetObjectResources(BuildGeometry(objectName, objectID));

		return pDXObj;
	}

	std::shared_ptr<ObjectResources> SkyBoxBuilder::BuildGeometry(std::string objectName, uint64 objectID)
	{
		std::shared_ptr<ObjectResources> _tmpObjRes = std::make_shared<ObjectResources>();

		_tmpObjRes->ObjName = objectName;

		// 뭐가 되었건.. 스카이 박스로 만들어줍니다.
		_tmpObjRes = BuildSkyBoxResources(_tmpObjRes, objectID);

		return _tmpObjRes;
	}

	void SkyBoxBuilder::InitBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pDC)
	{
		D3DDevice = pDevice;
		D3DDeviceContext = pDC;

		// 리소스 매니저 바인딩
		resourceManager = ResourceManager::GetInstance();
	}

	std::shared_ptr<IDXObject> SkyBoxBuilder::AddTexture(std::shared_ptr<IDXObject> DXObj, uint64 textureID,
		std::string textureName, std::wstring path, RenderingData::TextureMapType mapType)
	{
		std::shared_ptr<ObjectResources> objRes = DXObj->GetObjectResources();

		if (objRes == nullptr)
			return DXObj;

		// 텍스쳐 값을 추가할 머테리얼
		std::shared_ptr<RenderingData::Material> tempMaterial;

		// 머테리얼에 접근해본다.
			// 해당 맵의 키값으로 접근하는 것이기 때문에
			// 0(ID 초기화 값)이 머테리얼이 아니면 nullptr이 나올 것이다.
			// 0이 머테리얼이면 nullptr이 나오지 않을 것이다.
			// 기존의 머테리얼을 세팅해둔 상태면. 그것을 그대로 가져온다.
			// obj를 받아서 추가해주는 것이므로 이런식의 함수가 가능하다.
		if (resourceManager->GetMaterial(objRes->materialID) != nullptr && objRes->setMaterial == true)
		{
			tempMaterial = resourceManager->GetMaterial(objRes->materialID);
		}
		// 없다면 만들어서 넣어준다.
		else
		{
			objRes->setMaterial = true;

			tempMaterial = std::make_shared<RenderingData::Material>();

			objRes->materialID = resourceManager->AddMaterial(tempMaterial);
		}

		// 텍스쳐가 존재하는지 찾기
		if (resourceManager->GetTexture(textureID) != nullptr)
		{
			// 있다면 머테리얼에 넣어준다..
				// 현재는 수동으로 하지만 자체포맷의 Deserialize가 완성되면 자동으로 넣어줄 것
			switch (mapType)
			{
				case RenderingData::TextureMapType::ALBEDO:
				{
					tempMaterial->albedoMap = textureID;

					break;
				}

				case RenderingData::TextureMapType::NORMAL:
				{
					tempMaterial->normalMap = textureID;

					break;
				}

				case RenderingData::TextureMapType::METALLIC:
				{
					tempMaterial->metallicMap = textureID;

					break;
				}

				case RenderingData::TextureMapType::ROUGHNESS:
				{
					tempMaterial->roughnessMap = textureID;

					break;
				}

				case RenderingData::TextureMapType::AO:
				{
					tempMaterial->AOMap = textureID;

					break;
				}

				case RenderingData::TextureMapType::EMISSIVE:
				{
					tempMaterial->emissiveMap = textureID;

					break;
				}

				case RenderingData::TextureMapType::CUBE:
				{
					tempMaterial->cubeMap = textureID;

					break;
				}
			}

			return DXObj;
		}

		// 만들어서 넣어준다.
		uint64 tempID = resourceManager->LoadTexture(textureName, path);

		switch (mapType)
		{
			case RenderingData::TextureMapType::ALBEDO:
			{
				tempMaterial->albedoMap = tempID;

				break;
			}

			case RenderingData::TextureMapType::NORMAL:
			{
				tempMaterial->normalMap = tempID;

				break;
			}

			case RenderingData::TextureMapType::METALLIC:
			{
				tempMaterial->metallicMap = tempID;

				break;
			}

			case RenderingData::TextureMapType::ROUGHNESS:
			{
				tempMaterial->roughnessMap = tempID;

				break;
			}

			case RenderingData::TextureMapType::AO:
			{
				tempMaterial->AOMap = tempID;

				break;
			}

			case RenderingData::TextureMapType::EMISSIVE:
			{
				tempMaterial->emissiveMap = tempID;

				break;
			}

			case RenderingData::TextureMapType::CUBE:
			{
				tempMaterial->cubeMap = tempID;

				break;
			}
		}

		return DXObj;
	}

	std::shared_ptr<ObjectResources> SkyBoxBuilder::BuildSkyBoxResources(std::shared_ptr<ObjectResources> _objRes,
		uint64 objectID)
	{
		// 현재 ObjMesh가 있는지 확인
		if (resourceManager->GetMesh(objectID) != nullptr)
		{
			// 있다면 해당 ID를 넣어주고 리턴한다.
			_objRes->meshID = objectID;
			_objRes->setMesh = true;

			return _objRes;
		}

		// CubeMapMesh 만들어보기
			// 기본적인 박스
		std::vector<VertexStruct::SkyBoxVertex> vertices =
		{
			VertexStruct::SkyBoxVertex(Vector3{ +1.0f, +1.0f, +1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ +1.0f, -1.0f, +1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ -1.0f, -1.0f, +1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ -1.0f, +1.0f, +1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ -1.0f, +1.0f, -1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ -1.0f, -1.0f, -1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ +1.0f, -1.0f, -1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ +1.0f, +1.0f, -1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ +1.0f, +1.0f, -1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ +1.0f, -1.0f, -1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ +1.0f, -1.0f, +1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ +1.0f, +1.0f, +1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ -1.0f, +1.0f, +1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ -1.0f, -1.0f, +1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ -1.0f, -1.0f, -1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ -1.0f, +1.0f, -1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ -1.0f, +1.0f, +1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ -1.0f, +1.0f, -1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ +1.0f, +1.0f, -1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ +1.0f, +1.0f, +1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ +1.0f, -1.0f, +1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ +1.0f, -1.0f, -1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ -1.0f, -1.0f, -1.0f }),
			VertexStruct::SkyBoxVertex(Vector3{ -1.0f, -1.0f, +1.0f })

		};
		std::vector<unsigned> indices =
		{
			0, 1, 2,
			0, 2, 3,
			4, 5, 6,
			4, 6, 7,
			8, 9, 10,
			8, 10, 11,
			12, 13, 14,
			12, 14, 15,
			16, 17, 18,
			16, 18, 19,
			20, 21, 22,
			20, 22, 23
		};

		// 만들어진 구를 바탕으로 메시를 만들어줍니다.
		_objRes->meshID = resourceManager->CreateMesh(
		vertices,
		indices,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		RasterizerState::GetSolidRS(),
		objectID);

		_objRes->setMesh = true;

		return _objRes;
	}
}
