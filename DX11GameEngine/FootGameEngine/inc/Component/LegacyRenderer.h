#pragma once
#include "RendererBase.h"
#include "PCH/FootEngineTypedef.h"

namespace GameEngineSpace
{
	// 이 역시 Mesh 렌더러로 나올 수 있다.
		// 현재 내 구조가 특정 쉐이더를 사용하는 특정 빌더를 가져와서 만드는 구조기 때문에 이렇게 하는 것..
	class LegacyRenderer : public RendererBase
	{
	public:
		LegacyRenderer(std::weak_ptr<GameObject> object);

		virtual ~LegacyRenderer() {}

	public:
		// 빌더에 필요한 Init 함수.. 가변인자 템플릿을 사용하자.
		void Init(std::string objName, uint64 objID, uint64 diffuseID, std::wstring diffuseMap, uint64 normalID, std::wstring normalMap);

		void Render() override;
		void PrepareRender(float tick) override;
	};
}