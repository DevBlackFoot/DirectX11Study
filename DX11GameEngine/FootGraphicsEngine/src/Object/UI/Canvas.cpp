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
		int ID = 1;

		while(textUIMap.find(newUIName) != textUIMap.end())
		{
			newUIName = name + std::to_string(ID++);
		}

		std::shared_ptr<TextUI> newTextUI;
		newTextUI.reset(new TextUI());

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
			SafeReset(text.second);
		}

		textUIMap.clear();
	}
}
