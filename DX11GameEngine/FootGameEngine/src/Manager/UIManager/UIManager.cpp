#include "GamePch.h"
#include "UIManager/UIManager.h"

#include "GraphicsManager/GraphicsManager.h"
#include "InputManager/InputManager.h"
#include "SceneManager/SceneManager.h"
#include "WindowManager/WindowManager.h"

#include "Timer/Timer.h"

/*
namespace GameEngineSpace
{
	std::shared_ptr<UIManager> UIManager::instance = nullptr;

	std::shared_ptr<UIManager> UIManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<UIManager>();

		return instance;
	}

	void UIManager::Init()
	{
		// 그래픽스 매니저 캐싱
		graphicsInstance = GraphicsManager::GetInstance();
		inputInstance = InputManager::GetInstance();
		soundInstance = SoundManager::GetInstance();

		// 캔버스 생성i
		auto renderer = graphicsInstance->GetRenderer();
		auto width = WindowManager::GetInstance()->GetClientWidth();
		auto height = WindowManager::GetInstance()->GetClientHeight();
		nowWidth = width;
		nowHeight = height;

		mainCanvas = renderer->CreateFactory()->CreateCanvas("Canvas", width, height);

		/// 게임에서 사용할 모든 캔버스들의 목록
		// 시작 위치 제단 저주 - CurseAltar
		mainCanvas->CreateCanvas("CurseAltar");
		// 시작 위치 제단 초월 - EnhanceAltar
		mainCanvas->CreateCanvas("EnhanceAltar");
		// 무기 제단 - WeaponAltar
		mainCanvas->CreateCanvas("WeaponAltar");
		// 인벤토리 - Inventory
		mainCanvas->CreateCanvas("Inventory");
		// 디버그 캔버스 -> DebugCanvas;
		mainCanvas->CreateCanvas("DebugCanvas");

		// 모놀로그 캔버스
		mainCanvas->CreateCanvas("MonologueCanvas");
		// 대화창 캔버스 -> DialogCanvas;
		mainCanvas->CreateCanvas("DialogCanvas");

		// 튜토리얼 들을 모아둔 튜토리얼 캔버스
		mainCanvas->CreateCanvas("TutorialCanvas");

		// 엔딩 씬
		mainCanvas->CreateCanvas("EndingCanvas");
	}

	void UIManager::CreateCanvasAll()
	{

		CreateInventory();
		CreateCurseAltar();
		CreateEnhanceAltar();
		CreateWeaponAltar();
		CreateDebugCanvas();
		//CreateBossCanvas();
		CreateMonologueCanvas();
		CreateDialogCanvas();
		CreateTutorialCanvas();
		CreateEndingCanvas();
	}

	void UIManager::Render()
	{

		// 캔버스로 통합
		mainCanvas->Render(graphicsInstance->GetRenderer().get(), Timer::GetInstance()->DeltaTime());
	}

	// 해당 함수는 Input 업데이트 이후에 있어야한다.
	void UIManager::Update()
	{
		auto mousePos = inputInstance->GetMousePos();

		ButtonUI* collidedButton = mainCanvas->CollidedButtonWithMouse(mousePos.x, mousePos.y, inputInstance->GetInputState(VK_LBUTTON, KeyState::STAY));

	}

	void UIManager::LateUpdate()
	{
	}

	void UIManager::Release()
	{
		mainCanvas->Release();
	}

	Canvas* UIManager::CreateCanvas(const std::string& UIIDstr)
	{
		Canvas* newCanvas = mainCanvas->CreateCanvas(UIIDstr);
		newCanvas->SetScale({ mainCanvas->GetWidth() / static_cast<float>(nowWidth), mainCanvas->GetHeight() / static_cast<float>(nowHeight) });

		return newCanvas;
	}

	void UIManager::CreateUIText(const std::string& UIIDstr, const std::string& text, Vector color, Vector position, float rotation,
		Vector scale, Canvas* parentCanvas)
	{
		TextUI* tempText;
		if (parentCanvas == nullptr)
			tempText = mainCanvas->CreateTextUI(UIIDstr);
		else
			tempText = parentCanvas->CreateTextUI(UIIDstr);

		tempText->SetText(text);
		tempText->SetColor(color);
		tempText->SetPosition(position);
		tempText->SetRotation(VectorReplicate(rotation));
		tempText->SetScale(scale);
	}

	void UIManager::SetTextUIText(const std::string& UIIDstr, const std::string& text, Canvas* parentCanvas)
	{

		TextUI* result;
		if (parentCanvas == nullptr)
			result = mainCanvas->GetTextUI(UIIDstr);
		else
			result = parentCanvas->GetTextUI(UIIDstr);

		if (result != nullptr)
			result->SetText(text);
	}

	void UIManager::SetTextUIColor(const std::string& UIIDstr, Vector color, Canvas* parentCanvas)
	{
		TextUI* result;
		if (parentCanvas == nullptr)
			result = mainCanvas->GetTextUI(UIIDstr);
		else
			result = parentCanvas->GetTextUI(UIIDstr);

		if (result != nullptr)
			result->SetColor(color);
	}

	void UIManager::SetTextUIPosition(const std::string& UIIDstr, Vector position, Canvas* parentCanvas)
	{

		TextUI* result;
		if (parentCanvas == nullptr)
			result = mainCanvas->GetTextUI(UIIDstr);
		else
			result = parentCanvas->GetTextUI(UIIDstr);

		if (result != nullptr)
			result->SetPosition(position);
	}

	void UIManager::SetTextUIScale(const std::string& UIIDstr, Vector scale, Canvas* parentCanvas)
	{

		TextUI* result;
		if (parentCanvas == nullptr)
			result = mainCanvas->GetTextUI(UIIDstr);
		else
			result = parentCanvas->GetTextUI(UIIDstr);

		if (result != nullptr)
			result->SetScale(scale);
	}

	void UIManager::SetTextUIRotation(const std::string& UIIDstr, float rotation, Canvas* parentCanvas)
	{
		TextUI* result;
		if (parentCanvas == nullptr)
			result = mainCanvas->GetTextUI(UIIDstr);
		else
			result = parentCanvas->GetTextUI(UIIDstr);

		if (result != nullptr)
			result->SetRotation(VectorReplicate(rotation));
	}

	void UIManager::SetTextUIIsEnable(const std::string& UIIDstr, bool isEnable, Canvas* parentCanvas)
	{
		TextUI* result;
		if (parentCanvas == nullptr)
			result = mainCanvas->GetTextUI(UIIDstr);
		else
			result = parentCanvas->GetTextUI(UIIDstr);

		if (result != nullptr)
			result->SetEnable(isEnable);
	}

	void UIManager::SetTextUIAnchor(const std::string& UIIDstr, VerticalLocation vertical,
		HorizontalLocation horizontal, Canvas* parentCanvas)
	{
	}

	void UIManager::CreateUISprite(const std::string& UIIDstr, const std::string& textureName,
		const std::string& texturePath, long posX, long posY, long width, long height, float z, Canvas* parentCanvas)
	{
		TextureUI* newTextureUI;

		if (parentCanvas == nullptr)
			newTextureUI = mainCanvas->CreateTextureUI(UIIDstr);
		else
			newTextureUI = parentCanvas->CreateTextureUI(UIIDstr);

		auto* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		auto* texture = resourceManager->GetTexture(textureName);

		if (texture == nullptr)
			texture = graphicsInstance->GetRenderer()->CreateFactory()->CreateTexture(textureName, texturePath);

		newTextureUI->SetTexture(texture);
		newTextureUI->SetPosition(Vector{ static_cast<float>(posX), static_cast<float>(posY), z });
		newTextureUI->SetWidth(width);
		newTextureUI->SetHeight(height);
	}

	void UIManager::CreateUIButton(const std::string& UIIDstr, const std::string& noneTextureName, const std::string& noneTexturePath, long posX, long posY, long width, long height, float z, Canvas* parentCanvas)
	{
		ButtonUI* newTextureUI;

		if (parentCanvas == nullptr)
			newTextureUI = mainCanvas->CreateButtonUI(UIIDstr);
		else
			newTextureUI = parentCanvas->CreateButtonUI(UIIDstr);

		auto* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		// Default Texture
		auto* texture = resourceManager->GetTexture(noneTextureName);

		if (texture == nullptr)
			texture = graphicsInstance->GetRenderer()->CreateFactory()->CreateTexture(noneTextureName, noneTexturePath);

		newTextureUI->SetDefaultTexture(texture);

		newTextureUI->SetPosition(Vector{ static_cast<float>(posX), static_cast<float>(posY), z });
		newTextureUI->SetWidth(width);
		newTextureUI->SetHeight(height);
	}

	void UIManager::CreateUIButton(const std::string& UIIDstr, const std::string& noneTextureName,
		const std::string& noneTexturePath, const std::string& hoverTextureName, const std::string& hoverTexturePath,
		const std::string& downTextureName, const std::string& downTexturePath, long posX, long posY, long width,
		long height, float z, Canvas* parentCanvas)
	{

		ButtonUI* newTextureUI;

		if (parentCanvas == nullptr)
			newTextureUI = mainCanvas->CreateButtonUI(UIIDstr);
		else
			newTextureUI = parentCanvas->CreateButtonUI(UIIDstr);

		auto* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		// Default Texture
		auto* texture = resourceManager->GetTexture(noneTextureName);

		if (texture == nullptr)
			texture = graphicsInstance->GetRenderer()->CreateFactory()->CreateTexture(noneTextureName, noneTexturePath);

		newTextureUI->SetDefaultTexture(texture);

		// Hover Texture
		texture = resourceManager->GetTexture(hoverTextureName);

		if (texture == nullptr)
			texture = graphicsInstance->GetRenderer()->CreateFactory()->CreateTexture(hoverTextureName, hoverTexturePath);

		newTextureUI->SetHoverTexture(texture);

		// Press Texture
		texture = resourceManager->GetTexture(downTextureName);

		if (texture == nullptr)
			texture = graphicsInstance->GetRenderer()->CreateFactory()->CreateTexture(downTextureName, downTexturePath);

		newTextureUI->SetPressTexture(texture);

		newTextureUI->SetPosition(Vector{ static_cast<float>(posX), static_cast<float>(posY), z });
		newTextureUI->SetWidth(width);
		newTextureUI->SetHeight(height);
	}

	UIBase* UIManager::CreateProgressUI(const std::string& UIIDstr, const std::string& frontTextureName, const std::string& fontTexturePath, RECT frontSrc, const std::string& backTextureName, const std::string& backTexturePath, RECT backSrc, long posX, long posY, long width, long height, float z, Canvas* parentCanvas)
	{
		return nullptr;
	}

	void UIManager::SetSpriteTexture(const std::string& UIIDstr, const std::string& textureName, const std::string& texturePath, Canvas* parentCanvas)
	{
		auto* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		// Default Texture
		auto* texture = resourceManager->GetTexture(textureName);

		if (texture == nullptr)
			texture = graphicsInstance->GetRenderer()->CreateFactory()->CreateTexture(textureName, texturePath);

		if (parentCanvas == nullptr)
			mainCanvas->GetTextureUI(UIIDstr)->SetTexture(texture);
		else
			parentCanvas->GetTextureUI(UIIDstr)->SetTexture(texture);
	}

	void UIManager::SetSpritePosition(const std::string& UIIDstr, long posX, long posY, Canvas* parentCanvas)
	{
		if (parentCanvas == nullptr)
			mainCanvas->GetTextureUI(UIIDstr)->SetPosition({ static_cast<float>(posX), static_cast<float>(posY) });
		else
			parentCanvas->GetTextureUI(UIIDstr)->SetPosition({ static_cast<float>(posX), static_cast<float>(posY) });
	}

	void UIManager::SetSpriteSize(const std::string& UIIDstr, long width, long height, Canvas* parentCanvas)
	{
		if (parentCanvas == nullptr)
		{
			mainCanvas->GetTextureUI(UIIDstr)->SetWidth(width);
			mainCanvas->GetTextureUI(UIIDstr)->SetHeight(height);
		}
		else
		{
			parentCanvas->GetTextureUI(UIIDstr)->SetWidth(width);
			parentCanvas->GetTextureUI(UIIDstr)->SetHeight(height);
		}
	}

	void GameEngineSpace::UIManager::SetSpriteIsEnable(const std::string& UIIDstr, bool isEnable, Canvas* parentCanvas)
	{
		if (parentCanvas == nullptr)
			mainCanvas->GetTextureUI(UIIDstr)->SetEnable(isEnable);
		else
			parentCanvas->GetTextureUI(UIIDstr)->SetEnable(isEnable);
	}

	void UIManager::SetSpriteNoneTexture(const std::string& UIIDstr, const std::string& textureName, const std::string& texturePath, Canvas* parentCanvas)
	{
		auto* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		// Default Texture
		auto* texture = resourceManager->GetTexture(textureName);

		if (texture == nullptr)
			texture = graphicsInstance->GetRenderer()->CreateFactory()->CreateTexture(textureName, texturePath);

		if (parentCanvas == nullptr)
			mainCanvas->GetButtonUI(UIIDstr)->SetDefaultTexture(texture);
		else
			parentCanvas->GetButtonUI(UIIDstr)->SetDefaultTexture(texture);
	}

	void UIManager::SetSpriteHoverTexture(const std::string& UIIDstr, const std::string& textureName, const std::string& texturePath, Canvas* parentCanvas)
	{

		auto* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		// Default Texture
		auto* texture = resourceManager->GetTexture(textureName);

		if (texture == nullptr)
			texture = graphicsInstance->GetRenderer()->CreateFactory()->CreateTexture(textureName, texturePath);

		if (parentCanvas == nullptr)
			mainCanvas->GetButtonUI(UIIDstr)->SetHoverTexture(texture);
		else
			parentCanvas->GetButtonUI(UIIDstr)->SetHoverTexture(texture);
	}

	void UIManager::SetSpriteDownTexture(const std::string& UIIDstr, const std::string& textureName, const std::string& texturePath, Canvas* parentCanvas)
	{

		auto* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		// Default Texture
		auto* texture = resourceManager->GetTexture(textureName);

		if (texture == nullptr)
			texture = graphicsInstance->GetRenderer()->CreateFactory()->CreateTexture(textureName, texturePath);

		if (parentCanvas == nullptr)
			mainCanvas->GetButtonUI(UIIDstr)->SetPressTexture(texture);
		else
			parentCanvas->GetButtonUI(UIIDstr)->SetPressTexture(texture);
	}

	void UIManager::SetButtonIsEnable(const std::string& UIIDstr, bool isEnable, Canvas* parentCanvas)
	{
		if (parentCanvas == nullptr)
			mainCanvas->GetButtonUI(UIIDstr)->SetEnable(isEnable);
		else
			parentCanvas->GetButtonUI(UIIDstr)->SetEnable(isEnable);
	}

	void UIManager::SetButtonPosition(const std::string& UIIDstr, long posX, long posY, Canvas* parentCanvas)
	{
		if (parentCanvas == nullptr)
			mainCanvas->GetButtonUI(UIIDstr)->SetPosition({ static_cast<float>(posX), static_cast<float>(posY) });
		else
			parentCanvas->GetButtonUI(UIIDstr)->SetPosition({ static_cast<float>(posX), static_cast<float>(posY) });
	}

	void UIManager::SetButtonSize(const std::string& UIIDstr, long width, long height, Canvas* parentCanvas)
	{
		if (parentCanvas == nullptr)
		{
			mainCanvas->GetButtonUI(UIIDstr)->SetWidth(width);
			mainCanvas->GetButtonUI(UIIDstr)->SetHeight(height);
		}
		else
		{
			parentCanvas->GetButtonUI(UIIDstr)->SetWidth(width);
			parentCanvas->GetButtonUI(UIIDstr)->SetHeight(height);
		}
	}


	ButtonState UIManager::GetButtonState(const std::string& UIIDstr, Canvas* parentCanvas)
	{
		ButtonUI* result;

		if (parentCanvas == nullptr)
			result = mainCanvas->GetButtonUI(UIIDstr);
		else
			result = parentCanvas->GetButtonUI(UIIDstr);

		if (result == nullptr)
			return ButtonState::DEFAULT;

		return result->GetButtonState();
	}

	void UIManager::OnResize(uint32 width, uint32 height)
	{
		mainCanvas->SetWidth(width);
		mainCanvas->SetHeight(height);

		// 비율 계산
		float xScale = static_cast<float>(width) / nowWidth;
		float yScale = static_cast<float>(height) / nowHeight;

		// 모든 캔버스의 스케일을 변경합니다.
		//mainCanvas->GetCanvas("StartAltar")->SetScale({xScale, yScale});
		mainCanvas->SetScaleAllCanvas({ xScale, yScale });
	}

	Canvas* UIManager::GetCanvas(const std::string& canvasIDStr)
	{
		return mainCanvas->GetCanvas(canvasIDStr);
	}

	void UIManager::ClearUI()
	{
		//mainCanvas->Release();
	}
}
*/
