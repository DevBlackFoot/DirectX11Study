#pragma once

#include "IRenderer.h"

#include <vector>

using namespace std;
using namespace DirectX::SimpleMath;

namespace GraphicsEngineSpace
{
	class DX11GraphicsCore;
	class MainRenderTarget;
	class RenderTargetTexture;

	/**
	 * \brief IRenderer를 상속받아 실제로 구현한 클래스.
	 *
	 * 게임 엔진과 붙어 있는 부분을 떨어뜨리려 노력 중..
	 *
	 * 2022.06.14(화) Dev.BlackFoot
	 */
	class Renderer : public IRenderer
	{
		// 변수 목록
		// 윈도우 핸들
		HWND hWnd;

		// Device, DC, RS 등이 들어가 있는 코어 클래스
		DX11GraphicsCore* graphicsCore;

		// View들을 가지고 있는 렌더 타겟 클래스
		MainRenderTarget* mainRenderTarget;
		// 테스트용 렌더 타겟
		RenderTargetTexture* depthRenderTarget;

		// Test용 렌더 타겟 화면 표시
		std::shared_ptr<IDXObject> depthRenderTargetObj;

		// TODO DXTK에 있는 폰트 넣기

		// Sprite
		SpriteBatch* spriteBatch;

		float deltaTime;	

		// 창 사이즈, 상태 등과 관련된 각종 변수.
			// 이게 렌더러에..?
		bool minimized;
		bool maximized;

		int clientWidth;
		int clientHeight;

	private:
		// 팩토리를 만들면
			// 화면에 그려줄 오브젝트 목록들.
			// 렌더 큐? 렌더 벡터?
		vector<std::shared_ptr<IDXObject>> renderVector;

	public:
		Renderer();
		virtual ~Renderer();

		virtual bool Initialize(HWND _hWnd, int _clientWidth, int _clientHeight) override;
		virtual void Finalize() override;
		virtual void OnResize() override;
		virtual void RenderAll() override;

		// 종횡비 획득
		virtual float GetAspectRatio() const override;

		// 창 사이즈 관련 변수
		virtual bool IsVaildDevice() override;
		virtual void SetClientSize(int _width, int _height) override;
		virtual void SetWinMinMax(bool _isMinimized, bool _isMaximized) override;

		virtual void AddRenderObj(std::shared_ptr<IDXObject> obj) override;
		virtual void InitObject() override;
		virtual void ClearRenderVector() override;

	private:
		void BeginRender();
		void Render();
		void DebugRender();
		void EndRender();

	};
}

// 렌더러 자체를 생성하는 함수를 따로 빼준다.
extern "C" GRAPHICSENGINE_DECLSPEC GraphicsEngineSpace::IRenderer* CreateRenderer();
extern "C" GRAPHICSENGINE_DECLSPEC void DeleteRenderer(GraphicsEngineSpace::IRenderer* obj);