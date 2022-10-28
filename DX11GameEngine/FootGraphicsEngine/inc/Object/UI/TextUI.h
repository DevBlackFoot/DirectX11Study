#pragma once

// �ؽ�Ʈ UI�� ȭ�鿡 ���� ����..

#include <string>
#include "UIBase.h"

namespace GraphicsEngineSpace
{
	class Canvas;

	class TextUI : public UIBase
	{
		std::wstring text;
		std::string font;
		// ���İ����� �� color
		SimpleMath::Vector4 color;
		float fontSize;


	public:
		TextUI();
		~TextUI() override;

		void SetText(const std::string& text);
		void SetText(const std::wstring& text);
		void SetColor(const SimpleMath::Vector4& color);
		void SetFont(const std::string& fontName);
		void SetFontSize(float fontSize);

		std::string GetText();

	protected:
		void Render(float tick) override;

		friend Canvas;
	};
}
