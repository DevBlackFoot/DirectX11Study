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
		// �� �������� ���� ����
		renderObj = Factory::GetInstance()->CreateDXObject<LineObj>(
		BuilderManger::GetInstance()->GetBuilder("LineBuilder"), _objName, objID);

		// �׷����� ������Ʈ�� �־��ݴϴ�.
		GraphicsManager::GetInstance()->GetRenderer()->AddRenderObj(renderObj);
	}

	void PosColorRenderer::Render()
	{
		renderObj->Render();
	}

	void PosColorRenderer::PrepareRender(float tick)
	{
		// �� �Ŵ������� ���ͼ� ������Ʈ ������Ʈ
		std::shared_ptr<Camera> mainCam = SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera();
		renderObj->Update(transform->GetWorldTM(), mainCam->GetViewMatrix(), mainCam->GetProjMatrix());
	}
}
