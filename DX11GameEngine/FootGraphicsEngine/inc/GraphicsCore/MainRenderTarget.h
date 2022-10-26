#pragma once

namespace GraphicsEngineSpace
{
	/**
	 * \brief ȭ�鿡 �׸��� �׸��� ���� RenderTargetView, DepthStencilView ���� ��Ƶ� Ŭ����.
	 *
	 * ���Ŀ� ��Ƽ ���� Ÿ�� ���� �ϸ鼭 ����� �߰� �ǰ�����,
	 * ����(2022.07.01)�� RenderTargetView�� DepthStencilView ���� �Ҿ������ �ʰ� �� �����ϴ� ���Ҹ� �Ѵ�.
	 *
	 * (2022.10.24 �߰�)
	 * �ش� Ŭ������ ȭ�鿡 �ֿ��ϰ� ǥ�� �Ǵ� ���� Ÿ���� �ǹ��ϸ�, �ٸ� ����Ÿ�ٰ� �ٸ��� �����ϰ�
	 * Depth StencilView�� ������.
	 *
	 * 2022.07.01 Dev.BlackFoot
	 */
	class MainRenderTarget
	{
	private:
		// ���� ���ٽ� ����
		ID3D11Texture2D* depthStencilBuffer;

		// ���� ��
		ID3D11RenderTargetView* renderTargetView;
		ID3D11DepthStencilView* depthStencilView;

		// ���� ���ٽ� ������Ʈ
		ID3D11DepthStencilState* depthStencilState;

	public:
		MainRenderTarget();
		~MainRenderTarget();

		void Finalize();

		// ���� ����..
			// �ܺο��� ���� �����ؾ� �ϱ� ������.. const�� ������ �ʴ´�..
			// �̰� �ȵǸ� ���ľ� ��

			// ���� �ȵǸ�.. �����͸� ���۷����� ��������..
			// �ƴϸ� ����̽��� �޾Ƽ� ���⼭ �����ϴ� ������ �غ���..(�̰� �� ���� ���� �ִ�.)
			// r-value�� ��� �ȴ�..
		ID3D11RenderTargetView* GetRenderTargetView() { return renderTargetView; }
		ID3D11DepthStencilView* GetDepthStencilView() { return depthStencilView; }
		ID3D11Texture2D* GetDepthStencilBuffer() { return depthStencilBuffer; }
		ID3D11DepthStencilState* GetDepthStencilState() { return depthStencilState; }

		// ����̽��� �޾Ƽ� ���� Ÿ���� ���� ����� �ִ� ���� (����� �°�..)
			// ����� �޾ƿ�����.
		void CreateRenderTarget(
			ID3D11Device* _device, ID3D11DeviceContext* _immediateDC, IDXGISwapChain* _swapChain,
			int _clientWidth, int _clientHeight, UINT _4xMSAAQuality);

		// OnResize ó���� ���� Ÿ�ٵ��� �����ִ� �Լ�.
		void DeleteImmediateRenderTarget();
	};
}
