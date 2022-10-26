#include "GraphicsPch.h"
#include "DX11Renderer/Renderer.h"

#include <cassert>
#include <d3dcompiler.h>
#include "Object/IDXObject.h"
#include "GraphicsCore/DX11GraphicsCore.h"
#include "GraphicsCore/MainRenderTarget.h"
#include "GraphicsCore/RenderTargetTexture.h"

#include "GraphicsCore/RasterizerState.h"
#include "Resources/InputLayout.h"
#include "Builder/BuilderManager.h"
#include "Factory/Factory.h"
#include "Manager/ShaderManager.h"
#include "Manager/BufferManager.h"
#include "Manager/SamplerManager.h"

#include "Object/RenderTargetObj.h"

namespace GraphicsEngineSpace
{
	// 각종 오브젝트..

	Renderer::Renderer()
		: hWnd(0)
		, graphicsCore(nullptr)
		, mainRenderTarget(nullptr)
		, depthRenderTarget(nullptr)
		, spriteBatch(nullptr)
		, deltaTime(0.0f)
		, minimized(false)
		, maximized(false)
		, clientWidth(0)
		, clientHeight(0)
	{

	}

	Renderer::~Renderer()
	{
		if (graphicsCore)
		{
			delete graphicsCore;
		}
	}

	bool Renderer::Initialize(HWND _hWnd, int _clientWidth, int _clientHeight)
	{
		// 일단 멤버 변수 변경
		hWnd = _hWnd;
		clientWidth = _clientWidth;
		clientHeight = _clientHeight;

		graphicsCore = new DX11GraphicsCore;
		// 각종 디바이스 등 초기화.
		graphicsCore->Initialize(hWnd, clientWidth, clientHeight);

		mainRenderTarget = new MainRenderTarget;
		depthRenderTarget = new RenderTargetTexture;

		ID3D11Device* device = graphicsCore->GetDevice();
		ID3D11DeviceContext* deviceContext = graphicsCore->GetImmediateDC();

		// 각종 Effect 등 Static 클래스들 모두 초기화.
		RasterizerState::InitAllRS(device);
		ShaderManager::GetInstance()->Init(device, deviceContext);
		ShaderManager::GetInstance()->CompileAllShader();
		BufferManager::GetInstance()->Init(device, deviceContext);
		SamplerManager::GetInstance()->Init(device);

		OnResize();

		// 팩토리 인스턴스 생성 => dll 외부에서 생성하기 전에 내부에서 생성해준다.
		Factory::GetInstance();

		// 빌더 매니저 생성 및 Init => 디바이스를 받기 때문에 렌더러에서 Init을 해주어야한다.
		BuilderManger::GetInstance()->InitBuilderAll(graphicsCore->GetDevice(), graphicsCore->GetImmediateDC());

		// Texture 하나 만들어보기.
		depthRenderTargetObj = Factory::GetInstance()->CreateDXObject<RenderTargetObj>(
		BuilderManger::GetInstance()->GetBuilder("TextureRectBuilder"), "DepthTex",
		depthRenderTarget->GetShaderResourceView());

		depthRenderTargetObj->Init(device, deviceContext);

		// 스프라이트 생성
		spriteBatch = new DirectX::SpriteBatch(deviceContext);

		// 여기까지 하면 성공
		return true;
	}

	void Renderer::Finalize()
	{
		Factory::GetInstance()->DeleteFactory();

		BuilderManger::GetInstance()->DeleteBuildManager();

		InputLayout::DestoryAll();
		RasterizerState::DestroyAll();

		// 각종 COM 포인터를 Release 한다.
		mainRenderTarget->Finalize();

		graphicsCore->Finalize();
	}

	void Renderer::OnResize()
	{
		ID3D11Device* device = graphicsCore->GetDevice();
		ID3D11DeviceContext* deviceContext = graphicsCore->GetImmediateDC();

		// 기존의 것을 지워주고..
		mainRenderTarget->DeleteImmediateRenderTarget();
		// 새로 만들어준다.
		mainRenderTarget->CreateRenderTarget(
			device, deviceContext, graphicsCore->GetSwapChain(),
			clientWidth, clientHeight, graphicsCore->GetMSAAQuality());

		depthRenderTarget->Finalize();
		depthRenderTarget->Init(device, clientWidth, clientHeight, graphicsCore->GetMSAAQuality());

		// 뷰포트 설정.
		D3D11_VIEWPORT _vp;
		_vp.TopLeftX = 0.0f;
		_vp.TopLeftY = 0.0f;
		_vp.Width = static_cast<float>(clientWidth);
		_vp.Height = static_cast<float>(clientHeight);
		_vp.MinDepth = 0.0f;
		_vp.MaxDepth = 1.0f;

		deviceContext->RSSetViewports(1, &_vp);
	}

	void Renderer::RenderAll()
	{
		BeginRender();
		Render();
		DebugRender();
		EndRender();
	}

	float Renderer::GetAspectRatio() const
	{
		return static_cast<float>(clientWidth) / clientHeight;
	}

	bool Renderer::IsVaildDevice()
	{
		return (graphicsCore->GetDevice() != nullptr);
	}

	void Renderer::SetClientSize(int _width, int _height)
	{
		clientWidth = _width;
		clientHeight = _height;
	}

	void Renderer::SetWinMinMax(bool _isMinimized, bool _isMaximized)
	{
		minimized = _isMinimized;
		maximized = _isMaximized;
	}

	void Renderer::AddRenderObj(std::shared_ptr<IDXObject> obj)
	{
		renderVector.push_back(obj);
	}

	void Renderer::InitObject()
	{
		for (auto obj : renderVector)
		{
			obj->Init(graphicsCore->GetDevice(), graphicsCore->GetImmediateDC());
		}
	}

	void Renderer::ClearRenderVector()
	{
		for (auto renderObj : renderVector)
			renderObj.reset();

		renderVector.clear();
	}

	void Renderer::BeginRender()
	{
		// 이 지점에서 업데이트 해보자..
		depthRenderTargetObj->Update(XMMatrixIdentity(), XMMatrixIdentity(), XMMatrixIdentity());

		graphicsCore->ResetView(
			mainRenderTarget->GetRenderTargetView(),
			mainRenderTarget->GetDepthStencilView());

		graphicsCore->ResetView(
			depthRenderTarget->GetRenderTargetView(),
			mainRenderTarget->GetDepthStencilView()
		);

		// Pixel Shader에서 결과로 내보내주는 SV_target이 총 다섯개기 때문에 해당 부분에 총 다섯개의 렌더 타겟이 들어갈 수 있다.
		ID3D11RenderTargetView* renderTargets[] = { mainRenderTarget->GetRenderTargetView(), depthRenderTarget->GetRenderTargetView() };
		graphicsCore->GetImmediateDC()->OMSetRenderTargets(2, renderTargets, mainRenderTarget->GetDepthStencilView());
	}

	void Renderer::Render()
	{

		// 각종 렌더.
		for (auto obj : renderVector)
		{
			obj->Render();
		}
		

		graphicsCore->ResetRS();
	}

	void Renderer::DebugRender()
	{
		ID3D11RenderTargetView* backbufferRTV = mainRenderTarget->GetRenderTargetView();
		// 다 그리고 백버퍼에 그려주자.
		graphicsCore->GetImmediateDC()->OMSetRenderTargets(1, &backbufferRTV, mainRenderTarget->GetDepthStencilView());

		// 여기서 특정 렌더 타겟을 볼 수 있을까..
		//depthRenderTargetObj->Render();

		ID3D11ShaderResourceView* null[] = { nullptr };

		auto depthSRV = depthRenderTarget->GetShaderResourceView();

		spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, nullptr, nullptr, mainRenderTarget->GetDepthStencilState());
		spriteBatch->Draw(depthSRV, RECT{0, 0, static_cast<long>(clientWidth * 0.2f), static_cast<long>(clientHeight * 0.2f)});
		spriteBatch->End();

		graphicsCore->GetImmediateDC()->PSSetShaderResources(0, 1, null);

	}


	void Renderer::EndRender()
	{
		graphicsCore->PresentSwapChain();
	}

}

// 렌더러 생성 함수
GraphicsEngineSpace::IRenderer* CreateRenderer()
{
	return new GraphicsEngineSpace::Renderer;
}

void DeleteRenderer(GraphicsEngineSpace::IRenderer* obj)
{
	if (obj)
	{
		delete obj;
	}
}
