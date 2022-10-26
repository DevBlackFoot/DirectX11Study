#include "GamePch.h"
#include "Component/BasicCameraController.h"

#include "Timer/Timer.h"
#include "InputManager/InputManager.h"

#include "Object/GameObject.h"
#include "Component/Camera.h"
#include "Component/Transform.h"
//#include "UIManager.h"

namespace GameEngineSpace
{

	BasicCameraController::BasicCameraController(std::weak_ptr<GameObject> gameObj)
		: ComponentBase(gameObj, ComponentType::MONOBEHAVIOR), moveSpeed(10.0f)
	{
		transform = GetGameObject()->GetComponent<Transform>();
	}

	BasicCameraController::~BasicCameraController()
	{
	}

	void BasicCameraController::Release()
	{
		camera.reset();
		__super::Release();
	}

	void BasicCameraController::Awake()
	{
		// 카메라를 등록
		camera = GetGameObject()->GetComponent<Camera>();
		/*UIManager::GetInstance()->CreateUIText("CameraUpGE", "CameraLook : ", Vector{ 1.0f, 1.0f, 1.0f }, Vector{ 100.f, 200.f, 1.0f }, 0.f, Vector{ 1.f, 1.f, 1.f });
		UIManager::GetInstance()->CreateUIText("CameraLookGE", "CameraLook : ", Vector{1.0f, 1.0f, 1.0f}, Vector{100.f, 250.f, 1.0f}, 0.f, Vector{1.f, 1.f, 1.f});
		UIManager::GetInstance()->CreateUIText("CameraRightGE", "CameraLook : ", Vector{ 1.0f, 1.0f, 1.0f }, Vector{ 100.f, 300.f, 1.0f }, 0.f, Vector{ 1.f, 1.f, 1.f });*/

	}

	void BasicCameraController::PreUpdate(float tick)
	{
		/*Vector nowUp = transform->GetUp();
		std::string str = "CameraUp : ";
		str += "x : " + std::to_string(nowUp.x) + " y : " + std::to_string(nowUp.y) + " z : " + std::to_string(nowUp.z);
		UIManager::GetInstance()->SetTextUIText("CameraUpGE", str);

		// 현재의 룩정보를 봅시다..
		Vector nowLook = transform->GetLook();
		str = "CameraLook : ";
		str += "x : " + std::to_string(nowLook.x) + " y : " + std::to_string(nowLook.y) + " z : " + std::to_string(nowLook.z);
		UIManager::GetInstance()->SetTextUIText("CameraLookGE", str);

		Vector nowRight = transform->GetRight();
		str = "CameraRight : ";
		str += "x : " + std::to_string(nowRight.x) + " y : " + std::to_string(nowRight.y) + " z : " + std::to_string(nowRight.z);
		UIManager::GetInstance()->SetTextUIText("CameraRightGE", str);*/

	}

	void BasicCameraController::LateUpdate(float tick)
	{
		Vector3 rot = transform->GetWorldRotation();

		float delta = tick;

		camera->UpdateViewMatrix();

		if (InputManager::GetInstance()->GetInputState('W', KeyState::STAY)) // 앞으로
		{
			// 레퍼런스 타입 연산만 가능해서 벡터를 변수로 뽑아줘야한다..!
			Vector3 look = transform->GetLook();
			transform->SetPosition(transform->GetWorldPosition() + (look * moveSpeed * delta));
		}

		if (InputManager::GetInstance()->GetInputState('S', KeyState::STAY)) // s
		{
			Vector3 look = transform->GetLook();	
			transform->SetPosition(transform->GetWorldPosition() - (look * moveSpeed * delta));
		}

		if (InputManager::GetInstance()->GetInputState('A', KeyState::STAY)) // a
		{
			Vector3 right = transform->GetRight();	
			transform->SetPosition(transform->GetWorldPosition() - (right * moveSpeed * delta));
		}

		if (InputManager::GetInstance()->GetInputState('D', KeyState::STAY)) // d
		{
			Vector3 right = transform->GetRight();	
			transform->SetPosition(transform->GetWorldPosition() + (right * moveSpeed * delta));
		}

		if (InputManager::GetInstance()->GetInputState('Q', KeyState::STAY)) // q
		{
			Vector3 up = transform->GetUp();
			transform->SetPosition(transform->GetWorldPosition() - (up * moveSpeed * delta));
		}

		if (InputManager::GetInstance()->GetInputState('E', KeyState::STAY)) // e
		{
			Vector3 up = transform->GetUp();
			transform->SetPosition(transform->GetWorldPosition() + (up * moveSpeed * delta));
		}

		Vector2 currentMousePos = InputManager::GetInstance()->GetMousePos();

		if(InputManager::GetInstance()->GetInputState(VK_RBUTTON, KeyState::STAY))
		{
			// 기존 위치..
			Vector2 distPos = currentMousePos - prevMousePos;
			// 라디안으로 변경해서 해결
			Vector2 deltaMouseMove = 0.15f * distPos * FootMath::Pi/180;

			// World Right Up 기준으로 회전/
			Vector2 worldXY = {deltaMouseMove.y, deltaMouseMove.x};

			// 위의 값으로 회전. => -부호를 붙여서 방향 조절.
			transform->SetRotation(rot - worldXY, Space::WORLD);
		}

		prevMousePos = currentMousePos;
	}
}
