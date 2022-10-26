#pragma once

// 기본적으로 지훈이 형의 엔진에서 가져왔기 때문에 터진다..


/*#include "../GraphicsEngine/GraphicsEngineFramework.h"

namespace GameEngineSpace
{
	class GraphicsManager;
	class InputManager;
	class SoundManager;

	
	using GraphicsEngineSpace::ButtonState;
	using GraphicsEngineSpace::Canvas;
	using GraphicsEngineSpace::UIBase;
	using GraphicsEngineSpace::VerticalLocation;
	using GraphicsEngineSpace::HorizontalLocation;
	*/
	
	/**
	 * \brief UI, 텍스트 등 화면에 2차원으로 존재하는 UI 들만을 렌더링 하는 UI Manager 클래스
	 *
	 * 2022.08.08(월) Deb.BlackFoot
	 */
/*
	class UIManager final
	{
	private:
		// UI Manager의 인스턴스
		static std::shared_ptr<UIManager> instance;

		// 내부적으로 그래픽스 매니저 캐싱
		std::shared_ptr<GraphicsManager> graphicsInstance;
		// 내부적으로 인풋 매니저 캐싱
		std::shared_ptr<InputManager> inputInstance;
		// 내부적으로 soundManager 캐싱
		std::shared_ptr<SoundManager> soundInstance;

		//// 화면에 그려지는 모든 Text정보를 담고 있는 맵(디버깅 데이터 빼고)
		//std::map<std::string, std::shared_ptr<UIText>> UITextInScene;

		//// 화면에 그려지는 모든
		//std::map<std::string, std::shared_ptr<UISprite>> UISpriteInScene;

		// 캔버스
			// 게임에 있는 모든 UI 캔버스들을 init시 만들 수 있지 않을까.
			// UI로 렌더되는 모든 녀석들을 처음 Init 할 때 만들고 enable을 false로 지정해주고
			// 특정 상황에 필요할 때 해당 UI 아이디를 가져와서 만들어준다.
		Canvas* mainCanvas;

		// 화면 정보를 파악하고. 스케일을 늘려보자..
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

		// WIC 이슈를 피하기 위한 Canvas 생성
		void CreateCanvasAll();

		// 렌더
		void Render();

		// 버튼 입력을 위한 업데이트와 LateUpdate
		void Update();
		void LateUpdate();

		void Release();

		Canvas* CreateCanvas(const std::string& UIIDstr);

		// UIText
		void CreateUIText(const std::string& UIIDstr, const std::string& text, Vector color, Vector position, float rotation, Vector scale, Canvas* parentCanvas = nullptr);

		void SetTextUIText(const std::string& UIIDstr, const std::string& text, Canvas* parentCanvas = nullptr);
		void SetTextUIColor(const std::string& UIIDstr, Vector color, Canvas* parentCanvas = nullptr);
		void SetTextUIPosition(const std::string& UIIDstr, Vector position, Canvas* parentCanvas = nullptr);
		void SetTextUIScale(const std::string& UIIDstr, Vector scale, Canvas* parentCanvas = nullptr);
		void SetTextUIRotation(const std::string& UIIDstr, float rotation, Canvas* parentCanvas = nullptr);
		void SetTextUIIsEnable(const std::string& UIIDstr, bool isEnable, Canvas* parentCanvas = nullptr);

		void SetTextUIAnchor(const std::string& UIIDstr, VerticalLocation vertical, HorizontalLocation horizontal, Canvas* parentCanvas = nullptr);

		// UISprite
		// 그림 하나.
		void CreateUISprite(const std::string& UIIDstr, const std::string& textureName, const std::string& texturePath, long posX, long posY, long width, long height, float z, Canvas* parentCanvas = nullptr);
		// 버튼 용 그림 세개 한번에 받기..
		void CreateUIButton(const std::string& UIIDstr, const std::string& noneTextureName, const std::string& noneTexturePath,
			long posX, long posY, long width, long height, float z, Canvas* parentCanvas = nullptr);
		void CreateUIButton(const std::string& UIIDstr, const std::string& noneTextureName, const std::string& noneTexturePath,
			const std::string& hoverTextureName, const std::string& hoverTexturePath, const std::string& downTextureName, const std::string& downTexturePath,
			long posX, long posY, long width, long height, float z, Canvas* parentCanvas = nullptr);

		UIBase* CreateProgressUI(const std::string& UIIDstr, const std::string& frontTextureName, const std::string& fontTexturePath,
			RECT frontSrc, const std::string& backTextureName, const std::string& backTexturePath, RECT backSrc,
			long posX, long posY, long width, long height, float z, Canvas* parentCanvas = nullptr);

		// 각종 세팅.
		void SetSpriteTexture(const std::string& UIIDstr, const std::string& textureName, const std::string& texturePath, Canvas* parentCanvas = nullptr);
		void SetSpritePosition(const std::string& UIIDstr, long posX, long posY, Canvas* parentCanvas = nullptr);
		void SetSpriteSize(const std::string& UIIDstr, long width, long height, Canvas* parentCanvas = nullptr);
		void SetSpriteIsEnable(const std::string& UIIDstr, bool isEnable, Canvas* parentCanvas = nullptr);

		void SetSpriteNoneTexture(const std::string& UIIDstr, const std::string& textureName, const std::string& texturePath, Canvas* parentCanvas = nullptr);
		void SetSpriteHoverTexture(const std::string& UIIDstr, const std::string& textureName, const std::string& texturePath, Canvas* parentCanvas = nullptr);
		void SetSpriteDownTexture(const std::string& UIIDstr, const std::string& textureName, const std::string& texturePath, Canvas* parentCanvas = nullptr);
		void SetButtonIsEnable(const std::string& UIIDstr, bool isEnable, Canvas* parentCanvas = nullptr);
		void SetButtonPosition(const std::string& UIIDstr, long posX, long posY, Canvas* parentCanvas = nullptr);
		void SetButtonSize(const std::string& UIIDstr, long width, long height, Canvas* parentCanvas = nullptr);
	
		// 특정 버튼의 상태를 알고 싶을 때..
		ButtonState GetButtonState(const std::string& UIIDstr, Canvas* parentCanvas = nullptr);

		// 특정 캔버스를 얻고 싶을 때
		Canvas* GetCanvas(const std::string& canvasIDStr);

		// 캔버스 리사이즈
		void OnResize(uint32 width, uint32 height);

		void ClearUI();

	private:
		
		// 각종 캔버스 생성
		void CreateInventory();
		//void CreateStartAltar();		=> 반으로 쪼갰습니다.
		void CreateCurseAltar();
		void CreateEnhanceAltar();

		void CreateWeaponAltar();
		void CreateDebugCanvas();
		void CreateMonologueCanvas();
		void CreateDialogCanvas();
		void CreateTutorialCanvas();
		void CreateEndingCanvas();
	};

}
*/