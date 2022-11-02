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
		// 예외 확인
		assert(pDevice);
		assert(pDeviceContext);

		D3DDevice = pDevice;
		D3DDeviceContext = pDeviceContext;

		resourceManager = ResourceManager::GetInstance();

		return true;
	}

	void LineObj::Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj)
	{
		// 행렬 넣기
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

		// 가장 기본적인 인풋 레이아웃을 가져옵시다.
		// 내부 함수에 inputlayout 세팅이 있습니다.
		ShaderManager::GetInstance()->GetShader("LineVS")->SetUpShader();
		// pixel 셰이더
		ShaderManager::GetInstance()->GetShader("LinePS")->SetUpShader();
		// 버퍼 세팅을 할 이유가 없다 -> 들어가는 버퍼가 없기 때문.
		D3DDeviceContext->IASetPrimitiveTopology(mesh->GetPrimitiveTopology());

		// 버텍스 버퍼, 인덱스 버퍼 세팅
		UINT stride = sizeof(VertexStruct::ColorVertex);
		UINT offset = 0;
		D3DDeviceContext->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		D3DDeviceContext->IASetIndexBuffer(mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
		
		// 상수 버퍼 세팅.
		// 데이터는 constBuffer와 닮은꼴의 구조체를 던져야함.
		cbMatrix cbPerObj;
		cbPerObj.worldViewProj = world * view * proj;
		BufferManager::GetInstance()->GetBuffer("WorldViewProjCB")->SetUpBuffer(0, &cbPerObj, ShaderType::VERTEX);


		D3DDeviceContext->RSSetState(mesh->GetRasterState().Get());

		// 해당 내용 그리기.
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
