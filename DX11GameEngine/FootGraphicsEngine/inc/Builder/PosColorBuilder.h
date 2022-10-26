#pragma once
#include "Builder/IBuilder.h"

namespace GraphicsEngineSpace
{
	
	/**
	* \brief PosColor ������Ʈ���� ������ִ� PosColor Builder
	*
	* 2022.07.13(��) Dev.BlackFoot
	*/
	class PosColorBuilder : public IBuilder
	{
		// ���������� Device�� ������ �־�� �Ѵ�.
			// ��� �Լ��� �������� �Ǹ鼭 ���� �κп��� ����ϴ� ���� ��� ������ �ʿ�������.
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
		// �ϵ��ڵ����� �־��� �׸���, ��
		std::shared_ptr<ObjectResources> BuildGridResources(std::shared_ptr<ObjectResources> _objRes);
		std::shared_ptr<ObjectResources> BuildAxisResources(std::shared_ptr<ObjectResources> _objRes);

	};

}
