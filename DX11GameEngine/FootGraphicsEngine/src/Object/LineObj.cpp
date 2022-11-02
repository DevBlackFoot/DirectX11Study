#include "GraphicsPch.h"
#include "Object/LineObj.h"

#include "Manager/ShaderManager.h"
#include "Manager/BufferManager.h"
#include "Manager/ResourceManager.h"

#include "Object/Shader/ShaderBase.h"
#include "Object/Buffer/BufferBase.h"

namespace GraphicsEngineSpace
{
	LineObj::LineObj()
		: D3DDevice(nullptr)
		, D3DDeviceContext(nullptr)
		, objectResources(nullptr)
		, resourceManager(nullptr)
		, world()
		, view()
		, proj()
	{
	}

	LineObj::~LineObj()
	{
		SafeReset(objectResources)
	}

	bool LineObj::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	{
		// ���� Ȯ��
		assert(pDevice);
		assert(pDeviceContext);

		D3DDevice = pDevice;
		D3DDeviceContext = pDeviceContext;

		resourceManager = ResourceManager::GetInstance();

		return true;
	}

	void LineObj::Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj)
	{
		// ��� �ֱ�
		world = _world;
		view = _view;
		proj = _proj;
	}

	void LineObj::LightUpdate(const DirectionalLight& _dirLit, const PointLight& _pointLit, const SpotLight& _spotLit,
	                              const XMFLOAT3& _eyePosW)
	{

	}

	void LineObj::Render()
	{
		assert(D3DDeviceContext);
		assert(resourceManager->GetMesh(objectResources->meshID));
		
		std::shared_ptr<Mesh> mesh = resourceManager->GetMesh(objectResources->meshID);

		// ���� �⺻���� ��ǲ ���̾ƿ��� �����ɽô�.
		// ���� �Լ��� inputlayout ������ �ֽ��ϴ�.
		ShaderManager::GetInstance()->GetShader("LineVS")->SetUpShader();
		// pixel ���̴�
		ShaderManager::GetInstance()->GetShader("LinePS")->SetUpShader();
		// ���� ������ �� ������ ���� -> ���� ���۰� ���� ����.
		D3DDeviceContext->IASetPrimitiveTopology(mesh->GetPrimitiveTopology());

		// ���ؽ� ����, �ε��� ���� ����
		UINT stride = sizeof(VertexStruct::ColorVertex);
		UINT offset = 0;
		D3DDeviceContext->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		D3DDeviceContext->IASetIndexBuffer(mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
		
		// ��� ���� ����.
		// �����ʹ� constBuffer�� �������� ����ü�� ��������.
		cbMatrix cbPerObj;
		cbPerObj.worldViewProj = world * view * proj;
		BufferManager::GetInstance()->GetBuffer("WorldViewProjCB")->SetUpBuffer(0, &cbPerObj, ShaderType::VERTEX);


		D3DDeviceContext->RSSetState(mesh->GetRasterState().Get());

		// �ش� ���� �׸���.
		D3DDeviceContext->DrawIndexed(mesh->GetIdxBufferSize(), 0, 0);
	}

	void LineObj::SetObjectResources(std::shared_ptr<ObjectResources> objResources)
	{
		objectResources = objResources;
	}

	std::string LineObj::GetObjName()
	{
		return objectResources->ObjName;
	}

	std::shared_ptr<ObjectResources> LineObj::GetObjectResources()
	{
		return objectResources;
	}
}
