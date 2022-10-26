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
		// ��� ���� ������
		ReleaseCOM(renderTargetView)
		ReleaseCOM(depthStencilView)
		ReleaseCOM(depthStencilBuffer)
		ReleaseCOM(depthStencilState)
	}

	// �並 �����ô��� r-value�� ��� �Ǿ��� ������ ��ġ���� �޾Ƽ� �������ִ� ������ �غ���..
		// ��Ȯ�ϰԴ� CreateRenderTarget �κ�.
	void MainRenderTarget::CreateRenderTarget(ID3D11Device* _device, ID3D11DeviceContext* _immediateDC, IDXGISwapChain* _swapChain, int _clientWidth, int _clientHeight, UINT _4xMSAAQuality)
	{
		// ���� �ִ� �Ű� ������ ����� �����Ǿ����� Ȯ��.
		assert(_device);
		assert(_immediateDC);
		assert(_swapChain);

		// ���� Ÿ�� �� ����
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

		// ���� ���ٽ� ����, �� ����
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

		// State ����
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

	// ������ ���� Ÿ���� �����ִ� �Լ�.
	void MainRenderTarget::DeleteImmediateRenderTarget()
	{
		// ��� ��ɻ����δ� Finalize�� �ٸ� �ٰ� ����..
			// ���Ŀ� ���ο� ��� ������ �������� �� �޶��� ����..
			// ���⼭�� ȭ���� ����� �� �ٲ��ִ� ���� ���� Ÿ�ٵ鸸 �����ش�.
		ReleaseCOM(renderTargetView)
		ReleaseCOM(depthStencilView)
		ReleaseCOM(depthStencilBuffer)
	}

}
