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
		// 들어온 오브젝트가 빌더에 맞는 오브젝트인지 확인한다 아니면 리턴
		if (std::dynamic_pointer_cast<LegacyObj>(pDXObj) == nullptr)
		{
			return nullptr;
		}

		// DXObj에 지오메트리 정보들을 넣어준다.
		pDXObj->SetObjectResources(BuildGeometry(objectName));

		// 그 뒤 리턴.
		return pDXObj;
	}

	std::shared_ptr<IDXObject> Basic32Builder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, std::wstring _texFile)
	{
		// 들어온 오브젝트가 빌더에 맞는 오브젝트인지 확인한다 아니면 리턴
		if (std::dynamic_pointer_cast<LegacyObj>(pDXObj) == nullptr)
		{
			return nullptr;
		}

		// DXObj에 지오메트리 정보들을 넣어준다. 이 때 _TexFile도 같이 보내준다..
		auto objRes = BuildGeometry(objectName);
		objRes->DiffuseMapPathName = _texFile;
		pDXObj->SetObjectResources(objRes);

		// 그 뒤 리턴.
		return pDXObj;
	}

	std::shared_ptr<IDXObject> Basic32Builder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName,
	                                                         std::wstring diffuseMap, std::wstring normalMap)
	{
		if (std::dynamic_pointer_cast<LegacyObj>(pDXObj) == nullptr)
			return nullptr;

		// 쓸 데 없는 함수 사용을 줄인다.
		auto objRes = BuildGeometry(objectName);
		objRes->DiffuseMapPathName = diffuseMap;
		objRes->NormalMapPathName = normalMap;

		pDXObj->SetObjectResources(objRes);

		return pDXObj;
	}

	// 들어온 오브젝트의 이름에 따라.. 세팅을 다르게 해줍니다.
		// 각 경우에 맞춰서 새 리소스들을 설정해줍니다.
		std::shared_ptr<ObjectResources> Basic32Builder::BuildGeometry(std::string objectName)
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
			m_MeshASEParser->Load(const_cast<char*>(objectName.c_str()));
			// ASE 파일 하나 읽을 때마다 Idx를 1씩 올려준다.
				// 현재 이 코드는 하나의 ASE 파일에 하나의 Geom이 있다고 가정하고 있다
				// 다른 구조를 찾으면 좋을 것 이다..
				// 하나의 오브젝트를 만들때 파서의 내용을 지우고 다시 로드 하는 식으로..?
				// 그리고 Geomobj를 만들 때 파서 안의 모든 오브젝트를 읽는 식으로 할 수 있을 것 같다..
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

		// 여기서 파서를 초기화 해줍시다.
		m_MeshASEParser = new CASEParser;
		m_MeshASEParser->Init();
	}

	// 박스에 해당하는 버퍼 등을 생성.
	/*ObjectResources* Basic32Builder::BuildBoxResources(ObjectResources* _objRes)
	{
#pragma region Geometry
		VertexStruct::Basic32 vertices[] =
		{
			// 앞면. x 축 1
				// box 버텍스 / vertices에서의 인덱스
				// 일단 노말은 0, 0, 0 으로 만들어준다.
			{XMFLOAT3(1.0f, 2.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)}, // 0, 0
			{XMFLOAT3(1.0f, 0.0f, -1.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)}, // 1, 1
			{XMFLOAT3(1.0f, 0.0f, 1.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)}, // 2, 2
			{XMFLOAT3(1.0f, 2.0f, 1.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)}, // 3, 3

			// 앞면. z 축 1
			{XMFLOAT3(1.0f, 2.0f, 1.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)}, // 4, 4
			{XMFLOAT3(1.0f, 0.0f, 1.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)}, // 5, 5
			{XMFLOAT3(-1.0f, 0.0f, 1.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)}, // 1, 6
			{XMFLOAT3(-1.0f, 2.0f, 1.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)}, // 0, 7

			// 윗면, y축 1
			{XMFLOAT3(-1.0f, 2.0f, -1.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)}, // 7, 8
			{XMFLOAT3(1.0f, 2.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT2(0.0f, 1.0f)}, // 4, 9
			{XMFLOAT3(1.0f, 2.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT2(1.0f, 1.0f)}, // 0, 10
			{XMFLOAT3(-1.0f, 2.0f, 1.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)}, // 3, 11

			// 뒷면 x축 -1
			{XMFLOAT3(-1.0f, 2.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT2(0.0f, 0.0f)}, // 7, 12
			{XMFLOAT3(-1.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)}, // 6, 13
			{XMFLOAT3(-1.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT2(1.0f, 1.0f)}, // 5, 14
			{XMFLOAT3(-1.0f, 2.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)}, // 4, 15

			// 뒷면 z축 -1
			{XMFLOAT3(-1.0f, 2.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)}, // 3, 16
			{XMFLOAT3(-1.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)}, // 2, 17
			{XMFLOAT3(1.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)}, // 6, 18
			{XMFLOAT3(1.0f, 2.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)}, // 7, 19

			// 아랫면, y축 0
			{XMFLOAT3(-1.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)}, // 6, 20
			{XMFLOAT3(-1.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)}, // 5, 21
			{XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)}, // 1, 22
			{XMFLOAT3(1.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT2(1.0f, 0.0f)} // 2, 23
		};

		// 인덱스 버퍼
		UINT indices[] = {
			// 앞면 : x축 1
			0, 1, 3,
			1, 2, 3,

			// 앞면 : z축 1
			4, 5, 7,
			5, 6, 7,

			// 윗 면 : y축 2
			8, 9, 11,
			9, 10, 11,

			// 뒷 면 : x축 -1
			12, 13, 15,
			13, 14, 15,

			// 뒷 면 : z축 -1
			16, 17, 19,
			17, 18, 19,

			// 아랫 면 : y축 0
			20, 21, 23,
			21, 22, 23
		};

		// 인덱스 개수
		_objRes->IndexCount = sizeof(indices) / sizeof(UINT);
#pragma endregion

#pragma region Calculate Vertex Normal
		// 노말 계산후 다시 넣어주기.
		int numTriangles = _objRes->IndexCount / 3;
		for (UINT i = 0; i < numTriangles; i++)
		{
			// i번째 삼각형의 인덱스들
			UINT i0 = indices[i * 3 + 0];
			UINT i1 = indices[i * 3 + 1];
			UINT i2 = indices[i * 3 + 2];

			// i번째 삼각형의 버텍스들
			VertexStruct::Basic32 v0 = vertices[i0];
			VertexStruct::Basic32 v1 = vertices[i1];
			VertexStruct::Basic32 v2 = vertices[i2];

			// 면법선 계산.
			// 벡터에 값 넣어주기
			XMVECTOR v0Vector = XMLoadFloat3(&v0.Pos);
			XMVECTOR v1Vector = XMLoadFloat3(&v1.Pos);
			XMVECTOR v2Vector = XMLoadFloat3(&v2.Pos);

			XMVECTOR e0 = v1Vector - v0Vector;
			XMVECTOR e1 = v2Vector - v0Vector;
			XMVECTOR faceNormalVector = XMVector3Cross(e0, e1);

			// 해당 face노말을 XMFLOAT형식으로 변환.
			XMFLOAT3 faceNormal;
			XMStoreFloat3(&faceNormal, faceNormalVector);

			// 면 법선을 각 정점의 법선에 누적.
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

		// 정점의 노말 구해주기.
		UINT numVertices = sizeof(vertices) / sizeof(VertexStruct::Basic32);
		for (UINT i = 0; i < numVertices; i++)
		{
			// 정규화를 위해 XMVECTOR를 하나 만들어준다.
			XMVECTOR normalize = XMLoadFloat3(&vertices[i].Normal);
			normalize = XMVector3Normalize(normalize);

			XMStoreFloat3(&vertices[i].Normal, normalize);
		}
#pragma endregion

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
		_objRes->RasterizerState = RasterizerState::GetSolidRS();
		_objRes->Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		_objRes->DiffuseMapPathName = L"../Textures/WoodCrate02.dds";

		return _objRes;
	}*/

	// ASE 파일중 Basic32를 사용하는 녀석들을 생성.. 텍스쳐 파일을 받아오는 새 함수를 파야겠다..
	std::shared_ptr<ObjectResources> Basic32Builder::BuildMeshASEResources(std::shared_ptr<ObjectResources> _objRes)
	{
		// 일단 현재 인덱스의 GeomObj를 최적화 합니다.
		if (!m_MeshASEParser->Convert_Optimize(m_MeshASEParser->GetGeomObj(m_GeomIdx)))
		{
			return nullptr;
		}

#pragma region Geometry
		// 지오메트리 정보를 받아옵니다.
		ASEParser::GeomObject* _meshData = m_MeshASEParser->GetGeomObj(m_GeomIdx);

		// 버텍스 정보 입력
		std::vector<VertexStruct::NormalVertex> vertices;
		for (int i = 0; i < _meshData->m_opt_vertex.size(); i++)
		{
			auto _tmpVertex = make_unique<VertexStruct::NormalVertex>();
			_tmpVertex->Pos = _meshData->m_opt_vertex[i]->m_pos;
			_tmpVertex->Normal = _meshData->m_opt_vertex[i]->m_normal;
			_tmpVertex->Tex = XMFLOAT2(_meshData->m_opt_vertex[i]->u,  1 - _meshData->m_opt_vertex[i]->v);

			// 탄젠트 값을 계산해서 넣어줘야함..

			vertices.push_back(*_tmpVertex);
		}

		// 모든 삼각형에 대해서.. for문을 돌면서 tangent를 구해준다.
			// 탄젠트 값은 누산해야하기 때문에 임시변수를 하나 생성..
			// 인덱스를 넣어주면서
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

		// 버퍼 구조체 생성
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

		// 다른 부분들 채워주기.
		_objRes->IndexOffset = 0;
		_objRes->VertexOffset = 0;
		_objRes->RasterizerState = RasterizerState::GetSolidRS();
		_objRes->Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		// 리스트에서 어떻게 얘를 찾아올 수 있을까.. => 일단 얘는 그냥 상자로.
		//_objRes->DiffuseMapPathName = L"../Textures/WoodCrate01.dds";

		return _objRes;
	}

}
