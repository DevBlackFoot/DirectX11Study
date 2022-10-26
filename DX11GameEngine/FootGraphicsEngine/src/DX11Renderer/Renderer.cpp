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
	// ���� ������Ʈ..

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
		// �ϴ� ��� ���� ����
		hWnd = _hWnd;
		clientWidth = _clientWidth;
		clientHeight = _clientHeight;

		graphicsCore = new DX11GraphicsCore;
		// ���� ����̽� �� �ʱ�ȭ.
		graphicsCore->Initialize(hWnd, clientWidth, clientHeight);

		mainRenderTarget = new MainRenderTarget;
		depthRenderTarget = new RenderTargetTexture;

		ID3D11Device* device = graphicsCore->GetDevice();
		ID3D11DeviceContext* deviceContext = graphicsCore->GetImmediateDC();

		// ���� Effect �� Static Ŭ������ ��� �ʱ�ȭ.
		RasterizerState::InitAllRS(device);
		ShaderManager::GetInstance()->Init(device, deviceContext);
		ShaderManager::GetInstance()->CompileAllShader();
		BufferManager::GetInstance()->Init(device, deviceContext);
		SamplerManager::GetInstance()->Init(device);

		OnResize();

		// ���丮 �ν��Ͻ� ���� => dll �ܺο��� �����ϱ� ���� ���ο��� �������ش�.
		Factory::GetInstance();

		// ���� �Ŵ��� ���� �� Init => ����̽��� �ޱ� ������ ���������� Init�� ���־���Ѵ�.
		BuilderManger::GetInstance()->InitBuilderAll(graphicsCore->GetDevice(), graphicsCore->GetImmediateDC());

		// Texture �ϳ� ������.
		depthRenderTargetObj = Factory::GetInstance()->CreateDXObject<RenderTargetObj>(
		BuilderManger::GetInstance()->GetBuilder("TextureRectBuilder"), "DepthTex",
		depthRenderTarget->GetShaderResourceView());

		depthRenderTargetObj->Init(device, deviceContext);

		// ��������Ʈ ����
		spriteBatch = new DirectX::SpriteBatch(deviceContext);

		// ������� �ϸ� ����
		return true;
	}

	void Renderer::Finalize()
	{
		Factory::GetInstance()->DeleteFactory();

		BuilderManger::GetInstance()->DeleteBuildManager();

		InputLayout::DestoryAll();
		RasterizerState::DestroyAll();

		// ���� COM �����͸� Release �Ѵ�.
		mainRenderTarget->Finalize();

		graphicsCore->Finalize();
	}

	void Renderer::OnResize()
	{
		ID3D11Device* device = graphicsCore->GetDevice();
		ID3D11DeviceContext* deviceContext = graphicsCore->GetImmediateDC();

		// ������ ���� �����ְ�..
		mainRenderTarget->DeleteImmediateRenderTarget();
		// ���� ������ش�.
		mainRenderTarget->CreateRenderTarget(
			device, deviceContext, graphicsCore->GetSwapChain(),
			clientWidth, clientHeight, graphicsCore->GetMSAAQuality());

		depthRenderTarget->Finalize();
		depthRenderTarget->Init(device, clientWidth, clientHeight, graphicsCore->GetMSAAQuality());

		// ����Ʈ ����.
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
		// �� �������� ������Ʈ �غ���..
		depthRenderTargetObj->Update(XMMatrixIdentity(), XMMatrixIdentity(), XMMatrixIdentity());

		graphicsCore->ResetView(
			mainRenderTarget->GetRenderTargetView(),
			mainRenderTarget->GetDepthStencilView());

		graphicsCore->ResetView(
			depthRenderTarget->GetRenderTargetView(),
			mainRenderTarget->GetDepthStencilView()
		);

		// Pixel Shader���� ����� �������ִ� SV_target�� �� �ټ����� ������ �ش� �κп� �� �ټ����� ���� Ÿ���� �� �� �ִ�.
		ID3D11RenderTargetView* renderTargets[] = { mainRenderTarget->GetRenderTargetView(), depthRenderTarget->GetRenderTargetView() };
		graphicsCore->GetImmediateDC()->OMSetRenderTargets(2, renderTargets, mainRenderTarget->GetDepthStencilView());
	}

	void Renderer::Render()
	{

		// ���� ����.
		for (auto obj : renderVector)
		{
			obj->Render();
		}
		

		graphicsCore->ResetRS();
	}

	void Renderer::DebugRender()
	{
		ID3D11RenderTargetView* backbufferRTV = mainRenderTarget->GetRenderTargetView();
		// �� �׸��� ����ۿ� �׷�����.
		graphicsCore->GetImmediateDC()->OMSetRenderTargets(1, &backbufferRTV, mainRenderTarget->GetDepthStencilView());

		// ���⼭ Ư�� ���� Ÿ���� �� �� ������..
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

// ������ ���� �Լ�
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
