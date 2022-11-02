#pragma once
#include "Builder/IBuilder.h"

namespace GraphicsEngineSpace
{
	class ResourceManager;

	/**
	 * \brief SkyBox를 만들어주는 Builder
	 */
	class SkyBoxBuilder : public IBuilder
	{
		// 내부적으로 device;
		ID3D11Device* D3DDevice;
		ID3D11DeviceContext* D3DDeviceContext;

		// 사용해줄 리소스 매니저
		std::shared_ptr<ResourceManager> resourceManager;

	public:
		SkyBoxBuilder()
			: D3DDevice(nullptr)
			, D3DDeviceContext(nullptr)
			, resourceManager(nullptr)
		{
			
		}
		~SkyBoxBuilder() override {}

		std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, uint64 objectID) override;
		std::shared_ptr<ObjectResources> BuildGeometry(std::string objectName, uint64 objectID) override;
		void InitBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pDC) override;
		std::shared_ptr<IDXObject> AddTexture(std::shared_ptr<IDXObject> DXObj, uint64 textureID, std::string textureName, std::wstring path, RenderingData::TextureMapType mapType) override;

	private:
		std::shared_ptr<ObjectResources> BuildSkyBoxResources(std::shared_ptr<ObjectResources> _objRes, uint64 objectID);

	};
}
