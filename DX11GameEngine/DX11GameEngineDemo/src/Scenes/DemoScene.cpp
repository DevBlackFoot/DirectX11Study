#include "pch.h"
#include "Scenes/DemoScene.h"

#include "Component/Camera.h"
#include "Component/BasicCameraController.h"
#include "Component/PosColorRenderer.h"
#include "Component/LegacyRenderer.h"


namespace ClientSpace
{
	DemoScene::DemoScene(tstring sceneName)
		: SceneBase(sceneName)
	{
	}

	DemoScene::~DemoScene()
	{
	}

	void DemoScene::BuildScene()
	{
		// ���� ������Ʈ�� ����

		std::shared_ptr<GameObject> camera = CreateEmpty();
		this->SetMainCamera(camera->AddComponent<Camera>());
		camera->GetComponent<Transform>()->SetPosition(Vector3{0.f, 15.f, 10.f});
		camera->GetComponent<Transform>()->SetRotation(Vector3{-45.f, 0.f, 0.f});
		camera->GetComponent<Camera>()->LookAt(Vector3{0.f, 0.f, 0.f});

		camera->AddComponent<BasicCameraController>();

		std::shared_ptr<GameObject> grid = CreateEmpty();
		grid->AddComponent<PosColorRenderer>()->Init("Grid");

		std::shared_ptr<GameObject> axis = CreateEmpty();
		grid->AddComponent<PosColorRenderer>()->Init("Axis");

		std::shared_ptr<GameObject> normalBox = CreateEmpty();
		normalBox->AddComponent<LegacyRenderer>()->Init("Resources/Model/box.ASE", L"Resources/Texture/bricks.dds", L"Resources/Texture/bricks_nmap.dds");
	}
}
