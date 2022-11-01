#include "GraphicsPch.h"
#include "Builder/LegacyStaticBuilder.h"

#include "Object/IDXObject.h"
#include "Object/LegacyStaticObj.h"
#include "GraphicsCore/RasterizerState.h"
#include "ASEParser/CASEParser.h"

#include "Manager/ResourceManager.h"
#include <typeinfo>

namespace GraphicsEngineSpace
{
	LegacyStaticBuilder::~LegacyStaticBuilder()
	{
		SafeDelete(meshASEParser);
	}

	std::shared_ptr<IDXObject> LegacyStaticBuilder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, uint64 objectID)
	{
		// ���� ������Ʈ�� ������ �´� ������Ʈ���� Ȯ���Ѵ� �ƴϸ� ����
		if (std::dynamic_pointer_cast<LegacyStaticObj>(pDXObj) == nullptr)
		{
			return nullptr;
		}

		// DXObj�� ������Ʈ�� �������� �־��ش�.
		pDXObj->SetObjectResources(BuildGeometry(objectName, objectID));

		// �� �� ����.
		return pDXObj;
	}

	// ���� ������Ʈ�� �̸��� ����.. ������ �ٸ��� ���ݴϴ�.
		// �� ��쿡 ���缭 �� ���ҽ����� �������ݴϴ�.
	std::shared_ptr<ObjectResources> LegacyStaticBuilder::BuildGeometry(std::string objectName, uint64 objectID)
	{
		std::shared_ptr<ObjectResources> _tmpObjRes = std::make_shared<ObjectResources>();

		// ���� �̸��� �߰����ش�.
		_tmpObjRes->ObjName = objectName;

		// ������Ʈ �̸��� ASE�� ���� ���. ASE���ҽ��� ������ݴϴ�.
		// Ȯ���ڸ� �Ǻ��մϴ�.
		std::string extension = objectName;
		int _extensionIdx = objectName.rfind(".");
		if (_extensionIdx != -1)
		{
			extension = objectName.substr(_extensionIdx);
		}

		if (extension == ".ASE" || extension == ".ase")
		{
			// �ļ��� �ش� ������ �н��ϴ�.
			meshASEParser->Load(const_cast<char*>(objectName.c_str()));
			// ASE ���� �ϳ� ���� ������ Idx�� 1�� �÷��ش�.
				// ���� �� �ڵ�� �ϳ��� ASE ���Ͽ� �ϳ��� Geom�� �ִٰ� �����ϰ� �ִ�
				// �ٸ� ������ ã���� ���� �� �̴�..
				// �ϳ��� ������Ʈ�� ���鶧 �ļ��� ������ ����� �ٽ� �ε� �ϴ� ������..?
				// �׸��� Geomobj�� ���� �� �ļ� ���� ��� ������Ʈ�� �д� ������ �� �� ���� �� ����..
			geomIdx++;

			_tmpObjRes = BuildMeshASEResources(_tmpObjRes, objectID);
		}
		else
		{
			_tmpObjRes.reset();
			return nullptr;
		}

		return _tmpObjRes;
	}

	std::shared_ptr<IDXObject> LegacyStaticBuilder::AddTexture(std::shared_ptr<IDXObject> DXObj, uint64 textureID,
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

	void LegacyStaticBuilder::InitBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pDC)
	{
		D3DDevice = pDevice;
		D3DDeviceContext = pDC;

		// ���⼭ �ļ��� �ʱ�ȭ ���ݽô�.
		meshASEParser = new CASEParser;
		meshASEParser->Init();

		// ���ҽ� �Ŵ��� ĳ��
		resourceManager = ResourceManager::GetInstance();
	}

	// ASE ������ Basic32�� ����ϴ� �༮���� ����.. �ؽ��� ������ �޾ƿ��� �� �Լ��� �ľ߰ڴ�..
	std::shared_ptr<ObjectResources> LegacyStaticBuilder::BuildMeshASEResources(std::shared_ptr<ObjectResources> _objRes, uint64 objectID)
	{
		// ������ objectID�� �������� �־��ִ� ���̱� ������ �ٸ� �޽ð� �� Ȯ���� ����.
		if (resourceManager->GetMesh(objectID) != nullptr)
		{
			_objRes->materialID = objectID;
			_objRes->setMesh = true;

			return _objRes;
		}

		// �ϴ� ���� �ε����� GeomObj�� ����ȭ �մϴ�.
		if (meshASEParser->Convert_Optimize(meshASEParser->GetGeomObj(geomIdx)) != true)
		{
			return nullptr;
		}

#pragma region Geometry
		// ������Ʈ�� ������ �޾ƿɴϴ�.
		ASEParser::GeomObject* _meshData = meshASEParser->GetGeomObj(geomIdx);

		// ���ؽ� ���� �Է�
		std::vector<VertexStruct::NormalVertex> vertices;
		for (int i = 0; i < _meshData->m_opt_vertex.size(); i++)
		{
			auto _tmpVertex = make_unique<VertexStruct::NormalVertex>();
			_tmpVertex->Pos = _meshData->m_opt_vertex[i]->m_pos;
			_tmpVertex->Normal = _meshData->m_opt_vertex[i]->m_normal;
			_tmpVertex->Tex = XMFLOAT2(_meshData->m_opt_vertex[i]->u, 1 - _meshData->m_opt_vertex[i]->v);

			// ź��Ʈ ���� ����ؼ� �־������..

			vertices.push_back(*_tmpVertex);
		}

		// ��� �ﰢ���� ���ؼ�.. for���� ���鼭 tangent�� �����ش�.
			// ź��Ʈ ���� �����ؾ��ϱ� ������ �ӽú����� �ϳ� ����..
			// �ε����� �־��ָ鼭
		vector<Vector3> Tangent(vertices.size());

		int indexCount = 3 * _meshData->m_meshface.size();
		vector<UINT> indices(indexCount);
		for (int i = 0; i < _meshData->m_meshface.size(); i++)
		{
			indices[i * 3 + 0] = _meshData->m_opt_index[i].index[0];
			indices[i * 3 + 1] = _meshData->m_opt_index[i].index[2];
			indices[i * 3 + 2] = _meshData->m_opt_index[i].index[1];

			int i0 = _meshData->m_opt_index[i].index[0];
			int i1 = _meshData->m_opt_index[i].index[2];
			int i2 = _meshData->m_opt_index[i].index[1];

			// �ﰢ���� �̷�� �� ���ؽ��� ������(���� ������)
			const Vector3& p0 = vertices[i0].Pos;
			const Vector3& p1 = vertices[i1].Pos;
			const Vector3& p2 = vertices[i2].Pos;

			// �ﰢ���� �̷�� �� ���ؽ��� �ؽ�����ġ
			const Vector2& w0 = vertices[i0].Tex;
			const Vector2& w1 = vertices[i1].Tex;
			const Vector2& w2 = vertices[i2].Tex;

			// ���� ������ Ǯ��
				// ������������ ��ġ�� 3���������� ��� �����ϴ°�..
			Vector3 e1 = p1 - p0;
			Vector3 e2 = p2 - p0;
			float x1 = w1.x - w0.x;
			float x2 = w2.x - w0.x;
			float y1 = w1.y - w0.y;
			float y2 = w2.y - w0.y;

			// ����� �Ǻ�..
			float r = 1.0f / (x1 * y2 - y1 * x2);
			Vector3 t = (e1 * y2 - e2 * y1) * r;

			Tangent[i0] += t;
			Tangent[i1] += t;
			Tangent[i2] += t;
		}

		// ���� ź��Ʈ ���� �־��ش�..
		for (int i = 0; i < vertices.size(); i++)
		{
			Tangent[i].Normalize();
			vertices[i].TangentU = Tangent[i];
		}

#pragma endregion

		_objRes->meshID = resourceManager->CreateMesh(
			vertices,
			indices,
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			RasterizerState::GetSolidRS(),
			objectID);

		_objRes->setMesh = true;
		// �ٸ� �κе� ä���ֱ�.
		// ����Ʈ���� ��� �긦 ã�ƿ� �� ������.. => �ϴ� ��� �׳� ���ڷ�.
		//_objRes->DiffuseMapPathName = L"../Textures/WoodCrate01.dds";

		return _objRes;
	}

}
