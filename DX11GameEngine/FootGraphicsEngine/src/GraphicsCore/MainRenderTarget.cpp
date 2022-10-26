#include "GraphicsPch.h"
#include "GraphicsCore//MainRenderTarget.h"

namespace GraphicsEngineSpace
{
	MainRenderTarget::MainRenderTarget()
		: depthStencilBuffer(nullptr)
		, renderTargetView(nullptr)
		, depthStencilView(nullptr)
		, depthStencilState(nullptr)
	{
	}

	MainRenderTarget::~MainRenderTarget()
	{
	}

	void MainRenderTarget::Finalize()
	{
		// 멤버 변수 릴리즈
		ReleaseCOM(renderTargetView)
		ReleaseCOM(depthStencilView)
		ReleaseCOM(depthStencilBuffer)
		ReleaseCOM(depthStencilState)
	}

	// 뷰를 보내봤더니 r-value로 취급 되었기 때문에 장치들을 받아서 설정해주는 것으로 해보자..
		// 정확하게는 CreateRenderTarget 부분.
	void MainRenderTarget::CreateRenderTarget(ID3D11Device* _device, ID3D11DeviceContext* _immediateDC, IDXGISwapChain* _swapChain, int _clientWidth, int _clientHeight, UINT _4xMSAAQuality)
	{
		// 여기 있는 매개 변수가 제대로 생성되었는지 확인.
		assert(_device);
		assert(_immediateDC);
		assert(_swapChain);

		// 렌더 타겟 뷰 생성
		HR(_swapChain->ResizeBuffers(1, _clientWidth, _clientHeight, DXGI_FORMAT_B8G8R8A8_UNORM, 0));
		ID3D11Texture2D* _backBuffer;
		_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(&_backBuffer));

		if (_backBuffer != 0)
		{
			_device->CreateRenderTargetView(_backBuffer, 0, &renderTargetView);
		}
		else
		{
			return;
		}
		ReleaseCOM(_backBuffer);

		// 깊이 스텐실 버퍼, 뷰 생성
		D3D11_TEXTURE2D_DESC _depthStencilDesc;
		_depthStencilDesc.Width = _clientWidth;
		_depthStencilDesc.Height = _clientHeight;
		_depthStencilDesc.MipLevels = 1;
		_depthStencilDesc.ArraySize = 1;
		_depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		_depthStencilDesc.SampleDesc.Count = 1;
		_depthStencilDesc.SampleDesc.Quality = 0;

		_depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		_depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		_depthStencilDesc.CPUAccessFlags = 0;
		_depthStencilDesc.MiscFlags = 0;

		HR(_device->CreateTexture2D(&_depthStencilDesc, 0, &depthStencilBuffer));

		HR(_device->CreateDepthStencilView(depthStencilBuffer, 0, &depthStencilView));

		// State 생성
		D3D11_DEPTH_STENCIL_DESC descDSS = {};
		ZeroMemory(&descDSS, sizeof(D3D11_DEPTH_STENCIL_DESC));

		descDSS.DepthEnable = true;
		descDSS.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDSS.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		descDSS.StencilEnable = false;
		descDSS.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		descDSS.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		descDSS.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSS.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		descDSS.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSS.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HR(_device->CreateDepthStencilState(&descDSS, &depthStencilState));
	}

	// 기존의 렌더 타겟을 지워주는 함수.
	void MainRenderTarget::DeleteImmediateRenderTarget()
	{
		// 사실 기능상으로는 Finalize와 다를 바가 없다..
			// 이후에 내부에 멤버 변수가 많아지면 또 달라질 수도..
			// 여기서는 화면이 변경될 때 바꿔주는 각종 렌더 타겟들만 지워준다.
		ReleaseCOM(renderTargetView)
		ReleaseCOM(depthStencilView)
		ReleaseCOM(depthStencilBuffer)
	}

}
