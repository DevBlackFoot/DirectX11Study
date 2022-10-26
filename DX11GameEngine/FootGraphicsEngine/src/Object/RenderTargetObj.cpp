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
		// ���� Ȯ��
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

		// ���̴� SetUp
		ShaderManager::GetInstance()->GetShader("TextureRectVS")->SetUpShader();
		ShaderManager::GetInstance()->GetShader("TextureRectPS")->SetUpShader();

		// �������� ����
		D3DDeviceContext->IASetPrimitiveTopology(objectResources->Topology);

		// ���ؽ� ���� �ε��� ���� ����.
		UINT stride = sizeof(VertexStruct::TexVertex);
		UINT offset = 0;
		D3DDeviceContext->IASetVertexBuffers(0, 1, &objectResources->VB, &stride, &offset);
		D3DDeviceContext->IASetIndexBuffer(objectResources->IB, DXGI_FORMAT_R32_UINT, 0);

		// ��� ���� ����.
		// �����ʹ� constBuffer�� �������� ����ü�� ��������.
		cbMatrix cbPerObj;
		cbPerObj.worldViewProj = world * view * proj;
		BufferManager::GetInstance()->GetBuffer("MatrixCB")->SetUpBuffer(0, &cbPerObj, ShaderType::VERTEX);

		// �ȼ� ���̴� ���� ����
		ID3D11SamplerState* sampler = SamplerManager::GetInstance()->GetWrapSamplerState();
		D3DDeviceContext->PSSetSamplers(0, 1, &sampler);

		D3DDeviceContext->PSSetShaderResources(0, 1, &(objectResources->RenderTargetTexture));

		D3DDeviceContext->RSSetState(objectResources->RasterizerState);

		// �ش� ���� �׸���.
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
