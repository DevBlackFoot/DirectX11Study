#pragma once

namespace GraphicsEngineSpace
{
	/**
	 * \brief Deferred Rendering, ���� ����� ������ Ȯ���ϱ� ���� ���� Ÿ�� �ؽ��� Ŭ�����̴�.
	 *
	 * 2D Texture�� �ϳ� �����ؼ� ���� Ÿ��ȭ ��Ű��, �ش� �ؽ��ĸ� Shader Resource View�� �����ؼ�
	 * ȭ�鿡 �ؽ��ķμ� �׷��� �� �ֵ��� �ϴ� ������ ������.
	 *
	 * 2022.10.24 Dev.BlackFoot
	 */
	class RenderTargetTexture
	{
		// �ʿ��� ��� ����
	private:
		ID3D11Texture2D* renderTexture;
		ID3D11RenderTargetView* renderTarget;
		ID3D11ShaderResourceView* shaderResource;

	public:
		RenderTargetTexture();
		~RenderTargetTexture();

		// init. �ؽ����� ũ�⸦ �޾Ƽ� �������ش�.
		bool Init(ID3D11Device* device, int width, int height, UINT MSAAQuality);
		// COM ������ ���� (�������� �� �� �ҷ��ش�)
		void Finalize();

		// ���� Ÿ�� �並 ���� �ݴϴ�.
			// GraphicsCore�� ResetView�� �����ֱ� ���ؼ� �ʿ��մϴ�.
		ID3D11RenderTargetView* GetRenderTargetView() { return renderTarget; }

		// ���̴� ���ҽ� �並 �����ݴϴ�.
			// �̸� ���̴��� �������ν� �ٸ� ���̴� ���� �ش� �ؽ��� �����Ϳ� ���� ���������ϴ�.
		ID3D11ShaderResourceView* GetShaderResourceView() { return shaderResource; }
	};

}