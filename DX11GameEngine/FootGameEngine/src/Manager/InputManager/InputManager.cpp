#include "GamePch.h"
#include "InputManager/InputManager.h"

std::shared_ptr<GameEngineSpace::InputManager> GameEngineSpace::InputManager::instance = nullptr; 

namespace GameEngineSpace
{
	InputManager::InputManager()
		: prevCursorXY{}
		, prevKeyState{}
	{
		isMouseInWindow = false;
		isWindowActivated = true;
	}

	InputManager::~InputManager()
	{
	}

	// Ű �Է� Ȯ���Լ�.
	void InputManager::Update()
	{
		// ����ϴ� Ű ��� ��ü�� �����Ѵ�.
			// ù �����ӿ��� ����־ �������� �ʴ´�.
		for (auto& iter : currKeyState)
		{
			if ((GetAsyncKeyState(iter.first) & 0x8000) != 0)
				currKeyState[iter.first] = true;
			else
				currKeyState[iter.first] = false;
		}
	}

	void InputManager::LateUpdate()
	{
		// Ű ���� ����
		for (auto& iter : currKeyState)
			prevKeyState[iter.first] = iter.second;

		prevCursorXY = currCursorXY;
	}

	// ���� �����ӿ����� Ű ����.
	bool InputManager::GetInputState(int key, KeyState state)
	{
		if (isWindowActivated != true)
			return false;

		switch (state)
		{

			case KeyState::UP:
			{
				// Ű�� �����ٰ� ���� Ÿ�ֿ̹� True�� ��ȯ�Ѵ�.
				return currKeyState[key] ? false : (prevKeyState[key] ? true : false);
			}

			case KeyState::DOWN:
			{
				// Ű�� ������ Ÿ�ֿ̹� True�� ��ȯ�Ѵ�.
				return prevKeyState[key] ? false : (currKeyState[key] ? true : false);
			}

			case KeyState::STAY:
			{
				// Ű�� ������ ������ True�� ��ȯ�Ѵ�.
				return currKeyState[key];
			}

			case KeyState::TOGGLE:
			{
				// Ű�� ���� ������ On/Off �ȴ�.
				return GetKeyState(key) & 0x0001;
			}

		}

		return false;
	}

	// ���콺 ��ǥ ����.
	void InputManager::SetMousePos(float x, float y)
	{
		currCursorXY.x = x;
		currCursorXY.y = y;
	}

	// ���콺 ��ġ ������.
	const Vector2& InputManager::GetMousePos()
	{
		return currCursorXY;
	}

	// ���콺�� ������ ����
	Vector2 InputManager::GetMouseMoveVector()
	{
		return Vector2
		{
			currCursorXY.x - prevCursorXY.x,
			currCursorXY.y - prevCursorXY.y,
		};
	}

	std::shared_ptr<InputManager> InputManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<InputManager>();

		return instance;
	}
}