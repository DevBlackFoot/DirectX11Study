#include "GamePch.h"
#include "Component/PosColorRenderer.h"

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
	PosColorRenderer::PosColorRenderer(std::weak_ptr<GameObject> object)
		: RendererBase(object)
	{
	}

	void PosColorRenderer::Init(std::string _objName, uint64 objID)
	{
		// 이 지점에서 새로 리셋
		renderObj = Factory::GetInstance()->CreateDXObject<LineObj>(
		BuilderManger::GetInstance()->GetBuilder("LineBuilder"), _objName, objID);

		// 그려지는 오브젝트로 넣어줍니다.
		GraphicsManager::GetInstance()->GetRenderer()->AddRenderObj(renderObj);
	}

	void PosColorRenderer::Render()
	{
		renderObj->Render();
	}

	void PosColorRenderer::PrepareRender(float tick)
	{
		// 씬 매니저에서 얻어와서 오브젝트 업데이트
		std::shared_ptr<Camera> mainCam = SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera();
		renderObj->Update(transform->GetWorldTM(), mainCam->GetViewMatrix(), mainCam->GetProjMatrix());
	}
}
