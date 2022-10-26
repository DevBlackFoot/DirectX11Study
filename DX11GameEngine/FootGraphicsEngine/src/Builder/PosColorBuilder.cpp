#include "GraphicsPch.h"
#include "Builder/PosColorBuilder.h"

#include "Object/IDXObject.h"
#include "Object/PosColorObj.h"
#include "GraphicsCore/RasterizerState.h"

namespace GraphicsEngineSpace
{
	std::shared_ptr<IDXObject> PosColorBuilder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName)
	{
		// ���� ������Ʈ Ȯ��
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

	// ���� ������Ʈ�� �̸��� ���� ������ �ٸ��� ���ݴϴ�.
	std::shared_ptr<ObjectResources> PosColorBuilder::BuildGeometry(std::string objectName)
	{
		// ������Ʈ �̸��� Grid, Axis�� ���..
		std::shared_ptr<ObjectResources> _tmpObjRes = std::make_shared<ObjectResources>();
		// ���� �̸��� �߰����ش�.
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
		// for���� ���鼭 ���ؽ� ����.
			// ���� ���ؽ�
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

		// �ε��� ���� ����
			// �� ������ ���� 40�� => �ε����� ���� 40 * 2��
		UINT indices[84];

		for (int i = 0; i < 84; i++)
		{
			indices[i] = i;
		}

		_objRes->IndexCount = sizeof(indices) / sizeof(UINT);

		// ���� ����ü ����.
		// ���ؽ� ����
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

		// �ٸ� �κе� ä���ֱ�.
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
			// x�� => ������
			{XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)},
			{XMFLOAT3(15.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)},

			// y�� �ʷϻ�
			{XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::GreenYellow)},
			{XMFLOAT3(0.0f, 15.0f, 0.0f), XMFLOAT4((const float*)&Colors::GreenYellow)},

			// z�� �Ķ���
			{XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Blue)},
			{XMFLOAT3(0.0f, 0.0f, 15.0f), XMFLOAT4((const float*)&Colors::Blue)},
		};


		// �ε��� ���� ����
			// �� ������ ���� 40�� => �ε����� ���� 40 * 2��
		UINT indices[] =
		{
			0, 1,

			2, 3,

			4, 5
		};

		_objRes->IndexCount = sizeof(indices) / sizeof(UINT);

		// ���� ����ü ����.
		// ���ؽ� ����
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

		// �ε��� ����
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

		// �ٸ� �κе� ä���ֱ�.
		_objRes->IndexOffset = 0;
		_objRes->VertexOffset = 0;
		_objRes->RasterizerState = RasterizerState::GetWireFrameRS();
		_objRes->Topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		_objRes->DiffuseMapPathName = L"NULL";

		return _objRes;
	}

}