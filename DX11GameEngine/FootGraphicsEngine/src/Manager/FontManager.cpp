#include "GraphicsPch.h"
#include "Manager/FontManager.h"

using namespace ATL;

namespace GraphicsEngineSpace
{

	std::shared_ptr<FontManager> FontManager::instance = nullptr;


	FontManager::~FontManager()
	{
		
	}

	std::shared_ptr<FontManager> FontManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<FontManager>();

		return instance;
	}

	void FontManager::Init(ID3D11Device* _device, SpriteBatch* spriteBatch, ID3D11DepthStencilState* depthState)
	{
		device = _device;
		this->spriteBatch.reset(spriteBatch);
		this->depthState = depthState;

		// 여기서 모든 폰트를 만들어줍니다.
		CreateSpriteFont("굴림", "Font/gulim9k.spritefont");
		CreateSpriteFont("EBS-Light", "Font/ebsLight.spritefont");
	}

	std::shared_ptr<SpriteFont> FontManager::GetFont(const std::string& fontName)
	{
		if(fontMap.find(fontName) == fontMap.end())
			return nullptr;

		return fontMap.at(fontName);
	}

	void FontManager::Finalize()
	{
		for(auto font : fontMap)
		{
			font.second.reset();
		}

		fontMap.clear();

		SafeReset(spriteBatch)

		SafeReset(instance);
	}

	void FontManager::CreateSpriteFont(const std::string& name, const std::string& path)
	{
		TCHAR buffer[256] = {};

		_tcscpy_s(buffer, CA2T(path.c_str()));

		std::shared_ptr<SpriteFont> dxFont = std::make_shared<SpriteFont>(device, buffer);
		dxFont->SetLineSpacing(32.0f);

		// 만들어진 폰트 맵에 넣어주기
		fontMap.insert(std::pair{name, dxFont});
	}
}
