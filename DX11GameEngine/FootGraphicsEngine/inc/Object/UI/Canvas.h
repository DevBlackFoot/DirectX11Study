#pragma once

// UI가 올라갈 캔버스 클래스

#include <map>
#include "TextUI.h"
#include "SpriteUI.h"

namespace GraphicsEngineSpace
{
	class Canvas : public UIBase
	{
		// 각종 UI 정보들을 저장해둘 map
		std::map<std::string, std::shared_ptr<TextUI>> textUIMap;
		std::map<std::string, std::shared_ptr<SpriteUI>> spriteUIMap;

	public:
		Canvas(float width, float height);
		~Canvas();

		// 외부에서 사용 가능하도록 virtual.
		virtual std::shared_ptr<TextUI> CreateTextUI(const std::string& name);
		virtual std::shared_ptr<TextUI> GetTextUI(const std::string& name);

		virtual std::shared_ptr<SpriteUI> CreateSpriteUI(const std::string& name);
		virtual std::shared_ptr<SpriteUI> GetSpriteUI(const std::string& name);


		virtual void SetScaleAllCanvas(SimpleMath::Vector2 scale);

		void Render(float tick) override;
		virtual void Release();
	};

}
