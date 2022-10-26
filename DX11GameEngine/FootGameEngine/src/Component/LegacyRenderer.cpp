#include "GamePch.h"
#include "Component/LegacyRenderer.h"

#include "../../../FootGraphicsEngine/inc/Object/IDXObject.h"
#include "../../FootGraphicsEngine/GraphicsEngineFramework.h"

#include "Component/Camera.h"
#include "Object/GameObject.h"
#include "SceneManager/SceneBase/SceneBase.h"
#include "SceneManager/SceneManager.h"
#include "GraphicsManager/GraphicsManager.h"

using namespace GraphicsEngineSpace;

namespace GameEngineSpace
{
	LegacyRenderer::LegacyRenderer(std::weak_ptr<GameObject> object)
		: RendererBase(object)
	{
	}

	void LegacyRenderer::Init(std::string objName, std::wstring diffuseMap, std::wstring normalMap)
	{
		renderObj = Factory::GetInstance()->CreateDXObject<LegacyObj>(
		BuilderManger::GetInstance()->GetBuilder("Basic32Builder"), objName, diffuseMap, normalMap);

		GraphicsManager::GetInstance()->GetRenderer()->AddRenderObj(renderObj);
	}

	void LegacyRenderer::Render()
	{
		renderObj->Render();
	}

	void LegacyRenderer::PrepareRender(float tick)
	{
		std::shared_ptr<Camera> mainCam = SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera();

		renderObj->Update(transform->GetWorldTM(), mainCam->GetViewMatrix(), mainCam->GetProjMatrix());

		auto cameraWorldPos = mainCam->GetGameObject()->GetComponent<Transform>()->GetWorldPosition();

		DirectionalLight dLight = *GraphicsManager::GetInstance()->GetDirectionalLight();


		renderObj->LightUpdate(dLight, {}, {}, cameraWorldPos);
	}
}
