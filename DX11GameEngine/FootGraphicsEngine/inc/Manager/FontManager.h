#pragma once
#include <map>
#include <string>


namespace GraphicsEngineSpace
{
	/**
	 * \brief ���� SpriteFont���� ����� ������ �ִ� ������ �ϴ� �Ŵ��� Ŭ�����Դϴ�.
	 */
	class FontManager
	{
	private:
		static std::shared_ptr<FontManager> instance;

		// COM ������ �Ժη� ����Ʈ �����ͷ� ��������..
		ID3D11Device* device;
		ID3D11DepthStencilState* depthState;

		std::shared_ptr<SpriteBatch> spriteBatch;

		std::map<std::string, std::shared_ptr<SpriteFont>> fontMap;

	public:
		FontManager()
			: device(nullptr)
			, spriteBatch(nullptr)
			, depthState(nullptr)
		{}
		~FontManager();

		// �ν��Ͻ� ���
		static std::shared_ptr<FontManager> GetInstance();

		// ���� �ʱ�ȭ �� ��� ��Ʈ ����
		void Init(ID3D11Device* _device, SpriteBatch* spriteBatch, ID3D11DepthStencilState* depthState);

		// ���ϴ� ��Ʈ ���
		std::shared_ptr<SpriteFont> GetFont(const std::string& fontName);
		std::shared_ptr<SpriteBatch> GetSpriteBatch() { return spriteBatch; }
		ID3D11DepthStencilState* GetDepthState() { return depthState; }

		void Finalize();

	private:
		void CreateSpriteFont(const std::string& name, const std::string& path);
	};
}