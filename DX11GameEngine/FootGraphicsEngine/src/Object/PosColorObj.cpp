#include "GraphicsPch.h"
#include "Object/PosColorObj.h"
#include "Resources/InputLayout.h"
#include "Manager/ShaderManager.h"
#include "Object/Shader/ShaderBase.h"
#include "Manager/BufferManager.h"
#include "Object/Buffer/BufferBase.h"

namespace GraphicsEngineSpace
{
	PosColorObj::PosColorObj()
		: D3DDevice(nullptr)
		, D3DDeviceContext(nullptr)
		, objectResources(nullptr)
		, world()
		, view()
		, proj()
	{
	}

	PosColorObj::~PosColorObj()
	{
		SafeReset(objectResources)
	}

	bool PosColorObj::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	{
		// ���� Ȯ��
		assert(pDevice);
		assert(pDeviceContext);

		D3DDevice = pDevice;
		D3DDeviceContext = pDeviceContext;

		return true;
	}

	void PosColorObj::Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj)
	{
		// ��� �ֱ�
		world = _world;
		view = _view;
		proj = _proj;
	}

	void PosColorObj::LightUpdate(const DirectionalLight& _dirLit, const PointLight& _pointLit, const SpotLight& _spotLit,
	                              const XMFLOAT3& _eyePosW)
	{

	}

	void PosColorObj::Render()
	{
		assert(D3DDeviceContext);

		// ���� �⺻���� ��ǲ ���̾ƿ��� �����ɽô�.
		// ���� �Լ��� inputlayout ������ �ֽ��ϴ�.
		ShaderManager::GetInstance()->GetShader("PosColorVS")->SetUpShader();
		// pixel ���̴�
		ShaderManager::GetInstance()->GetShader("PosColorPS")->SetUpShader();
		// ���� ������ �� ������ ���� -> ���� ���۰� ���� ����.
		D3DDeviceContext->IASetPrimitiveTopology(objectResources->Topology);

		// ���ؽ� ����, �ε��� ���� ����
		UINT stride = sizeof(VertexStruct::ColorVertex);
		UINT offset = 0;
		D3DDeviceContext->IASetVertexBuffers(0, 1, &objectResources->VB, &stride, &offset);
		D3DDeviceContext->IASetIndexBuffer(objectResources->IB, DXGI_FORMAT_R32_UINT, 0);
		
		// ��� ���� ����.
		// �����ʹ� constBuffer�� �������� ����ü�� ��������.
		cbMatrix cbPerObj;
		cbPerObj.worldViewProj = world * view * proj;
		BufferManager::GetInstance()->GetBuffer("MatrixCB")->SetUpBuffer(0, &cbPerObj, ShaderType::VERTEX);


		D3DDeviceContext->RSSetState(objectResources->RasterizerState);

		// �ش� ���� �׸���.
		D3DDeviceContext->DrawIndexed(objectResources->IndexCount, objectResources->IndexOffset, objectResources->VertexOffset);
	}

	void PosColorObj::SetObjectResources(std::shared_ptr<ObjectResources> objResources)
	{
		objectResources = objResources;
	}

	std::string PosColorObj::GetObjName()
	{
		return objectResources->ObjName;
	}

}
