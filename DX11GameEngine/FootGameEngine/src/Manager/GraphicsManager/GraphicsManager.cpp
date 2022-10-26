#include "GamePch.h"
#include "GraphicsManager/GraphicsManager.h"
#include "WindowManager/WindowManager.h"
#include "InputManager/InputManager.h"
#include "SceneManager/SceneManager.h"

namespace GameEngineSpace
{
	// ����ƽ���� ����
	std::shared_ptr<GraphicsManager> GraphicsManager::instance = nullptr;

	bool GraphicsManager::Init(HWND hWnd, uint32 screenWidth, uint32 screenHeight)
	{
		// ���⼭ Dll�� �ε����ݴϴ�.
			// TODO : �� ������ �°� �� ����.
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
			// DLL �ε� ����
			OutputDebugString(_T("Load graphics engine dll failed.\n"));
			return false;
		}

		OutputDebugString(_T("Load graphics engine dll success.\n"));

		/*
		using GraphicsEngineSpace::GraphicsEngineConstructor;

		GraphicsEngineConstructor graphicsConstructor = (GraphicsEngineConstructor)GetProcAddress(graphicsDLL, "CreateGraphicsEngine");
		renderer.reset(graphicsConstructor());*/

		// �ʱ� ������� ����� �ϱ����� Ŭ���̾�Ʈ ���� ������.
		RECT rect;
		GetClientRect(hWnd, &rect);
		uint32 _width = rect.right - rect.left;
		uint32 _height = rect.bottom - rect.top;

		// �������� �����غ��ô�.
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

		// �������� ����� ���մϴ�..
		// ������ �Ŵ��� �ҷ�����.
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

		// �ʱ�ȭ
		dLight->Ambient = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
		dLight->Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		dLight->Specular = 1.0f;
		dLight->Direction = XMFLOAT3(0.0f, -0.5f, 0.57735f);

		return true;
	}

	void GraphicsManager::Update()
	{
		/// ȭ�� ���ſ� �־ ������ ������ �����ݴϴ�..
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

	std::shared_ptr<GraphicsManager> GraphicsManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<GraphicsManager>();

		return instance;
	}
}