#include "GraphicsPch.h"
#include "Builder/Basic32Builder.h"

#include "Object/IDXObject.h"
#include "Object/LegacyObj.h"
#include "GraphicsCore/RasterizerState.h"
#include "ASEParser/CASEParser.h"

namespace GraphicsEngineSpace
{
	Basic32Builder::~Basic32Builder()
	{
		SafeDelete(m_MeshASEParser);
	}

	std::shared_ptr<IDXObject> Basic32Builder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName)
	{
		// ���� ������Ʈ�� ������ �´� ������Ʈ���� Ȯ���Ѵ� �ƴϸ� ����
		if (std::dynamic_pointer_cast<LegacyObj>(pDXObj) == nullptr)
		{
			return nullptr;
		}

		// DXObj�� ������Ʈ�� �������� �־��ش�.
		pDXObj->SetObjectResources(BuildGeometry(objectName));

		// �� �� ����.
		return pDXObj;
	}

	std::shared_ptr<IDXObject> Basic32Builder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, std::wstring _texFile)
	{
		// ���� ������Ʈ�� ������ �´� ������Ʈ���� Ȯ���Ѵ� �ƴϸ� ����
		if (std::dynamic_pointer_cast<LegacyObj>(pDXObj) == nullptr)
		{
			return nullptr;
		}

		// DXObj�� ������Ʈ�� �������� �־��ش�. �� �� _TexFile�� ���� �����ش�..
		auto objRes = BuildGeometry(objectName);
		objRes->DiffuseMapPathName = _texFile;
		pDXObj->SetObjectResources(objRes);

		// �� �� ����.
		return pDXObj;
	}

	std::shared_ptr<IDXObject> Basic32Builder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName,
	                                                         std::wstring diffuseMap, std::wstring normalMap)
	{
		if (std::dynamic_pointer_cast<LegacyObj>(pDXObj) == nullptr)
			return nullptr;

		// �� �� ���� �Լ� ����� ���δ�.
		auto objRes = BuildGeometry(objectName);
		objRes->DiffuseMapPathName = diffuseMap;
		objRes->NormalMapPathName = normalMap;

		pDXObj->SetObjectResources(objRes);

		return pDXObj;
	}

	// ���� ������Ʈ�� �̸��� ����.. ������ �ٸ��� ���ݴϴ�.
		// �� ��쿡 ���缭 �� ���ҽ����� �������ݴϴ�.
		std::shared_ptr<ObjectResources> Basic32Builder::BuildGeometry(std::string objectName)
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
			m_MeshASEParser->Load(const_cast<char*>(objectName.c_str()));
			// ASE ���� �ϳ� ���� ������ Idx�� 1�� �÷��ش�.
				// ���� �� �ڵ�� �ϳ��� ASE ���Ͽ� �ϳ��� Geom�� �ִٰ� �����ϰ� �ִ�
				// �ٸ� ������ ã���� ���� �� �̴�..
				// �ϳ��� ������Ʈ�� ���鶧 �ļ��� ������ ����� �ٽ� �ε� �ϴ� ������..?
				// �׸��� Geomobj�� ���� �� �ļ� ���� ��� ������Ʈ�� �д� ������ �� �� ���� �� ����..
			m_GeomIdx++;

			_tmpObjRes = BuildMeshASEResources(_tmpObjRes);
		}
		else
		{
			_tmpObjRes.reset();
			return nullptr;
		}

		return _tmpObjRes;
	}

	void Basic32Builder::InitBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pDC)
	{
		m_D3DDevice = pDevice;
		m_D3DDeviceContext = pDC;

		// ���⼭ �ļ��� �ʱ�ȭ ���ݽô�.
		m_MeshASEParser = new CASEParser;
		m_MeshASEParser->Init();
	}

	// �ڽ��� �ش��ϴ� ���� ���� ����.
	/*ObjectResources* Basic32Builder::BuildBoxResources(ObjectResources* _objRes)
	{
#pragma region Geometry
		VertexStruct::Basic32 vertices[] =
		{
			// �ո�. x �� 1
				// box ���ؽ� / vertices������ �ε���
				// �ϴ� �븻�� 0, 0, 0 ���� ������ش�.
			{XMFLOAT3(1.0f, 2.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)}, // 0, 0
			{XMFLOAT3(1.0f, 0.0f, -1.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)}, // 1, 1
			{XMFLOAT3(1.0f, 0.0f, 1.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)}, // 2, 2
			{XMFLOAT3(1.0f, 2.0f, 1.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)}, // 3, 3

			// �ո�. z �� 1
			{XMFLOAT3(1.0f, 2.0f, 1.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)}, // 4, 4
			{XMFLOAT3(1.0f, 0.0f, 1.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)}, // 5, 5
			{XMFLOAT3(-1.0f, 0.0f, 1.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)}, // 1, 6
			{XMFLOAT3(-1.0f, 2.0f, 1.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)}, // 0, 7

			// ����, y�� 1
			{XMFLOAT3(-1.0f, 2.0f, -1.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)}, // 7, 8
			{XMFLOAT3(1.0f, 2.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT2(0.0f, 1.0f)}, // 4, 9
			{XMFLOAT3(1.0f, 2.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT2(1.0f, 1.0f)}, // 0, 10
			{XMFLOAT3(-1.0f, 2.0f, 1.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)}, // 3, 11

			// �޸� x�� -1
			{XMFLOAT3(-1.0f, 2.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT2(0.0f, 0.0f)}, // 7, 12
			{XMFLOAT3(-1.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)}, // 6, 13
			{XMFLOAT3(-1.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT2(1.0f, 1.0f)}, // 5, 14
			{XMFLOAT3(-1.0f, 2.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)}, // 4, 15

			// �޸� z�� -1
			{XMFLOAT3(-1.0f, 2.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)}, // 3, 16
			{XMFLOAT3(-1.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)}, // 2, 17
			{XMFLOAT3(1.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)}, // 6, 18
			{XMFLOAT3(1.0f, 2.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)}, // 7, 19

			// �Ʒ���, y�� 0
			{XMFLOAT3(-1.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)}, // 6, 20
			{XMFLOAT3(-1.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)}, // 5, 21
			{XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)}, // 1, 22
			{XMFLOAT3(1.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT2(1.0f, 0.0f)} // 2, 23
		};

		// �ε��� ����
		UINT indices[] = {
			// �ո� : x�� 1
			0, 1, 3,
			1, 2, 3,

			// �ո� : z�� 1
			4, 5, 7,
			5, 6, 7,

			// �� �� : y�� 2
			8, 9, 11,
			9, 10, 11,

			// �� �� : x�� -1
			12, 13, 15,
			13, 14, 15,

			// �� �� : z�� -1
			16, 17, 19,
			17, 18, 19,

			// �Ʒ� �� : y�� 0
			20, 21, 23,
			21, 22, 23
		};

		// �ε��� ����
		_objRes->IndexCount = sizeof(indices) / sizeof(UINT);
#pragma endregion

#pragma region Calculate Vertex Normal
		// �븻 ����� �ٽ� �־��ֱ�.
		int numTriangles = _objRes->IndexCount / 3;
		for (UINT i = 0; i < numTriangles; i++)
		{
			// i��° �ﰢ���� �ε�����
			UINT i0 = indices[i * 3 + 0];
			UINT i1 = indices[i * 3 + 1];
			UINT i2 = indices[i * 3 + 2];

			// i��° �ﰢ���� ���ؽ���
			VertexStruct::Basic32 v0 = vertices[i0];
			VertexStruct::Basic32 v1 = vertices[i1];
			VertexStruct::Basic32 v2 = vertices[i2];

			// ����� ���.
			// ���Ϳ� �� �־��ֱ�
			XMVECTOR v0Vector = XMLoadFloat3(&v0.Pos);
			XMVECTOR v1Vector = XMLoadFloat3(&v1.Pos);
			XMVECTOR v2Vector = XMLoadFloat3(&v2.Pos);

			XMVECTOR e0 = v1Vector - v0Vector;
			XMVECTOR e1 = v2Vector - v0Vector;
			XMVECTOR faceNormalVector = XMVector3Cross(e0, e1);

			// �ش� face�븻�� XMFLOAT�������� ��ȯ.
			XMFLOAT3 faceNormal;
			XMStoreFloat3(&faceNormal, faceNormalVector);

			// �� ������ �� ������ ������ ����.
			vertices[i0].Normal.x += faceNormal.x;
			vertices[i0].Normal.y += faceNormal.y;
			vertices[i0].Normal.z += faceNormal.z;

			vertices[i1].Normal.x += faceNormal.x;
			vertices[i1].Normal.y += faceNormal.y;
			vertices[i1].Normal.z += faceNormal.z;

			vertices[i2].Normal.x += faceNormal.x;
			vertices[i2].Normal.y += faceNormal.y;
			vertices[i2].Normal.z += faceNormal.z;
		}

		// ������ �븻 �����ֱ�.
		UINT numVertices = sizeof(vertices) / sizeof(VertexStruct::Basic32);
		for (UINT i = 0; i < numVertices; i++)
		{
			// ����ȭ�� ���� XMVECTOR�� �ϳ� ������ش�.
			XMVECTOR normalize = XMLoadFloat3(&vertices[i].Normal);
			normalize = XMVector3Normalize(normalize);

			XMStoreFloat3(&vertices[i].Normal, normalize);
		}
#pragma endregion

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
		_objRes->RasterizerState = RasterizerState::GetSolidRS();
		_objRes->Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		_objRes->DiffuseMapPathName = L"../Textures/WoodCrate02.dds";

		return _objRes;
	}*/

	// ASE ������ Basic32�� ����ϴ� �༮���� ����.. �ؽ��� ������ �޾ƿ��� �� �Լ��� �ľ߰ڴ�..
	std::shared_ptr<ObjectResources> Basic32Builder::BuildMeshASEResources(std::shared_ptr<ObjectResources> _objRes)
	{
		// �ϴ� ���� �ε����� GeomObj�� ����ȭ �մϴ�.
		if (!m_MeshASEParser->Convert_Optimize(m_MeshASEParser->GetGeomObj(m_GeomIdx)))
		{
			return nullptr;
		}

#pragma region Geometry
		// ������Ʈ�� ������ �޾ƿɴϴ�.
		ASEParser::GeomObject* _meshData = m_MeshASEParser->GetGeomObj(m_GeomIdx);

		// ���ؽ� ���� �Է�
		std::vector<VertexStruct::NormalVertex> vertices;
		for (int i = 0; i < _meshData->m_opt_vertex.size(); i++)
		{
			auto _tmpVertex = make_unique<VertexStruct::NormalVertex>();
			_tmpVertex->Pos = _meshData->m_opt_vertex[i]->m_pos;
			_tmpVertex->Normal = _meshData->m_opt_vertex[i]->m_normal;
			_tmpVertex->Tex = XMFLOAT2(_meshData->m_opt_vertex[i]->u,  1 - _meshData->m_opt_vertex[i]->v);

			// ź��Ʈ ���� ����ؼ� �־������..

			vertices.push_back(*_tmpVertex);
		}

		// ��� �ﰢ���� ���ؼ�.. for���� ���鼭 tangent�� �����ش�.
			// ź��Ʈ ���� �����ؾ��ϱ� ������ �ӽú����� �ϳ� ����..
			// �ε����� �־��ָ鼭
		vector<Vector3> Tangent(vertices.size());

		_objRes->IndexCount = 3 * _meshData->m_meshface.size();
		vector<UINT> indices(_objRes->IndexCount);
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

		// ���� ����ü ����
		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(VertexStruct::NormalVertex) * vertices.size();
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vInitData;
		vInitData.pSysMem = vertices.data();
		HR(m_D3DDevice->CreateBuffer(&vbd, &vInitData, &_objRes->VB));

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * _objRes->IndexCount;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA iInitData;
		iInitData.pSysMem = indices.data();

		HR(m_D3DDevice->CreateBuffer(&ibd, &iInitData, &_objRes->IB));

		// �ٸ� �κе� ä���ֱ�.
		_objRes->IndexOffset = 0;
		_objRes->VertexOffset = 0;
		_objRes->RasterizerState = RasterizerState::GetSolidRS();
		_objRes->Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		// ����Ʈ���� ��� �긦 ã�ƿ� �� ������.. => �ϴ� ��� �׳� ���ڷ�.
		//_objRes->DiffuseMapPathName = L"../Textures/WoodCrate01.dds";

		return _objRes;
	}

}
