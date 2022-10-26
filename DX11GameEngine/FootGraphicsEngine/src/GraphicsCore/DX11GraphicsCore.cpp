#include "GraphicsPch.h"
#include "GraphicsCore//DX11GraphicsCore.h"

#include "GraphicsCore/RasterizerState.h"

namespace GraphicsEngineSpace
{
	DX11GraphicsCore::DX11GraphicsCore()
		: m_D3DDevice(nullptr)
		, m_D3DImmediateContext(nullptr)
		, m_SwapChain(nullptr)
		, m_4xMSAAQuality(0)
		, m_Enable4xMSAA(false)
	{

	}

	DX11GraphicsCore::~DX11GraphicsCore()
	{
		if (m_D3DImmediateContext)
			m_D3DImmediateContext->ClearState();
	}

	bool DX11GraphicsCore::Initialize(HWND _hWnd, int _clientWidth, int _clientHeight)
	{
		// _hWnd는 가져온다..
		// Device와 DC를 만들어봅시다..
		UINT _createDeviceFlags = 0;

		// 디버그면..
#if defined(DEBUG) || defined(_DEBUG)
		_createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL _featureLevel;
		HRESULT hr = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0,
			_createDeviceFlags, 0, 0, D3D11_SDK_VERSION,
			&m_D3DDevice, &_featureLevel, &m_D3DImmediateContext);
		if (FAILED(hr))
		{
			return false;
		}

		// 버전 확인.
		// 버전 확인. 여기서 다른게 나오면 배열을 하나 생성해서 추가해야함.
		if (_featureLevel != D3D_FEATURE_LEVEL_11_0)
		{
			MessageBox(_hWnd, L"Direct3D Feature Level 11 unsupported.", 0, 0);
			return false;
		}

		// 4X MSAA 품질 수준 점검
		HR(m_D3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMSAAQuality));
		assert(m_4xMSAAQuality > 0);

		// SwapChain 생성.

		// DXGI_SWAP_CHAIN_DESC 구조체 생성
		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = _clientWidth;
		sd.BufferDesc.Height = _clientHeight;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		if (m_Enable4xMSAA)
		{
			sd.SampleDesc.Count = 4;

			sd.SampleDesc.Quality = m_4xMSAAQuality - 1;
		}
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = _hWnd;
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		IDXGIDevice* _dxgiDevice = 0;
		HR(m_D3DDevice->QueryInterface(__uuidof(IDXGIDevice),
			(void**)(&_dxgiDevice)));

		IDXGIAdapter* _dxgiAdapter = 0;
		HR(_dxgiDevice->GetParent(__uuidof(IDXGIAdapter),
			(void**)&_dxgiAdapter));

		IDXGIFactory* _dxgiFactory = 0;
		HR(_dxgiAdapter->GetParent(__uuidof(IDXGIFactory),
			(void**)&_dxgiFactory));

		HR(_dxgiFactory->CreateSwapChain(m_D3DDevice, &sd, &m_SwapChain));

		ReleaseCOM(_dxgiDevice);
		ReleaseCOM(_dxgiAdapter);
		ReleaseCOM(_dxgiFactory);

		// 이후 OnResize를 거친 뒤..
			// 여기서 Device, DC 등이 무슨 일을 하는 지 봐야함.
			// 일단은.. 새로 버퍼를 생성하는 과정에서 swapChain과 Device 등 여기 있는 게 다 사용되긴 하는데...
			// 문제는 여기에 각종 뷰를 어떻게 들고 올 수 있을 것인가.
			// 기본적으로 여기서 뷰와 버퍼들을 생성하게 된다..
				// 생각해보면 여기에 존재하는 View들은 마지막으로 화면에 그려주는 View들이기 때문에 그냥 멤버변수로 가지고 있어도..?
				// 응집성이 떨어질 것 같다.
			// 밖에서 인자로 받아서 onresize를 하는 방법도.. 있을거 같긴한데 의미가 없을 거 같고..

		// 렌더 스테이트 생성
		// 원래 렌더 스테이트를 생성하는 곳에서 생성해주자..
		RasterizerState::InitAllRS(m_D3DDevice);

		// 여기까지 하면 성공
		return true;
	}

	void DX11GraphicsCore::Finalize()
	{
		// 각종 친구들 해제.
		ReleaseCOM(m_SwapChain);

		if (m_D3DImmediateContext)
			m_D3DImmediateContext->ClearState();

		ReleaseCOM(m_D3DImmediateContext);
		ReleaseCOM(m_D3DDevice);
	}

	/**
	* \brief 렌더 시작전 지워주는 단계.
	* \param _nowRT 현재 렌더 타겟 뷰
	* \param _nowDSV 현재 뎁스 스텐실 뷰
	*/
	void DX11GraphicsCore::ResetView(ID3D11RenderTargetView* _nowRT, ID3D11DepthStencilView* _nowDSV)
	{
		assert(m_D3DImmediateContext);
		assert(m_SwapChain);

		// 렌더타겟과 스텐실 버퍼를 초기화한다.
		m_D3DImmediateContext->ClearRenderTargetView(_nowRT, reinterpret_cast<const float*>(&Colors::Gray));
		m_D3DImmediateContext->ClearDepthStencilView(_nowDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void DX11GraphicsCore::ResetRS()
	{
		m_D3DImmediateContext->RSSetState(0);
	}

	void DX11GraphicsCore::PresentSwapChain()
	{
		assert(m_SwapChain);

		HR(m_SwapChain->Present(1, 0));
	}

}
