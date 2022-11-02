#include "GraphicsPch.h"
#include "Object/SkyBox.h"

#include "Manager/ShaderManager.h"
#include "Manager/BufferManager.h"
#include "Manager/ResourceManager.h"
#include "Manager/SamplerManager.h"

#include "Object/Shader/ShaderBase.h"
#include "Object/Buffer/BufferBase.h"
#include "Resources/Texture.h"


namespace GraphicsEngineSpace
{
	GraphicsEngineSpace::SkyBox::SkyBox()
		: D3DDevice(nullptr)
		, D3DDeviceContext(nullptr)
		, objectResources(nullptr)
		, resourceManager(nullptr)
		, world()
		, view()
		, proj()
	{
	}

	SkyBox::~SkyBox()
	{
		SafeReset(objectResources)
	}

	bool SkyBox::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	{
		// ���� Ȯ��
		assert(pDevice);
		assert(pDeviceContext);

		D3DDevice = pDevice;
		D3DDeviceContext = pDeviceContext;

		resourceManager = ResourceManager::GetInstance();

		return true;
	}

	void SkyBox::Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj)
	{
		// ��� �ֱ�
		world = _world;
		view = _view;
		proj = _proj;
	}

	void SkyBox::LightUpdate(const DirectionalLight& _dirLit, const PointLight& _pointLit, const SpotLight& _spotLit,
		const XMFLOAT3& _eyePosW)
	{

	}

	void SkyBox::Render()
	{
		assert(D3DDeviceContext);
		assert(resourceManager->GetMesh(objectResources->meshID));
		assert(resourceManager->GetMaterial(objectResources->materialID));

		auto mesh = resourceManager->GetMesh(objectResources->meshID);
		auto material = resourceManager->GetMaterial(objectResources->materialID);

		// CubeMap����
		assert(resourceManager->GetTexture(material->cubeMap));

		auto cube = resourceManager->GetTexture(material->cubeMap);

		if (mesh == nullptr)
			return;

		// SkyBox Render
		// ���⼭ SkyBox Render
		// RasterizeState �� Depth State�� ��� �⺻������ ��å ����� �����ϰ� �Ǿ��ִ�..
		// ���̴� ����
		ShaderManager::GetInstance()->GetShader("SkyBoxVS")->SetUpShader();
		ShaderManager::GetInstance()->GetShader("SkyBoxPS")->SetUpShader();

		D3DDeviceContext->IASetPrimitiveTopology(mesh->GetPrimitiveTopology());

		UINT stride = sizeof(VertexStruct::SkyBoxVertex);
		UINT offset = 0;

		D3DDeviceContext->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		D3DDeviceContext->IASetIndexBuffer(mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		// ��� ���� ����
			// view�� �޾ƿ���..
		cbMatrix cbMat;
		cbMat.worldViewProj = world * view * proj;

		BufferManager::GetInstance()->GetBuffer("WorldViewProjCB")->SetUpBuffer(0, &cbMat, ShaderType::VERTEX);

		// skyBox �־��ֱ�..
		D3DDeviceContext->PSSetShaderResources(0, 1, cube->GetMapSRV().GetAddressOf());

		ID3D11SamplerState* sampler = SamplerManager::GetInstance()->GetTriLinearSamplerState();

		D3DDeviceContext->PSSetSamplers(0, 1, &sampler);

		D3DDeviceContext->RSSetState(mesh->GetRasterState().Get());

		D3DDeviceContext->DrawIndexed(mesh->GetIdxBufferSize(), 0, 0);
	}

	void SkyBox::SetObjectResources(std::shared_ptr<ObjectResources> objResources)
	{
		objectResources = objResources;
	}

	std::string SkyBox::GetObjName()
	{
		return objectResources->ObjName;
	}

	std::shared_ptr<ObjectResources> SkyBox::GetObjectResources()
	{
		return objectResources;
	}
}
