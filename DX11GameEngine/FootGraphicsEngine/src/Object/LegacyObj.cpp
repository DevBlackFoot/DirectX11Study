#include "GraphicsPch.h"
#include "Object//LegacyObj.h"
#include "DirectXTex.h"

#include "Manager/BufferManager.h"
#include "Manager/ShaderManager.h"
#include "Manager/SamplerManager.h"

#include "Object/Shader/ShaderBase.h"
#include "Object/Buffer/BufferBase.h"


namespace GraphicsEngineSpace
{
	LegacyObj::LegacyObj()
		: D3DDevice(nullptr)
		, D3DDeviceContext(nullptr)
		, objectResources(nullptr)
		, diffuseMapSRV(nullptr)
		, normalMapSRV(nullptr)
		, mat({})
		, dirLight({})
		, pointLight({})
		, spotLight({})
		, eyePosW()
		, world()
		, view()
		, proj()
	{
	}

	LegacyObj::~LegacyObj()
	{
		SafeReset(objectResources);
	}

	bool LegacyObj::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	{
		// 예외 확인
		assert(pDevice);
		assert(pDeviceContext);

		D3DDevice = pDevice;
		D3DDeviceContext = pDeviceContext;

		// 머테리얼 초기화.
		mat.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 96.0f);

		// 텍스쳐 불러오기.
		InitTexture();

		// 초기화 할 때. 

		return true;
	}

	void LegacyObj::Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj)
	{
		// 행렬 넣기
		world = _world;
		view = _view;
		proj = _proj;
	}

	void LegacyObj::LightUpdate(const DirectionalLight& _dirLit, const PointLight& _pointLit, const SpotLight& _spotLit,
	                             const XMFLOAT3& _eyePosW)
	{
		dirLight = _dirLit;
		pointLight = _pointLit;
		spotLight = _spotLit;
		eyePosW = _eyePosW;
	}

	void LegacyObj::Render()
	{
		assert(D3DDeviceContext);

		// 쉐이더를 세팅합니다.
		ShaderManager::GetInstance()->GetShader("LegacyModelVS")->SetUpShader();
		ShaderManager::GetInstance()->GetShader("LegacyModelPS")->SetUpShader();

		D3DDeviceContext->IASetPrimitiveTopology(objectResources->Topology);

		UINT stride = sizeof(VertexStruct::NormalVertex);
		UINT offset = 0;
		D3DDeviceContext->IASetVertexBuffers(0, 1, &objectResources->VB, &stride, &offset);
		D3DDeviceContext->IASetIndexBuffer(objectResources->IB, DXGI_FORMAT_R32_UINT, 0);

		// 상수 버퍼 세팅
		cbMatrix cbPerObj;
		cbPerObj.worldViewProj = world * view * proj;
		cbPerObj.worldTransform = world;
		cbPerObj.worldInvTranspose = MathHelper::InverseTranspose(world);
		BufferManager::GetInstance()->GetBuffer("MatrixCB")->SetUpBuffer(0, &cbPerObj, ShaderType::VERTEX);

		// 픽셀 셰이더 상수 버퍼 세팅
		SimpleMath::Vector4 viewpos = SimpleMath::Vector4{eyePosW.x, eyePosW.y, eyePosW.z, 0.f};
		BufferManager::GetInstance()->GetBuffer("ViewPosCB")->SetUpBuffer(0, &viewpos, ShaderType::PIXEL);

		BufferManager::GetInstance()->GetBuffer("DLightCB")->SetUpBuffer(1, &dirLight, ShaderType::PIXEL);
		BufferManager::GetInstance()->GetBuffer("PLightCB")->SetUpBuffer(2, &pointLight, ShaderType::PIXEL);
		BufferManager::GetInstance()->GetBuffer("LegacyMaterialCB")->SetUpBuffer(3, &mat, ShaderType::PIXEL);

		// 텍스쳐 세팅
		D3DDeviceContext->PSSetShaderResources(0, 1, &diffuseMapSRV);
		D3DDeviceContext->PSSetShaderResources(1, 1, &normalMapSRV);

		// 샘플러 세팅. 만들어서 넣어주자..
			// 반드시 클래스로 빼자
		ID3D11SamplerState* sampler = SamplerManager::GetInstance()->GetWrapSamplerState();

		D3DDeviceContext->PSSetSamplers(0, 1, &sampler);

		// 렌더 스테이트 설정.
		D3DDeviceContext->RSSetState(objectResources->RasterizerState);

		// 해당 내용 그리기
		D3DDeviceContext->DrawIndexed(objectResources->IndexCount, objectResources->IndexOffset, objectResources->VertexOffset);
	}

	void LegacyObj::SetObjectResources(std::shared_ptr<ObjectResources> objResources)
	{
		objectResources = objResources;
	}

	std::string LegacyObj::GetObjName()
	{
		return objectResources->ObjName;
	}

	void LegacyObj::InitTexture()
	{
		//ID3D11Resource* texResource = nullptr;

		// TODO : Direct Tex 사용법 배우기. 파일에서 texture 읽어와야함. -> 그걸 Device에 붙여야함.
		/*HR(CreateDDSTextureFromFile(m_D3DDevice, m_ObjectResources->DiffuseMapPathName,
			&texResource, &m_DiffuseMapSRV));*/
		CreateSRV(objectResources->DiffuseMapPathName, &diffuseMapSRV);
		CreateSRV(objectResources->NormalMapPathName, &normalMapSRV);		

		// 같은 방식으로 노말맵도..

		//ReleaseCOM(texResource);
	}

	// 가장 간단한 텍스쳐 생성 및 SRV 생성 코드..
		// 이후 중복된 텍스쳐 생성을 막기 위해 Resource매니저 등으로 이전 필요.
	void LegacyObj::CreateSRV(std::wstring path, ID3D11ShaderResourceView** srv)
	{
		ScratchImage image;

		// 여기서 만들어 줘야함. 이후에 따로 클래스를 파는게 좋을 듯 하다.
		std::wstring ext = path;
		int _extensionIdx = ext.rfind(L".");
		if (_extensionIdx != -1)
		{
			ext = path.substr(_extensionIdx);
		}

		if (ext == L".dds" || ext == L".DDS")
		{
			::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, image);
		}
		else // 각종 Wiic 파일
		{
			::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, image);
		}

		ScratchImage mipChain;

		HRESULT hr;

		ID3D11Texture2D* texture2D;

		hr = ::CreateTexture(D3DDevice, image.GetImages(), image.GetImageCount(), image.GetMetadata(),
			reinterpret_cast<ID3D11Resource**>(&texture2D));

		if (FAILED(hr))
			::MessageBoxA(nullptr, "Texture Create Failed!", nullptr, MB_OK);

		// desc에 정보 넣어주기.
		D3D11_TEXTURE2D_DESC textureDesc;
		texture2D->GetDesc(&textureDesc);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		srvDesc.Format = textureDesc.Format;
		srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;

		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		
		hr = D3DDevice->CreateShaderResourceView(texture2D, &srvDesc, srv);

		ReleaseCOM(texture2D);
	}
}
