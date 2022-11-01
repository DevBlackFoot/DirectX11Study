#include "GraphicsPch.h"
#include "Builder/LineBuilder.h"

#include "Object/IDXObject.h"
#include "Object/LineObj.h"
#include "GraphicsCore/RasterizerState.h"
#include "Manager/ResourceManager.h"

namespace GraphicsEngineSpace
{
	std::shared_ptr<IDXObject> LineBuilder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, uint64 objectID)
	{
		// 들어온 오브젝트 확인
		if (std::dynamic_pointer_cast<LineObj>(pDXObj) == nullptr)
		{
			return nullptr;
		}

		pDXObj->SetObjectResources(BuildGeometry(objectName, objectID));

		return pDXObj;
	}

	// 들어온 오브젝트의 이름에 따라 세팅을 다르게 해줍니다.
	std::shared_ptr<ObjectResources> LineBuilder::BuildGeometry(std::string objectName, uint64 objectID)
	{
		// 오브젝트 이름이 Grid, Axis인 경우..
		std::shared_ptr<ObjectResources> _tmpObjRes = std::make_shared<ObjectResources>();
		// 또한 이름을 추가해준다.
		_tmpObjRes->ObjName = objectName;

		if (objectName == "Grid")
		{
			_tmpObjRes = BuildGridResources(_tmpObjRes, objectID);
		}
		else if (objectName == "Axis")
		{
			_tmpObjRes = BuildAxisResources(_tmpObjRes, objectID);
		}
		else
		{
			_tmpObjRes.reset();
			return nullptr;
		}

		return _tmpObjRes;
	}

	void LineBuilder::InitBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pDC)
	{
		D3DDevice = pDevice;
		D3DDeviceContext = pDC;

		// 리소스 매니저 바인딩
		resourceManager = ResourceManager::GetInstance();
	}

	// LineBuilder는 Texture를 사용하지 않습니다.
	std::shared_ptr<IDXObject> LineBuilder::AddTexture(std::shared_ptr<IDXObject> DXObj, uint64 textureID,
	                                                   std::string textureName, std::wstring path, RenderingData::TextureMapType mapType)
	{
		return nullptr;
	}

	std::shared_ptr<ObjectResources> LineBuilder::BuildGridResources(std::shared_ptr<ObjectResources> _objRes, uint64 objectID)
	{
		// 현재 ObjMesh가 있는지 확인
		if(resourceManager->GetMesh(objectID) != nullptr)
		{
			// 있다면 해당 ID를 넣어주고 리턴한다.
			_objRes->meshID = objectID;
			return _objRes;
		}

		//VertexStruct::ColorVertex vertices[84];
		std::vector<VertexStruct::ColorVertex> vertices(84);

		float z_val = -10.0f;
		// for문을 돌면서 버텍스 생성.
			// 세로 버텍스
		for (int i = 0; i < 42; i += 2)
		{
			vertices[i] = { XMFLOAT3(-10.0f, 0.0f, z_val), XMFLOAT4((const float*)&Colors::White) };
			vertices[i + 1] = { XMFLOAT3(10.0f, 0.0f, z_val), XMFLOAT4((const float*)&Colors::White) };
			z_val++;
		}
		float x_val = -10.0f;
		for (int i = 42; i < 84; i += 2)
		{
			vertices[i] = { XMFLOAT3(x_val, 0.0f, -10.0f), XMFLOAT4((const float*)&Colors::White) };
			vertices[i + 1] = { XMFLOAT3(x_val, 0.0f, 10.0f), XMFLOAT4((const float*)&Colors::White) };
			x_val++;
		}

		// 인덱스 버퍼 생성
			// 총 라인의 개수 40개 => 인덱스의 개수 40 * 2개
		//UINT indices[84];
		std::vector<UINT> indices(84);

		for (int i = 0; i < 84; i++)
		{
			indices[i] = i;
		}

		// 리소스 매니저에서 메시를 생성한뒤, 생성한 메시의 번호를 기억한다.
		_objRes->meshID = resourceManager->CreateMesh(
			vertices,
			indices,
			D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
			RasterizerState::GetWireFrameRS(),
			objectID);

		return _objRes;
	}

	std::shared_ptr<ObjectResources> LineBuilder::BuildAxisResources(std::shared_ptr<ObjectResources> _objRes, uint64 objectID)
	{
		// 현재 ObjMesh가 있는지 확인
		if (resourceManager->GetMesh(objectID) != nullptr)
		{
			// 있다면 해당 ID를 넣어주고 리턴한다.
			_objRes->meshID = objectID;
			return _objRes;
		}

		std::vector<VertexStruct::ColorVertex> vertices =
		{
			// x축 => 빨간색
			{XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)},
			{XMFLOAT3(15.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)},

			// y축 초록색
			{XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::GreenYellow)},
			{XMFLOAT3(0.0f, 15.0f, 0.0f), XMFLOAT4((const float*)&Colors::GreenYellow)},

			// z축 파란색
			{XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Blue)},
			{XMFLOAT3(0.0f, 0.0f, 15.0f), XMFLOAT4((const float*)&Colors::Blue)},
		};


		// 인덱스 버퍼 생성
			// 총 라인의 개수 40개 => 인덱스의 개수 40 * 2개
		std::vector<UINT> indices =
		{
			0, 1,

			2, 3,

			4, 5
		};

		_objRes->meshID = resourceManager->CreateMesh(
			vertices,
			indices,
			D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
			RasterizerState::GetWireFrameRS(),
			objectID);

		return _objRes;
	}

}