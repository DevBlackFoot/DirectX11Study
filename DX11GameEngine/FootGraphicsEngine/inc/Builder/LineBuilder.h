#pragma once
#include "Builder/IBuilder.h"

namespace GraphicsEngineSpace
{
	class ResourceManager;

	/**
	* \brief PosColor ������Ʈ���� ������ִ� PosColor Builder
	*
	* 2022.07.13(��) Dev.BlackFoot
	*/
	class LineBuilder : public IBuilder
	{
		// ���������� Device�� ������ �־�� �Ѵ�.
			// ��� �Լ��� �������� �Ǹ鼭 ���� �κп��� ����ϴ� ���� ��� ������ �ʿ�������.
		ID3D11Device* D3DDevice;
		ID3D11DeviceContext* D3DDeviceContext;

		// ������� ���ҽ� �Ŵ���.
		std::shared_ptr<ResourceManager> resourceManager;
		

	public:
		LineBuilder()
			: D3DDevice(nullptr)
			, D3DDeviceContext(nullptr)
			, resourceManager(nullptr)
		{}
		~LineBuilder() override {}

		std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, uint64 objectID) override;
		//std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, std::string textureName, std::wstring path) override;
		std::shared_ptr<ObjectResources> BuildGeometry(std::string objectName, uint64 objectID) override;
		void InitBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pDC) override;
		std::shared_ptr<IDXObject> AddTexture(std::shared_ptr<IDXObject> DXObj, uint64 textureID,
		                                      std::string textureName, std::wstring path, RenderingData::TextureMapType mapType) override;

	private:
		// �ϵ��ڵ����� �־��� �׸���, ��
		std::shared_ptr<ObjectResources> BuildGridResources(std::shared_ptr<ObjectResources> _objRes, uint64 objectID);
		std::shared_ptr<ObjectResources> BuildAxisResources(std::shared_ptr<ObjectResources> _objRes, uint64 objectID);

	};

}
