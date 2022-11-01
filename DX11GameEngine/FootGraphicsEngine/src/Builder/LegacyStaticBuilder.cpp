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
		// 들어온 오브젝트가 빌더에 맞는 오브젝트인지 확인한다 아니면 리턴
		if (std::dynamic_pointer_cast<LegacyStaticObj>(pDXObj) == nullptr)
		{
			return nullptr;
		}

		// DXObj에 지오메트리 정보들을 넣어준다.
		pDXObj->SetObjectResources(BuildGeometry(objectName, objectID));

		// 그 뒤 리턴.
		return pDXObj;
	}

	// 들어온 오브젝트의 이름에 따라.. 세팅을 다르게 해줍니다.
		// 각 경우에 맞춰서 새 리소스들을 설정해줍니다.
	std::shared_ptr<ObjectResources> LegacyStaticBuilder::BuildGeometry(std::string objectName, uint64 objectID)
	{
		std::shared_ptr<ObjectResources> _tmpObjRes = std::make_shared<ObjectResources>();

		// 또한 이름을 추가해준다.
		_tmpObjRes->ObjName = objectName;

		// 오브젝트 이름이 ASE로 끝날 경우. ASE리소스를 만들어줍니다.
		// 확장자를 판별합니다.
		std::string extension = objectName;
		int _extensionIdx = objectName.rfind(".");
		if (_extensionIdx != -1)
		{
			extension = objectName.substr(_extensionIdx);
		}

		if (extension == ".ASE" || extension == ".ase")
		{
			// 파서로 해당 파일을 읽습니다.
			meshASEParser->Load(const_cast<char*>(objectName.c_str()));
			// ASE 파일 하나 읽을 때마다 Idx를 1씩 올려준다.
				// 현재 이 코드는 하나의 ASE 파일에 하나의 Geom이 있다고 가정하고 있다
				// 다른 구조를 찾으면 좋을 것 이다..
				// 하나의 오브젝트를 만들때 파서의 내용을 지우고 다시 로드 하는 식으로..?
				// 그리고 Geomobj를 만들 때 파서 안의 모든 오브젝트를 읽는 식으로 할 수 있을 것 같다..
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

	void LegacyStaticBuilder::InitBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pDC)
	{
		D3DDevice = pDevice;
		D3DDeviceContext = pDC;

		// 여기서 파서를 초기화 해줍시다.
		meshASEParser = new CASEParser;
		meshASEParser->Init();

		// 리소스 매니저 캐싱
		resourceManager = ResourceManager::GetInstance();
	}

	// ASE 파일중 Basic32를 사용하는 녀석들을 생성.. 텍스쳐 파일을 받아오는 새 함수를 파야겠다..
	std::shared_ptr<ObjectResources> LegacyStaticBuilder::BuildMeshASEResources(std::shared_ptr<ObjectResources> _objRes, uint64 objectID)
	{
		// 여기의 objectID는 수동으로 넣어주는 것이기 때문에 다른 메시가 들어갈 확률이 없다.
		if (resourceManager->GetMesh(objectID) != nullptr)
		{
			_objRes->materialID = objectID;
			_objRes->setMesh = true;

			return _objRes;
		}

		// 일단 현재 인덱스의 GeomObj를 최적화 합니다.
		if (meshASEParser->Convert_Optimize(meshASEParser->GetGeomObj(geomIdx)) != true)
		{
			return nullptr;
		}

#pragma region Geometry
		// 지오메트리 정보를 받아옵니다.
		ASEParser::GeomObject* _meshData = meshASEParser->GetGeomObj(geomIdx);

		// 버텍스 정보 입력
		std::vector<VertexStruct::NormalVertex> vertices;
		for (int i = 0; i < _meshData->m_opt_vertex.size(); i++)
		{
			auto _tmpVertex = make_unique<VertexStruct::NormalVertex>();
			_tmpVertex->Pos = _meshData->m_opt_vertex[i]->m_pos;
			_tmpVertex->Normal = _meshData->m_opt_vertex[i]->m_normal;
			_tmpVertex->Tex = XMFLOAT2(_meshData->m_opt_vertex[i]->u, 1 - _meshData->m_opt_vertex[i]->v);

			// 탄젠트 값을 계산해서 넣어줘야함..

			vertices.push_back(*_tmpVertex);
		}

		// 모든 삼각형에 대해서.. for문을 돌면서 tangent를 구해준다.
			// 탄젠트 값은 누산해야하기 때문에 임시변수를 하나 생성..
			// 인덱스를 넣어주면서
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

			// 삼각형을 이루는 각 버텍스의 포지션(월드 포지션)
			const Vector3& p0 = vertices[i0].Pos;
			const Vector3& p1 = vertices[i1].Pos;
			const Vector3& p2 = vertices[i2].Pos;

			// 삼각형을 이루는 각 버텍스의 텍스쳐위치
			const Vector2& w0 = vertices[i0].Tex;
			const Vector2& w1 = vertices[i1].Tex;
			const Vector2& w2 = vertices[i2].Tex;

			// 연립 방정식 풀기
				// 평면공간에서의 위치가 3차원에서는 어디에 존재하는가..
			Vector3 e1 = p1 - p0;
			Vector3 e2 = p2 - p0;
			float x1 = w1.x - w0.x;
			float x2 = w2.x - w0.x;
			float y1 = w1.y - w0.y;
			float y2 = w2.y - w0.y;

			// 역행렬 판별..
			float r = 1.0f / (x1 * y2 - y1 * x2);
			Vector3 t = (e1 * y2 - e2 * y1) * r;

			Tangent[i0] += t;
			Tangent[i1] += t;
			Tangent[i2] += t;
		}

		// 나온 탄젠트 값을 넣어준다..
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
		// 다른 부분들 채워주기.
		// 리스트에서 어떻게 얘를 찾아올 수 있을까.. => 일단 얘는 그냥 상자로.
		//_objRes->DiffuseMapPathName = L"../Textures/WoodCrate01.dds";

		return _objRes;
	}

}
