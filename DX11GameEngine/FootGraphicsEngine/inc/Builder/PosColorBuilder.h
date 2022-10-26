#pragma once
#include "Builder/IBuilder.h"

namespace GraphicsEngineSpace
{
	
	/**
	* \brief PosColor 오브젝트들을 만들어주는 PosColor Builder
	*
	* 2022.07.13(수) Dev.BlackFoot
	*/
	class PosColorBuilder : public IBuilder
	{
		// 내부적으로 Device를 가지고 있어야 한다.
			// 멤버 함수가 여러개가 되면서 여러 부분에서 사용하는 각종 멤버 변수가 필요해졌다.
		ID3D11Device* m_D3DDevice;
		ID3D11DeviceContext* m_D3DDeviceContext;

	public:
		PosColorBuilder()
			: m_D3DDevice(nullptr)
			, m_D3DDeviceContext(nullptr)
		{}
		~PosColorBuilder() override {}

		std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName) override;
		std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, std::wstring _texFile) override;
		std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, std::wstring diffuseMap, std::wstring normalMap) override;
		std::shared_ptr<ObjectResources> BuildGeometry(std::string objectName) override;
		void InitBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pDC) override;

	private:
		// 하드코딩으로 넣어줄 그리드, 축
		std::shared_ptr<ObjectResources> BuildGridResources(std::shared_ptr<ObjectResources> _objRes);
		std::shared_ptr<ObjectResources> BuildAxisResources(std::shared_ptr<ObjectResources> _objRes);

	};

}
