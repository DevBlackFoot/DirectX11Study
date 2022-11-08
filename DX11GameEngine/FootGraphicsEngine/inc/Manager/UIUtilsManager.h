#pragma once
#include <map>
#include <string>


namespace GraphicsEngineSpace
{
	/**
	 * \brief Font, Sprite Batch, DepthState, BlendState �� UI�� �׸��� ���� ���� �������� ��Ƶΰ� �ִ�
	 *  �Ŵ��� Ŭ�����Դϴ�.
	 */
	class UIUtilsManager
	{
	private:
		static std::shared_ptr<UIUtilsManager> instance;

		// COM ������ �Ժη� ����Ʈ �����ͷ� ��������..
		ID3D11Device* device;
		ID3D11DepthStencilState* depthState;
		// Sprite���� ������� Blend State
		ID3D11BlendState* blendState;

		std::shared_ptr<SpriteBatch> spriteBatch;

		std::map<std::string, std::shared_ptr<SpriteFont>> fontMap;

	public:
		UIUtilsManager()
			: device(nullptr)
			, depthState(nullptr)
			, blendState(nullptr)
			, spriteBatch(nullptr)
		{}
		~UIUtilsManager();

		// �ν��Ͻ� ���
		static std::shared_ptr<UIUtilsManager> GetInstance();

		// ���� �ʱ�ȭ �� ��� ��Ʈ ����
		void Init(ID3D11Device* _device, std::shared_ptr<SpriteBatch> spriteBatch, ID3D11DepthStencilState* depthState, ID3D11BlendState* blendState);

		// ���ϴ� ��Ʈ ���
		std::shared_ptr<SpriteFont> GetFont(const std::string& fontName);
		std::shared_ptr<SpriteBatch> GetSpriteBatch() { return spriteBatch; }
		ID3D11DepthStencilState* GetDepthState() { return depthState; }
		ID3D11BlendState* GetBlendState() { return blendState; }

		void Finalize();

	private:
		void CreateSpriteFont(const std::string& name, const std::string& path);
	};
}