#include "GraphicsPch.h"
#include "Builder/BuilderManager.h"
#include "Builder/IBuilder.h"

// 추가할 빌더들을 include 해준다.
	// TODO : 시간 될 때 묶을 수 있는 것들을 묶어보자
	// 일단 현재는 코드를 옮겨본다.
#include "Builder/LegacyStaticBuilder.h"
#include "Builder/LineBuilder.h"
#include "Builder/SkyBoxBuilder.h"


namespace GraphicsEngineSpace
{
	BuilderManger* BuilderManger::m_Instance = nullptr;

	void BuilderManger::InitBuilderAll(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	{
		// 예외처리
		assert(pDevice);
		assert(pDeviceContext);
		assert(RasterizerState::GetSolidRS());
		assert(RasterizerState::GetWireFrameRS());

		// 멤버 변수 초기화.
		m_D3DDevice = pDevice;
		m_D3DDeviceContext = pDeviceContext;

		// 만들어둔 템플릿 함수로 넣어보기
		AddBuilder<LegacyStaticBuilder>("LegacyStaticBuilder");
		AddBuilder<LineBuilder>("LineBuilder");
		AddBuilder<SkyBoxBuilder>("SkyBoxBuilder");
	}


	void BuilderManger::DeleteBuildManager()
	{
		// 맵을 순회하면서 모든 정보를 지워준다.
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
		// 키에 맞는 빌더를 찾아서 내보내줌
		return m_BuilderMap.at(_builderName);
	}

}
