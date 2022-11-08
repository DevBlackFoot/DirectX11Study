#include "GamePch.h"
#include "GraphicsManager/GraphicsManager.h"
#include "WindowManager/WindowManager.h"
#include "InputManager/InputManager.h"
#include "Manager/ResourceManager.h"
#include "SceneManager/SceneManager.h"

namespace GameEngineSpace
{
	// 스태틱변수 선언
	std::shared_ptr<GraphicsManager> GraphicsManager::instance = nullptr;

	bool GraphicsManager::Init(HWND hWnd, uint32 screenWidth, uint32 screenHeight)
	{
		// 여기서 Dll을 로드해줍니다.
			// TODO : 내 구조에 맞게 재 설계.
#ifdef x64
#ifdef _DEBUG
		graphicsDLL = LoadLibrary(_T("DLL/FootGraphicsEngine_Debug_x64.dll"));
#else
		graphicsDLL = LoadLibrary(_T("DLL/FootGraphicsEngine_Release_x64.dll"));
#endif
#else
#ifdef _DEBUG
		graphicsDLL = LoadLibrary(_T("DLL/FootGraphicsEngine_Debug_x86.dll"));
#else
		graphicsDLL = LoadLibrary(_T("DLL/FootGraphicsEngine_Release_x86.dll"));
#endif
#endif

		if (graphicsDLL == nullptr)
		{
			// TO DO
			// DLL 로드 실패
			OutputDebugString(_T("Load graphics engine dll failed.\n"));
			return false;
		}

		OutputDebugString(_T("Load graphics engine dll success.\n"));

		/*
		using GraphicsEngineSpace::GraphicsEngineConstructor;

		GraphicsEngineConstructor graphicsConstructor = (GraphicsEngineConstructor)GetProcAddress(graphicsDLL, "CreateGraphicsEngine");
		renderer.reset(graphicsConstructor());*/

		// 초기 리사이즈를 제대로 하기위한 클라이언트 정보 얻어오기.
		RECT rect;
		GetClientRect(hWnd, &rect);
		uint32 _width = rect.right - rect.left;
		uint32 _height = rect.bottom - rect.top;

		// 렌더러를 생성해봅시다.
		renderer.reset(CreateRenderer());

		if (renderer->Initialize(hWnd, _width, _height) != true)
			return false;

		/*if (renderer->CreateDirectXEngine(hWnd, screenWidth, screenHeight) != true)
		{
			OutputDebugString(_T("Create DirectX dll failed.\n"));
			return false;
		}*/

		/*if (renderer->CreateDirectXEngine(hWnd, _width, _height) != true)
		{
			OutputDebugString(_T("Create DirectX dll failed.\n"));
			return false;
		}*/

		// 리사이즈 펑션을 정합니다..
		// 윈도우 매니저 불러오기.
		WindowManager::GetInstance()->SetOnResizeFunc([&](uint32 width, uint32 height)
			{
				renderer->SetClientSize(width, height);
				renderer->OnResize();
			}
		);

		WindowManager::GetInstance()->SetOnMouseMoveFunc([&](float x, float y)
			{
				InputManager::GetInstance()->SetMousePos(x, y);
			}
		);

		dLight = new DirectionalLight();

		// 초기화
		dLight->Ambient = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
		dLight->Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		dLight->Specular = 1.0f;
		dLight->Direction = XMFLOAT3(0.0f, -0.5f, 0.57735f);

		return true;
	}

	void GraphicsManager::Update()
	{
		/// 화면 갱신에 있어서 참조할 사항을 날려줍니다..
		///
	}

	void GraphicsManager::Release()
	{
		if (renderer != nullptr)
			renderer->Finalize();
		renderer.reset();

		if (graphicsDLL != nullptr)
			FreeLibrary(graphicsDLL);
	}

	void GraphicsManager::CreateUITest(HWND hWnd)
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		uint32 _width = rect.right - rect.left;
		uint32 _height = rect.bottom - rect.top;

		// 테스트용 스프라이트
				// 해당 부분을 렌더러에 만들어 준다.
			//testCanvas = std::make_shared<Canvas>(_width, _height);
		testCanvas = renderer->CreateCanvas("TestCanvas", _width, _height);

		auto testSprite = testCanvas->CreateSpriteUI("TestSprite");

		auto resourceManager = ResourceManager::GetInstance();

		uint64 id = resourceManager->LoadTexture("Smol", L"Resources/UI/Smol.png");

		testSprite->SetTexture(resourceManager->GetTexture(id));
		testSprite->SetPosition({200.f, 100.f, 0.1f});
		testSprite->SetWidth(200.f);
		testSprite->SetHeight(200.f);

	}

	void GraphicsManager::UIRender(float tick)
	{
		testCanvas->Render(tick);
	}

	std::shared_ptr<GraphicsManager> GraphicsManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<GraphicsManager>();

		return instance;
	}
}