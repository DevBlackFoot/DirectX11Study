#pragma once
#include "Builder/IBuilder.h"

namespace GraphicsEngineSpace
{
	/**
	 * \brief TextureRect Obj�� ���� ������ �־��ִ� Builder Ŭ����
	 */
	class TextureRectBuilder : public IBuilder
	{
		ID3D11Device* D3DDevice;
		ID3D11DeviceContext* D3DDeviceContext;

	public:
		TextureRectBuilder()
			: D3DDevice(nullptr)
			, D3DDeviceContext(nullptr)
		{
			
		}
		~TextureRectBuilder() override {}

		std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName) override;
		std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, std::wstring _texFile) override;
		std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, std::wstring diffuseMap, std::wstring normalMap) override;
		std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, ID3D11ShaderResourceView* shaderResource) override;
		std::shared_ptr<ObjectResources> BuildGeometry(std::string objectName) override;

		void InitBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pDC) override;

		// �⺻������ �簢���� 2���� �ؽ��� �ϳ��� ������ݴϴ�.
	};
}