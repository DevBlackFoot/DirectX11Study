#include "GraphicsPch.h"
#include "Object/RenderTargetObj.h"

#include "Manager/ShaderManager.h"
#include "Object/Shader/ShaderBase.h"
#include "Manager/BufferManager.h"
#include "Manager/SamplerManager.h"
#include "Object/Buffer/BufferBase.h"

namespace GraphicsEngineSpace
{
	RenderTargetObj::RenderTargetObj()
		: D3DDevice(nullptr)
		, D3DDeviceContext(nullptr)
		, objectResources(nullptr)
		, world()
		, view()
		, proj()
	{
	}

	RenderTargetObj::~RenderTargetObj()
	{
		SafeReset(objectResources)
	}

	bool RenderTargetObj::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	{
		// 예외 확인
		assert(pDevice);
		assert(pDeviceContext);

		D3DDevice = pDevice;
		D3DDeviceContext = pDeviceContext;

		return true;
	}

	void RenderTargetObj::Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj)
	{
		world = _world;
		view = _view;
		proj = _proj;
	}

	void RenderTargetObj::LightUpdate(const DirectionalLight& _dirLit, const PointLight& _pointLit,
		const SpotLight& _spotLit, const XMFLOAT3& _eyePosW)
	{
		
	}

	void RenderTargetObj::Render()
	{
		assert(D3DDeviceContext);

		// 쉐이더 SetUp
		ShaderManager::GetInstance()->GetShader("TextureRectVS")->SetUpShader();
		ShaderManager::GetInstance()->GetShader("TextureRectPS")->SetUpShader();

		// 토폴로지 세팅
		D3DDeviceContext->IASetPrimitiveTopology(objectResources->Topology);

		// 버텍스 버퍼 인덱스 버퍼 세팅.
		UINT stride = sizeof(VertexStruct::TexVertex);
		UINT offset = 0;
		D3DDeviceContext->IASetVertexBuffers(0, 1, &objectResources->VB, &stride, &offset);
		D3DDeviceContext->IASetIndexBuffer(objectResources->IB, DXGI_FORMAT_R32_UINT, 0);

		// 상수 버퍼 세팅.
		// 데이터는 constBuffer와 닮은꼴의 구조체를 던져야함.
		cbMatrix cbPerObj;
		cbPerObj.worldViewProj = world * view * proj;
		BufferManager::GetInstance()->GetBuffer("MatrixCB")->SetUpBuffer(0, &cbPerObj, ShaderType::VERTEX);

		// 픽셀 셰이더 버퍼 세팅
		ID3D11SamplerState* sampler = SamplerManager::GetInstance()->GetWrapSamplerState();
		D3DDeviceContext->PSSetSamplers(0, 1, &sampler);

		D3DDeviceContext->PSSetShaderResources(0, 1, &(objectResources->RenderTargetTexture));

		D3DDeviceContext->RSSetState(objectResources->RasterizerState);

		// 해당 내용 그리기.
		D3DDeviceContext->DrawIndexed(objectResources->IndexCount, objectResources->IndexOffset, objectResources->VertexOffset);
	}

	void RenderTargetObj::SetObjectResources(std::shared_ptr<ObjectResources> objResources)
	{
		objectResources = objResources;
	}

	std::string RenderTargetObj::GetObjName()
	{
		return objectResources->ObjName;
	}
}
