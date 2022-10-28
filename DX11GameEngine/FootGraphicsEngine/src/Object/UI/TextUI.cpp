#include "GraphicsPch.h"

#include "Object/UI/TextUI.h"
#include "Manager/FontManager.h"

namespace GraphicsEngineSpace
{
	TextUI::TextUI()
		: text(L"Temp Text")
		, font{"굴림"}
		, color{1.0f, 1.0f, 1.0f, 1.0f}
		, fontSize(16.0f)
	{
	}

	TextUI::~TextUI()
	{

	}

	void TextUI::SetText(const std::string& text)
	{
		this->text.assign(text.begin(), text.end());

		width = text.size() * (fontSize - 1.0f);
		height = fontSize;
	}

	void TextUI::SetText(const std::wstring& text)
	{
		this->text = text;

		width = text.size() * (fontSize - 1.0f);
		height = fontSize;
	}

	void TextUI::SetColor(const SimpleMath::Vector4& color)
	{
		this->color = color;
	}

	void TextUI::SetFont(const std::string& fontName)
	{
		font = fontName;
	}

	void TextUI::SetFontSize(float fontSize)
	{
		if(fontSize < 0.0f)
			this->fontSize = 0.0f;
		else
			this->fontSize = fontSize;

		width = text.size() * (this->fontSize - 1.0f);
		height = this->fontSize;
	}

	std::string TextUI::GetText()
	{
		std::string temp;
		temp.assign(text.begin(), text.end());

		return temp;
	}

	void TextUI::Render(float tick)
	{
		if(isEnable != true)
			return;

		// 그림 그리기.
		std::shared_ptr<SpriteBatch> batch = FontManager::GetInstance()->GetSpriteBatch();
		ID3D11DepthStencilState* depthState = FontManager::GetInstance()->GetDepthState();

		batch->Begin(DirectX::SpriteSortMode_Deferred, nullptr, nullptr, depthState);
		FontManager::GetInstance()->GetFont(font)->DrawString(batch.get(), text.c_str(), color, GetScreenPosition(), 0.0f
		, SimpleMath::Vector2{ fontSize / 32.0f, fontSize / 32.0f } * GetScreenScale());
		batch->End();

		for(auto iter : children)
		{
			if(iter != nullptr)
				iter->Render(tick);
		}
	}
}
