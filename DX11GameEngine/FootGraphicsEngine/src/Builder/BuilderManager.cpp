#include "GraphicsPch.h"
#include "Builder/BuilderManager.h"
#include "Builder/IBuilder.h"
#include "Builder/Basic32Builder.h"
#include "Builder/PosColorBuilder.h"
#include "Builder/TextureRectBuilder.h"

// �߰��� �������� include ���ش�.
	// TODO : �ð� �� �� ���� �� �ִ� �͵��� �����
	// �ϴ� ����� �ڵ带 �Űܺ���.


namespace GraphicsEngineSpace
{
	BuilderManger* BuilderManger::m_Instance = nullptr;

	void BuilderManger::InitBuilderAll(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	{
		// ����ó��
		assert(pDevice);
		assert(pDeviceContext);
		assert(RasterizerState::GetSolidRS());
		assert(RasterizerState::GetWireFrameRS());

		// ��� ���� �ʱ�ȭ.
		m_D3DDevice = pDevice;
		m_D3DDeviceContext = pDeviceContext;

		// ������ ���ø� �Լ��� �־��
		AddBuilder<Basic32Builder>("Basic32Builder");
		AddBuilder<PosColorBuilder>("PosColorBuilder");
		AddBuilder<TextureRectBuilder>("TextureRectBuilder");
	}


	void BuilderManger::DeleteBuildManager()
	{
		// ���� ��ȸ�ϸ鼭 ��� ������ �����ش�.
		for (auto _builder : m_BuilderMap)
		{
			delete _builder.second;
		}

		m_BuilderMap.clear();

		if (m_Instance != nullptr)
		{
			SafeDelete(m_Instance);
		}
	}

	IBuilder* BuilderManger::GetBuilder(std::string _builderName)
	{
		// Ű�� �´� ������ ã�Ƽ� ��������
		return m_BuilderMap.at(_builderName);
	}

}
