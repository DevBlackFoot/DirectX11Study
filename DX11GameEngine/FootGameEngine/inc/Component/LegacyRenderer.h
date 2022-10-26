#pragma once
#include "RendererBase.h"
#include "PCH/FootEngineTypedef.h"

namespace GameEngineSpace
{
	// �� ���� Mesh �������� ���� �� �ִ�.
		// ���� �� ������ Ư�� ���̴��� ����ϴ� Ư�� ������ �����ͼ� ����� ������ ������ �̷��� �ϴ� ��..
	class LegacyRenderer : public RendererBase
	{
	public:
		LegacyRenderer(std::weak_ptr<GameObject> object);

		virtual ~LegacyRenderer() {}

	public:
		// ������ �ʿ��� Init �Լ�.. �������� ���ø��� �������.
		void Init(std::string objName, std::wstring diffuseMap, std::wstring normalMap);

		void Render() override;
		void PrepareRender(float tick) override;
	};
}