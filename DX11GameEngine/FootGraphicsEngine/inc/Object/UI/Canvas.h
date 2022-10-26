#pragma once

// UI가 올라갈 캔버스 클래스

#include <map>
#include "TextUI.h"

namespace GraphicsEngineSpace
{
	class Canvas : public UIBase
	{
		// 각종 UI 정보들을 저장해둘 map
		std::map<std::string, std::shared_ptr<TextUI>> textUIMap;

	public:
		Canvas(float width, float height);
		~Canvas();

		std::shared_ptr<TextUI> CreateTextUI(const std::string& name);

		std::shared_ptr<TextUI> GetTextUI(const std::string& name);

		void SetScaleAllCanvas(SimpleMath::Vector2 scale);

		void Render(float tick) override;
		void Release();
	};

}
