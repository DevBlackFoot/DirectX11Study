#pragma once
#include "PCH/FootEngineTypedef.h"

// 그래픽스 엔진 사용을 위한 헤더
#include "../../FootGraphicsEngine/GraphicsEngineFramework.h"

namespace GameEngineSpace
{
	using namespace GraphicsEngineSpace;

	/**
	 * \brief 실제로 그래픽스 엔진을 초기화 하고, 가지고 있는 매니저 클래스
	 *
	 * 그래픽스 엔진의 사용이 필요할 때 해당 클래스를 매개로 이용한다. => 실질적으로 화면에 그림을 그리는 친구..
	 *
	 * 2022.08.02 Dev.BlackFoot
	 */
	class GraphicsManager final
	{
	private:
		static std::shared_ptr<GraphicsManager> instance;

		// 렌더러
		std::shared_ptr<IRenderer> renderer;
		HMODULE graphicsDLL;

		// 빛 정보들을 담은 큐..
			// 지만 지금은 DirLit 하나만
		DirectionalLight* dLight;

	public:
		GraphicsManager() = default;
		~GraphicsManager() = default;
		GraphicsManager(const GraphicsManager& other) = delete;
		GraphicsManager& operator= (const GraphicsManager& other) = delete;
		GraphicsManager(const GraphicsManager&& other) = delete;
		GraphicsManager& operator= (const GraphicsManager&& other) = delete;

	private:
		// 렌더 큐에 있는 모든 데이터를 그립니다.
		//void RenderPrefab();

	public:
		bool Init(HWND hWnd, uint32 screenWidth, uint32 screenHeight);
		void Update();
		void Release();

		// 그래픽스 매니저의 목적을.. 렌더러를 던져주는걸로 해보자..
		std::shared_ptr<IRenderer> GetRenderer() { return renderer; }

		// 빛도 던져줍시다..
		DirectionalLight* GetDirectionalLight() { return dLight; }

	public:
		static std::shared_ptr<GraphicsManager> GetInstance();

	};
}
