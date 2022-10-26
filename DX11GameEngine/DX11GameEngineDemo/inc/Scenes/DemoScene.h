#pragma once
#include "Manager/SceneManager/SceneBase/SceneBase.h"

namespace ClientSpace
{
	class DemoScene : public GameEngineSpace::SceneBase
	{
	public:
		DemoScene(tstring sceneName);
		virtual ~DemoScene();

	public:
		virtual void BuildScene() override;
	};
}