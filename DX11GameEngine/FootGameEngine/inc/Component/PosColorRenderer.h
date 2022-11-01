#pragma once
#include "RendererBase.h"
#include "PCH/FootEngineTypedef.h"

namespace GameEngineSpace
{
	// �⺻���� PosColor�� �̾Ƴ��� ���� ������ ������Ʈ..
		// ��� ���Ŀ� �׷��Ƚ��� ������ ���� �̰� Mesh������ ������ ���� ���� ���� �� �ϴ�.
	class PosColorRenderer : public RendererBase
	{
	private:
		// ������������ ���� ��� ������ ���������� ���� �� ����..

	public:
		PosColorRenderer(std::weak_ptr<GameObject> object);

		virtual ~PosColorRenderer() {}

	public:
		// ���⼭ �ʱ�ȭ �� ��, ������Ʈ �̸��� �־��ش�.
			// ������ ���ؼ� �ʱ�ȭ�� ���̱� ������ ������ �̸��� �־��.
		void Init(std::string _objName, uint64 objID);

		void Render() override;
		void PrepareRender(float tick) override;
	};

}