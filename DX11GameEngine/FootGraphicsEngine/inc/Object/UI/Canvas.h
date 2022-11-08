#pragma once

// UI�� �ö� ĵ���� Ŭ����

#include <map>
#include "TextUI.h"
#include "SpriteUI.h"

namespace GraphicsEngineSpace
{
	class Canvas : public UIBase
	{
		// ���� UI �������� �����ص� map
		std::map<std::string, std::shared_ptr<TextUI>> textUIMap;
		std::map<std::string, std::shared_ptr<SpriteUI>> spriteUIMap;

	public:
		Canvas(float width, float height);
		~Canvas();

		// �ܺο��� ��� �����ϵ��� virtual.
		virtual std::shared_ptr<TextUI> CreateTextUI(const std::string& name);
		virtual std::shared_ptr<TextUI> GetTextUI(const std::string& name);

		virtual std::shared_ptr<SpriteUI> CreateSpriteUI(const std::string& name);
		virtual std::shared_ptr<SpriteUI> GetSpriteUI(const std::string& name);


		virtual void SetScaleAllCanvas(SimpleMath::Vector2 scale);

		void Render(float tick) override;
		virtual void Release();
	};

}
