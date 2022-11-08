#include "GraphicsPch.h"

#include "Object/UI/Canvas.h"
#include "Object/UI/TextUI.h"

namespace GraphicsEngineSpace
{
	Canvas::Canvas(float width, float height)
	{
		this->width = width;
		this->height = height;
	}

	Canvas::~Canvas()
	{
		Release();
	}

	std::shared_ptr<TextUI> Canvas::CreateTextUI(const std::string& name)
	{
		std::string newUIName = name;
		std::shared_ptr<TextUI> newTextUI = std::make_shared<TextUI>();

		newTextUI->SetName(name);
		newTextUI->SetParent(shared_from_this());

		textUIMap.insert(std::pair{name, newTextUI});

		return textUIMap.at(name);
	}

	std::shared_ptr<TextUI> Canvas::GetTextUI(const std::string& name)
	{
		auto textUI = textUIMap.find(name);
		if(textUI == textUIMap.end())
			return nullptr;

		return textUI->second;
	}

	std::shared_ptr<SpriteUI> Canvas::CreateSpriteUI(const std::string& name)
	{
		std::string newUIName = name;
		std::shared_ptr<SpriteUI> newSpriteUI = std::make_shared<SpriteUI>();

		newSpriteUI->SetName(name);
		newSpriteUI->SetParent(shared_from_this());

		spriteUIMap.insert(std::pair{name, newSpriteUI});

		return spriteUIMap.at(name);
	}

	std::shared_ptr<SpriteUI> Canvas::GetSpriteUI(const std::string& name)
	{
		auto spriteUI = spriteUIMap.find(name);
		if(spriteUI == spriteUIMap.end())
			return nullptr;

		return spriteUI->second;
	}

	void Canvas::SetScaleAllCanvas(SimpleMath::Vector2 scale)
	{
		// TODO 해당 부분은 캔버스 안에 캔버스가 생기면 넣는 것으로..
	}

	void Canvas::Render(float tick)
	{
		if(isEnable != true)
			return;

		for(auto iter : children)
		{
			if (iter != nullptr)
				iter->Render(tick);
		}

	}

	void Canvas::Release()
	{
		for(auto text : textUIMap)
		{
			SafeReset(text.second)
				
		}

		for(auto sprite : spriteUIMap)
		{
			SafeReset(sprite.second)
		}

		textUIMap.clear();
		spriteUIMap.clear();
	}
}
