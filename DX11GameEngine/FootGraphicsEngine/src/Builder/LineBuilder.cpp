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
		// ���� ������Ʈ Ȯ��
		if (std::dynamic_pointer_cast<LineObj>(pDXObj) == nullptr)
		{
			return nullptr;
		}

		pDXObj->SetObjectResources(BuildGeometry(objectName, objectID));

		return pDXObj;
	}

	// ���� ������Ʈ�� �̸��� ���� ������ �ٸ��� ���ݴϴ�.
	std::shared_ptr<ObjectResources> LineBuilder::BuildGeometry(std::string objectName, uint64 objectID)
	{
		// ������Ʈ �̸��� Grid, Axis�� ���..
		std::shared_ptr<ObjectResources> _tmpObjRes = std::make_shared<ObjectResources>();
		// ���� �̸��� �߰����ش�.
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

		// ���ҽ� �Ŵ��� ���ε�
		resourceManager = ResourceManager::GetInstance();
	}

	// LineBuilder�� Texture�� ������� �ʽ��ϴ�.
	std::shared_ptr<IDXObject> LineBuilder::AddTexture(std::shared_ptr<IDXObject> DXObj, uint64 textureID,
	                                                   std::string textureName, std::wstring path, RenderingData::TextureMapType mapType)
	{
		return nullptr;
	}

	std::shared_ptr<ObjectResources> LineBuilder::BuildGridResources(std::shared_ptr<ObjectResources> _objRes, uint64 objectID)
	{
		// ���� ObjMesh�� �ִ��� Ȯ��
		if(resourceManager->GetMesh(objectID) != nullptr)
		{
			// �ִٸ� �ش� ID�� �־��ְ� �����Ѵ�.
			_objRes->meshID = objectID;
			return _objRes;
		}

		//VertexStruct::ColorVertex vertices[84];
		std::vector<VertexStruct::ColorVertex> vertices(84);

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
		//UINT indices[84];
		std::vector<UINT> indices(84);

		for (int i = 0; i < 84; i++)
		{
			indices[i] = i;
		}

		// ���ҽ� �Ŵ������� �޽ø� �����ѵ�, ������ �޽��� ��ȣ�� ����Ѵ�.
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
		// ���� ObjMesh�� �ִ��� Ȯ��
		if (resourceManager->GetMesh(objectID) != nullptr)
		{
			// �ִٸ� �ش� ID�� �־��ְ� �����Ѵ�.
			_objRes->meshID = objectID;
			return _objRes;
		}

		std::vector<VertexStruct::ColorVertex> vertices =
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