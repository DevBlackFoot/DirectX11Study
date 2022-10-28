#pragma once
#include <map>
#include <string>


namespace GraphicsEngineSpace
{
	/**
	 * \brief 각종 SpriteFont들을 만들고 가지고 있는 역할을 하는 매니저 클래스입니다.
	 */
	class FontManager
	{
	private:
		static std::shared_ptr<FontManager> instance;

		// COM 변수는 함부로 스마트 포인터로 묶지말자..
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

		// 인스턴스 얻기
		static std::shared_ptr<FontManager> GetInstance();

		// 내부 초기화 및 모든 폰트 생성
		void Init(ID3D11Device* _device, SpriteBatch* spriteBatch, ID3D11DepthStencilState* depthState);

		// 원하는 폰트 얻기
		std::shared_ptr<SpriteFont> GetFont(const std::string& fontName);
		std::shared_ptr<SpriteBatch> GetSpriteBatch() { return spriteBatch; }
		ID3D11DepthStencilState* GetDepthState() { return depthState; }

		void Finalize();

	private:
		void CreateSpriteFont(const std::string& name, const std::string& path);
	};
}