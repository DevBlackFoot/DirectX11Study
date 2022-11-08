#pragma once

#include "IRenderer.h"

#include <vector>

using namespace std;
using namespace DirectX::SimpleMath;

namespace GraphicsEngineSpace
{
	class DX11GraphicsCore;
	class RenderTargetDepth;
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
		std::shared_ptr<DX11GraphicsCore> graphicsCore;

		// View들을 가지고 있는 렌더 타겟 클래스
		RenderTargetDepth* mainRenderTarget;

		// depth debug용 렌더 타겟.
		RenderTargetTexture* depthRenderTarget;
		RenderTargetTexture* normalRenderTarget;
		RenderTargetTexture* albedoRenderTarget;
		RenderTargetTexture* worldPosRenderTarget;

		// 스크린에서 사용하는 뷰포트의 DESC를 정해둔 screenViewport
			// CubeMap을 그리기 위한 Viewport를 따로 설정해줘야하기 때문에..
		D3D11_VIEWPORT screenViewport;

		// BlendState 세팅
		ID3D11BlendState* blendState;

		// Sprite
		std::shared_ptr<SpriteBatch> spriteBatch;

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

		virtual void EndRender() override;
		// 캔버스 생성
		virtual std::shared_ptr<Canvas> CreateCanvas(const std::string& name, float width, float height) override;

	private:
		void BeginRender();
		void Render();
		void DebugRender();

	};
}

// 렌더러 자체를 생성하는 함수를 따로 빼준다.
extern "C" GRAPHICSENGINE_DECLSPEC GraphicsEngineSpace::IRenderer* CreateRenderer();
extern "C" GRAPHICSENGINE_DECLSPEC void DeleteRenderer(GraphicsEngineSpace::IRenderer* obj);