#pragma once
/// Dll 매크로
#ifdef FOOTGRAPHICSENGINE_EXPORTS
#define GRAPHICSENGINE_DECLSPEC __declspec(dllexport)
#else
#define GRAPHICSENGINE_DECLSPEC __declspec(dllimport)
#endif

namespace GraphicsEngineSpace
{
	class IDXObject;
	class Canvas;

	/**
	* \brief Renderer의 기능만 정리해둔 인터페이스 클래스
	*
	* 2022.06.14(화) Dev.BlackFoot
	*/
	class GRAPHICSENGINE_DECLSPEC IRenderer
	{
	public:
		IRenderer() = default;
		virtual ~IRenderer() {}

		// 반드시 구현해줘야하는 목록
		// 초기화 => Device, DC, SwapChain 생성
		// 초기화 할 때 받아서 Render에게 넘겨준다.
		virtual bool Initialize(HWND _hWnd, int _clientWidth, int _clientHeight) abstract;


		// 렌더러 자체의 초기화가 끝난 후, 오브젝트 들을 초기화 해주는 함수 추가
		virtual void InitObject() abstract;

		// 엔진 종료
		virtual void Finalize() abstract;
		// 화면 창이 바뀌었을 때 해주는 onResize(렌더 타겟 생성)
		virtual void OnResize() abstract;

		// 그리기 함수
			// Begin, Render, DebugRender 까지만 들어가 있는 렌더 함수
		virtual void RenderAll() abstract;
		// 그리기 종료함수
			// present로 최종 렌더를 마치는 함수
		virtual void EndRender() abstract;

		// 창 사이즈와 관련된 함수 목록 => 외부에서 사용 가능해야하기 때문에 인터페이스에 추가
		virtual bool IsVaildDevice() abstract;
		virtual void SetClientSize(int _width, int _height) {}
		virtual void SetWinMinMax(bool _isMinimized, bool _isMaximized) {}

		// 화면비 얻어오기
		virtual float GetAspectRatio() const abstract;

		// 렌더 오브젝트 추가.
		virtual void AddRenderObj(std::shared_ptr<IDXObject> obj) abstract;
		
		// 씬이 바뀔 때 렌더 obj를 지워주는 함수
		virtual void ClearRenderVector() abstract;

		// UI 관련 생성함수
		virtual std::shared_ptr<Canvas> CreateCanvas(const std::string& name, float width, float height) abstract;
	};

}