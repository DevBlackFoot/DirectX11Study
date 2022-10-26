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
		// 예외 확인
		assert(pDevice);
		assert(pDeviceContext);

		D3DDevice = pDevice;
		D3DDeviceContext = pDeviceContext;

		return true;
	}

	void PosColorObj::Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj)
	{
		// 행렬 넣기
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

		// 가장 기본적인 인풋 레이아웃을 가져옵시다.
		// 내부 함수에 inputlayout 세팅이 있습니다.
		ShaderManager::GetInstance()->GetShader("PosColorVS")->SetUpShader();
		// pixel 셰이더
		ShaderManager::GetInstance()->GetShader("PosColorPS")->SetUpShader();
		// 버퍼 세팅을 할 이유가 없다 -> 들어가는 버퍼가 없기 때문.
		D3DDeviceContext->IASetPrimitiveTopology(objectResources->Topology);

		// 버텍스 버퍼, 인덱스 버퍼 세팅
		UINT stride = sizeof(VertexStruct::ColorVertex);
		UINT offset = 0;
		D3DDeviceContext->IASetVertexBuffers(0, 1, &objectResources->VB, &stride, &offset);
		D3DDeviceContext->IASetIndexBuffer(objectResources->IB, DXGI_FORMAT_R32_UINT, 0);
		
		// 상수 버퍼 세팅.
		// 데이터는 constBuffer와 닮은꼴의 구조체를 던져야함.
		cbMatrix cbPerObj;
		cbPerObj.worldViewProj = world * view * proj;
		BufferManager::GetInstance()->GetBuffer("MatrixCB")->SetUpBuffer(0, &cbPerObj, ShaderType::VERTEX);


		D3DDeviceContext->RSSetState(objectResources->RasterizerState);

		// 해당 내용 그리기.
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
