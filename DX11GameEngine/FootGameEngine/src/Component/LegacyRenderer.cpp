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

	void LegacyRenderer::Init(std::string objName, uint64 objID, uint64 diffuseID, std::wstring diffuseMap, uint64 normalID, std::wstring normalMap)
	{
		renderObj = Factory::GetInstance()->CreateDXObject<LegacyStaticObj>(
		BuilderManger::GetInstance()->GetBuilder("LegacyStaticBuilder"), objName, objID);

		auto legacyBuilder = BuilderManger::GetInstance()->GetBuilder("LegacyStaticBuilder");
		legacyBuilder->AddTexture(renderObj, diffuseID, "albedo", diffuseMap, RenderingData::TextureMapType::ALBEDO);
		legacyBuilder->AddTexture(renderObj, normalID, "normal", normalMap, RenderingData::TextureMapType::NORMAL);

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
