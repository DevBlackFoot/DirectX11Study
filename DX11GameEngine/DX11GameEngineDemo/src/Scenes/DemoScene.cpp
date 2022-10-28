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
		// 각종 오브젝트들 생성

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

		std::shared_ptr<GameObject> genji = CreateEmpty();
		genji->AddComponent<LegacyRenderer>()->Init("Resources/Model/genji_max.ASE", L"Resources/Texture/000000002405.dds", L"Resources/Texture/000000002406_b.dds");
		genji->GetComponent<Transform>()->SetPosition(Vector3{5.f, 0.f, 0.f});
	}
}
