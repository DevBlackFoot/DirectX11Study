#include "GraphicsPch.h"
#include "Builder/PosColorBuilder.h"

#include "Object/IDXObject.h"
#include "Object/PosColorObj.h"
#include "GraphicsCore/RasterizerState.h"

namespace GraphicsEngineSpace
{
	std::shared_ptr<IDXObject> PosColorBuilder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName)
	{
		// 들어온 오브젝트 확인
		if (std::dynamic_pointer_cast<PosColorObj>(pDXObj) == nullptr)
		{
			return nullptr;
		}

		pDXObj->SetObjectResources(BuildGeometry(objectName));

		return pDXObj;
	}

	std::shared_ptr<IDXObject> PosColorBuilder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, std::wstring _texFile)
	{
		return nullptr;
	}

	std::shared_ptr<IDXObject> PosColorBuilder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName,
	                                                          std::wstring diffuseMap, std::wstring normalMap)
	{
		return nullptr;
	}

	// 들어온 오브젝트의 이름에 따라 세팅을 다르게 해줍니다.
	std::shared_ptr<ObjectResources> PosColorBuilder::BuildGeometry(std::string objectName)
	{
		// 오브젝트 이름이 Grid, Axis인 경우..
		std::shared_ptr<ObjectResources> _tmpObjRes = std::make_shared<ObjectResources>();
		// 또한 이름을 추가해준다.
		_tmpObjRes->ObjName = objectName;

		if (objectName == "Grid")
		{
			_tmpObjRes = BuildGridResources(_tmpObjRes);
		}
		else if (objectName == "Axis")
		{
			_tmpObjRes = BuildAxisResources(_tmpObjRes);
		}
		else
		{
			_tmpObjRes.reset();
			return nullptr;
		}

		return _tmpObjRes;
	}

	void PosColorBuilder::InitBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pDC)
	{
		m_D3DDevice = pDevice;
		m_D3DDeviceContext = pDC;
	}

	std::shared_ptr<ObjectResources> PosColorBuilder::BuildGridResources(std::shared_ptr<ObjectResources> _objRes)
	{
		VertexStruct::ColorVertex vertices[84];

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
		UINT indices[84];

		for (int i = 0; i < 84; i++)
		{
			indices[i] = i;
		}

		_objRes->IndexCount = sizeof(indices) / sizeof(UINT);

		// 버퍼 구조체 생성.
		// 버텍스 버퍼
		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(vertices);
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vInitData;
		vInitData.pSysMem = vertices;
		HR(m_D3DDevice->CreateBuffer(&vbd, &vInitData, &_objRes->VB));

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(indices);
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA iInitData;
		iInitData.pSysMem = indices;

		HR(m_D3DDevice->CreateBuffer(&ibd, &iInitData, &_objRes->IB));

		// 다른 부분들 채워주기.
		_objRes->IndexOffset = 0;
		_objRes->VertexOffset = 0;
		_objRes->RasterizerState = RasterizerState::GetWireFrameRS();
		_objRes->Topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		_objRes->DiffuseMapPathName = L"NULL";

		return _objRes;
	}

	std::shared_ptr<ObjectResources> PosColorBuilder::BuildAxisResources(std::shared_ptr<ObjectResources> _objRes)
	{
		VertexStruct::ColorVertex vertices[] =
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
		UINT indices[] =
		{
			0, 1,

			2, 3,

			4, 5
		};

		_objRes->IndexCount = sizeof(indices) / sizeof(UINT);

		// 버퍼 구조체 생성.
		// 버텍스 버퍼
		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(vertices);
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vInitData;
		vInitData.pSysMem = vertices;
		HR(m_D3DDevice->CreateBuffer(&vbd, &vInitData, &_objRes->VB));

		// 인덱스 버퍼
		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(indices);
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA iInitData;
		iInitData.pSysMem = indices;

		HR(m_D3DDevice->CreateBuffer(&ibd, &iInitData, &_objRes->IB));

		// 다른 부분들 채워주기.
		_objRes->IndexOffset = 0;
		_objRes->VertexOffset = 0;
		_objRes->RasterizerState = RasterizerState::GetWireFrameRS();
		_objRes->Topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		_objRes->DiffuseMapPathName = L"NULL";

		return _objRes;
	}

}