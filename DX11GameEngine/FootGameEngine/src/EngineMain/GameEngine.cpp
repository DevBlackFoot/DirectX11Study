#include "GamePch.h"
#include "EngineMain/GameEngine.h"

#include "InputManager/InputManager.h"
#include "Timer/Timer.h"
#include "WindowManager/WindowManager.h"
#include "GraphicsManager/GraphicsManager.h"
#include "SceneManager/SceneManager.h"

namespace GameEngineSpace
{
	std::shared_ptr<GameEngine> GameEngine::gameEngineInstance = nullptr;

	void GameEngine::Init(tstring gameTitle, HINSTANCE hInst, int screenWidth, int screenHeight)
	{

		// ��� �Ŵ����� Init �մϴ�.
		WindowManager::GetInstance()->InitWindow(gameTitle, hInst, screenWidth, screenHeight);
		Timer::GetInstance()->Init();
		InputManager::GetInstance();
		GraphicsManager::GetInstance()->Init(WindowManager::GetInstance()->GetHWND(), screenWidth, screenHeight);
		SceneManager::GetInstance()->Init();

		// ��� �Ŵ����� Init�� ������.. �����츦 ����ݴϴ�.
		WindowManager::GetInstance()->StartWindow();
	}

	INT GameEngine::Tick()
	{
		// �� ƽ ��������..
			// ������ �Ŵ��� ������ update�� �����ϴ�
		return WindowManager::GetInstance()->Update();
	}

	void GameEngine::Release()
	{
		Timer::GetInstance()->Release();
		SceneManager::GetInstance()->Release();
		GraphicsManager::GetInstance()->Release();
		WindowManager::GetInstance()->Release();
	}
}