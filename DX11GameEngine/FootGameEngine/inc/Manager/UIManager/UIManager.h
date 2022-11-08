#pragma once

#include "../../FootGraphicsEngine/GraphicsEngineFramework.h"

namespace GameEngineSpace
{
	class GraphicsManager;
	
	using GraphicsEngineSpace::Canvas;
	using GraphicsEngineSpace::UIBase;
		
	/**
	 * \brief UI, �ؽ�Ʈ �� ȭ�鿡 2�������� �����ϴ� UI �鸸�� ������ �ϴ� UI Manager Ŭ����
	 *
	 * 2022.08.08(��) Deb.BlackFoot
	 */

	class UIManager final
	{
	private:
		// UI Manager�� �ν��Ͻ�
		static std::shared_ptr<UIManager> instance;

		// ���������� �׷��Ƚ� �Ŵ��� ĳ��
		std::shared_ptr<GraphicsManager> graphicsInstance;

		// ĵ����
			// ���ӿ� �ִ� ��� UI ĵ�������� init�� ���� �� ���� ������.
			// UI�� �����Ǵ� ��� �༮���� ó�� Init �� �� ����� enable�� false�� �������ְ�
			// Ư�� ��Ȳ�� �ʿ��� �� �ش� UI ���̵� �����ͼ� ������ش�.
		Canvas* mainCanvas;

		// ȭ�� ������ �ľ��ϰ�. �������� �÷�����..
		uint32 nowWidth;
		uint32 nowHeight;

	public:
		UIManager() = default;
		~UIManager() = default;
		UIManager(const UIManager& other) = delete;
		UIManager(const UIManager&& other) = delete;
		UIManager& operator= (const UIManager& other) = delete;
		UIManager& operator= (const UIManager&& other) = delete;

		static std::shared_ptr<UIManager> GetInstance();

		void Init();

		// WIC �̽��� ���ϱ� ���� Canvas ����
		void CreateCanvasAll();

		// ����
		void Render();

		// ��ư �Է��� ���� ������Ʈ�� LateUpdate
		void Update();
		void LateUpdate();

		void Release();

		Canvas* CreateCanvas(const std::string& UIIDstr);

		// Ư�� ĵ������ ��� ���� ��
		Canvas* GetCanvas(const std::string& canvasIDStr);

		// ĵ���� ��������
		void OnResize(uint32 width, uint32 height);

		void ClearUI();

	private:
	};

}
