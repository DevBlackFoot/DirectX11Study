#pragma once

namespace GraphicsEngineSpace
{
	/**
	* \brief Device, DeviceContext, SwapChain, RasterizerState 들을 관리하는 코어 클래스
	*
	* 내부적으로 각종 초기화를 진행한다, Getter로 내부의 Device 등을 밖으로 반환할 수 있다.
	* Graphics 내부에서만 사용할 것이기 때문에 따로 Interface를 뽑지 않는다.
	*
	* 좀 더 다양한 곳에서 사용하기 위해 싱글톤 클래스로 만들어 주었다.
	*
	* 2022.10.04(화) Dev.BlackFoot
	*/
	class DX11GraphicsCore
	{
	private:
		// 싱글톤에 사용하기 위한 인스턴스
		static std::shared_ptr<DX11GraphicsCore> instance;

		// 숨겨져 있는 각종 Device관련 변수들..
		ID3D11Device* m_D3DDevice;					// Device
		ID3D11DeviceContext* m_D3DImmediateContext;	// DC
		IDXGISwapChain* m_SwapChain;				// SwapChain

		// 안티 앨리어싱을 위한 변수
		UINT m_4xMSAAQuality;
		bool m_Enable4xMSAA;


	public:
		DX11GraphicsCore();
		~DX11GraphicsCore();

		static std::shared_ptr<DX11GraphicsCore> GetInstance();

		// 아래의 멤버 변수를 초기화 해주는 과정이 필요하다.
		// 현재 렌더러가 가지고 있는 각종 초기화 함수를 여기로 옮긴다.
		// 렌더러에서 함수가 호출 되면 여기로 넘겨서 처리하는 방식으로.
		bool Initialize(HWND _hWnd, int _clientWidth, int _clientHeight);
		void Finalize();

		// 렌더에 해당되는 각종 함수들 래핑
		void ResetView(ID3D11RenderTargetView* _nowRT, ID3D11DepthStencilView* _nowDSV, const FLOAT color[4]);
		void ResetRS();
		void PresentSwapChain();

		// 간단한? 게터를 만들어 주자.
			// 세터는 딱히 필요가 없다..
			// const로 하는게 안전하려나..
		ID3D11Device* GetDevice() const { return m_D3DDevice; }
		ID3D11DeviceContext* GetImmediateDC() const { return m_D3DImmediateContext; }
		IDXGISwapChain* GetSwapChain() const { return m_SwapChain; }

		UINT GetMSAAQuality() const { return m_4xMSAAQuality; }

	};

}
