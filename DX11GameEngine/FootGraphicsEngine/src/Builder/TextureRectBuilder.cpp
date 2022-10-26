#include "GraphicsPch.h"
#include "Builder/TextureRectBuilder.h"

#include "Object/IDXObject.h"
#include "Object/RenderTargetObj.h"
#include "GraphicsCore/RasterizerState.h"

namespace GraphicsEngineSpace
{
	std::shared_ptr<IDXObject> TextureRectBuilder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj,
		std::string objectName)
	{
		return nullptr;
	}

	std::shared_ptr<IDXObject> TextureRectBuilder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj,
		std::string objectName, std::wstring _texFile)
	{
		return nullptr;
	}

	std::shared_ptr<IDXObject> TextureRectBuilder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj,
		std::string objectName, std::wstring diffuseMap, std::wstring normalMap)
	{
		return nullptr;
	}

	std::shared_ptr<IDXObject> TextureRectBuilder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj,
		std::string objectName, ID3D11ShaderResourceView* shaderResource)
	{
		// 들어온 오브젝트 확인
		if(std::dynamic_pointer_cast<RenderTargetObj>(pDXObj) == nullptr)
		{
			return nullptr;
		}

		std::shared_ptr<ObjectResources> objRes = BuildGeometry(objectName);

		objRes->RenderTargetTexture = shaderResource;

		pDXObj->SetObjectResources(objRes);

		return pDXObj;	
	}

	std::shared_ptr<ObjectResources> TextureRectBuilder::BuildGeometry(std::string objectName)
	{
		std::shared_ptr<ObjectResources> tmpObjRes = std::make_shared<ObjectResources>();

		tmpObjRes->ObjName = objectName;

		// 각종 버퍼 세팅
		VertexStruct::TexVertex vertices[4];

		// 여섯개짜리 평면
			// 위치 자체는 transform으로 옮길 수 있다.
		// 사실 여기서 사이즈를 위해 left, top 등을 설정해주어야 하지만..
		vertices[0] = {XMFLOAT3{0.0f, 0.f, 0.0f}, XMFLOAT2{0.0f, 0.0f}}; // left top
		vertices[1] = { XMFLOAT3{100.0f, 100.f, 0.0f}, XMFLOAT2{1.0f, 1.0f} }; // Bottom right
		vertices[2] = { XMFLOAT3{0.0f, 100.f, 0.0f}, XMFLOAT2{0.0f, 1.0f} }; // bottom Left

	//	vertices[3] = { XMFLOAT3{-100.0f, -100.f, 0.0f}, XMFLOAT2{0.0f, 0.0f} }; // Top left
		vertices[3] = { XMFLOAT3{100.0f, 0.f, 0.0f}, XMFLOAT2{1.0f, 0.0f} }; // Top right
	//	vertices[5] = { XMFLOAT3{100.0f, 100.f, 0.0f}, XMFLOAT2{1.0f, 1.0f} }; // bottom right

		UINT indices[6] =
		{
			0, 1, 2,
			0, 3, 1
		};

		tmpObjRes->IndexCount = sizeof(indices) / sizeof(UINT);

		// 버퍼 구조체 생성
		// 버텍스 버퍼
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
		HR(D3DDevice->CreateBuffer(&vbd, &vInitData, &tmpObjRes->VB));

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(indices);
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA iInitData;
		iInitData.pSysMem = indices;

		HR(D3DDevice->CreateBuffer(&ibd, &iInitData, &tmpObjRes->IB));

		// 다른 부분들 채워주기.
		tmpObjRes->IndexOffset = 0;
		tmpObjRes->VertexOffset = 0;
		tmpObjRes->RasterizerState = RasterizerState::GetWireFrameRS();
		tmpObjRes->Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		return tmpObjRes;
	}

	void TextureRectBuilder::InitBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pDC)
	{
		D3DDevice = pDevice;
		D3DDeviceContext = pDC;
	}
}
