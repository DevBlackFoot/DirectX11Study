#pragma once

// UI�� �ö� ĵ���� Ŭ����

#include <map>
#include "TextUI.h"

namespace GraphicsEngineSpace
{
	class Canvas : public UIBase
	{
		// ���� UI �������� �����ص� map
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
