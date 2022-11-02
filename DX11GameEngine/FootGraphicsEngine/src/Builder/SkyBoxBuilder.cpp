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
		// ���� ������Ʈ Ȯ��
		if(std::dynamic_pointer_cast<SkyBox>(pDXObj) == nullptr)
			return nullptr;

		pDXObj->SetObjectResources(BuildGeometry(objectName, objectID));

		return pDXObj;
	}

	std::shared_ptr<ObjectResources> SkyBoxBuilder::BuildGeometry(std::string objectName, uint64 objectID)
	{
		std::shared_ptr<ObjectResources> _tmpObjRes = std::make_shared<ObjectResources>();

		_tmpObjRes->ObjName = objectName;

		// ���� �Ǿ���.. ��ī�� �ڽ��� ������ݴϴ�.
		_tmpObjRes = BuildSkyBoxResources(_tmpObjRes, objectID);

		return _tmpObjRes;
	}

	void SkyBoxBuilder::InitBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pDC)
	{
		D3DDevice = pDevice;
		D3DDeviceContext = pDC;

		// ���ҽ� �Ŵ��� ���ε�
		resourceManager = ResourceManager::GetInstance();
	}

	std::shared_ptr<IDXObject> SkyBoxBuilder::AddTexture(std::shared_ptr<IDXObject> DXObj, uint64 textureID,
		std::string textureName, std::wstring path, RenderingData::TextureMapType mapType)
	{
		std::shared_ptr<ObjectResources> objRes = DXObj->GetObjectResources();

		if (objRes == nullptr)
			return DXObj;

		// �ؽ��� ���� �߰��� ���׸���
		std::shared_ptr<RenderingData::Material> tempMaterial;

		// ���׸��� �����غ���.
			// �ش� ���� Ű������ �����ϴ� ���̱� ������
			// 0(ID �ʱ�ȭ ��)�� ���׸����� �ƴϸ� nullptr�� ���� ���̴�.
			// 0�� ���׸����̸� nullptr�� ������ ���� ���̴�.
			// ������ ���׸����� �����ص� ���¸�. �װ��� �״�� �����´�.
			// obj�� �޾Ƽ� �߰����ִ� ���̹Ƿ� �̷����� �Լ��� �����ϴ�.
		if (resourceManager->GetMaterial(objRes->materialID) != nullptr && objRes->setMaterial == true)
		{
			tempMaterial = resourceManager->GetMaterial(objRes->materialID);
		}
		// ���ٸ� ���� �־��ش�.
		else
		{
			objRes->setMaterial = true;

			tempMaterial = std::make_shared<RenderingData::Material>();

			objRes->materialID = resourceManager->AddMaterial(tempMaterial);
		}

		// �ؽ��İ� �����ϴ��� ã��
		if (resourceManager->GetTexture(textureID) != nullptr)
		{
			// �ִٸ� ���׸��� �־��ش�..
				// ����� �������� ������ ��ü������ Deserialize�� �ϼ��Ǹ� �ڵ����� �־��� ��
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

		// ���� �־��ش�.
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
		// ���� ObjMesh�� �ִ��� Ȯ��
		if (resourceManager->GetMesh(objectID) != nullptr)
		{
			// �ִٸ� �ش� ID�� �־��ְ� �����Ѵ�.
			_objRes->meshID = objectID;
			_objRes->setMesh = true;

			return _objRes;
		}

		// CubeMapMesh ������
			// �⺻���� �ڽ�
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

		// ������� ���� �������� �޽ø� ������ݴϴ�.
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
