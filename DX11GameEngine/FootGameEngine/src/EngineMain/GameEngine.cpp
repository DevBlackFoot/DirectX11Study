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

		// 모든 매니저를 Init 합니다.
		WindowManager::GetInstance()->InitWindow(gameTitle, hInst, screenWidth, screenHeight);
		Timer::GetInstance()->Init();
		InputManager::GetInstance();
		GraphicsManager::GetInstance()->Init(WindowManager::GetInstance()->GetHWND(), screenWidth, screenHeight);
		SceneManager::GetInstance()->Init();

		// 모든 매니저의 Init이 끝난뒤.. 윈도우를 띄워줍니다.
		WindowManager::GetInstance()->StartWindow();
	}

	INT GameEngine::Tick()
	{
		// 매 틱 돌때마다..
			// 윈도우 매니저 내부의 update를 돌립니다
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