#pragma once

namespace GraphicsEngineSpace
{
	/**
	* \brief Device, DeviceContext, SwapChain, RasterizerState ���� �����ϴ� �ھ� Ŭ����
	*
	* ���������� ���� �ʱ�ȭ�� �����Ѵ�, Getter�� ������ Device ���� ������ ��ȯ�� �� �ִ�.
	* Graphics ���ο����� ����� ���̱� ������ ���� Interface�� ���� �ʴ´�.
	*
	* �� �� �پ��� ������ ����ϱ� ���� �̱��� Ŭ������ ����� �־���.
	*
	* 2022.10.04(ȭ) Dev.BlackFoot
	*/
	class DX11GraphicsCore
	{
	private:
		// �̱��濡 ����ϱ� ���� �ν��Ͻ�
		static std::shared_ptr<DX11GraphicsCore> instance;

		// ������ �ִ� ���� Device���� ������..
		ID3D11Device* m_D3DDevice;					// Device
		ID3D11DeviceContext* m_D3DImmediateContext;	// DC
		IDXGISwapChain* m_SwapChain;				// SwapChain

		// ��Ƽ �ٸ������ ���� ����
		UINT m_4xMSAAQuality;
		bool m_Enable4xMSAA;


	public:
		DX11GraphicsCore();
		~DX11GraphicsCore();

		static std::shared_ptr<DX11GraphicsCore> GetInstance();

		// �Ʒ��� ��� ������ �ʱ�ȭ ���ִ� ������ �ʿ��ϴ�.
		// ���� �������� ������ �ִ� ���� �ʱ�ȭ �Լ��� ����� �ű��.
		// ���������� �Լ��� ȣ�� �Ǹ� ����� �Ѱܼ� ó���ϴ� �������.
		bool Initialize(HWND _hWnd, int _clientWidth, int _clientHeight);
		void Finalize();

		// ������ �ش�Ǵ� ���� �Լ��� ����
		void ResetView(ID3D11RenderTargetView* _nowRT, ID3D11DepthStencilView* _nowDSV, const FLOAT color[4]);
		void ResetRS();
		void PresentSwapChain();

		// ������? ���͸� ����� ����.
			// ���ʹ� ���� �ʿ䰡 ����..
			// const�� �ϴ°� �����Ϸ���..
		ID3D11Device* GetDevice() const { return m_D3DDevice; }
		ID3D11DeviceContext* GetImmediateDC() const { return m_D3DImmediateContext; }
		IDXGISwapChain* GetSwapChain() const { return m_SwapChain; }

		UINT GetMSAAQuality() const { return m_4xMSAAQuality; }

	};

}
