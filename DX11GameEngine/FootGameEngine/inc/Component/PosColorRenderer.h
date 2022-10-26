#pragma once
#include "RendererBase.h"
#include "PCH/FootEngineTypedef.h"

namespace GameEngineSpace
{
	// 기본적인 PosColor를 뽑아내기 위한 렌더러 컴포넌트..
		// 사실 이후에 그래픽스에 많은게 들어가면 이건 Mesh렌더러 등으로 뽑을 수도 있을 듯 하다.
	class PosColorRenderer : public RendererBase
	{
	private:
		// 현시점에서는 따로 멤버 변수가 있을이유가 없을 것 같다..

	public:
		PosColorRenderer(std::weak_ptr<GameObject> object);

		virtual ~PosColorRenderer() {}

	public:
		// 여기서 초기화 할 때, 오브젝트 이름을 넣어준다.
			// 빌더를 통해서 초기화할 것이기 때문에 지정된 이름을 넣어본다.
		void Init(std::string _objName);

		void Render() override;
		void PrepareRender(float tick) override;
	};

}