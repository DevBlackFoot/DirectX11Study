#pragma once
#include "Builder/IBuilder.h"

class CASEParser;

namespace GraphicsEngineSpace
{
	class ResourceManager;

	/**
	 * \brief Basic32���̴��� ����Ͽ� �ؽ���, �������� �޴� ������Ʈ�� ����� �����Դϴ�.
	 *
	 * �ϵ� �ڵ��� �޾�����.. filePath�� �޾Ƽ� ASE�Ľ��� �Ҽ��� �ֵ��� ������� �մϴ�.
	 * 2022.07.07(��) Dev.BlackFoot
	 */
	class LegacyStaticBuilder : public IBuilder
	{
		// ���������� Device�� ������ �־�� �Ѵ�.
			// ��� �Լ��� �������� �Ǹ鼭 ���� �κп��� ����ϴ� ���� ��� ������ �ʿ�������.
		ID3D11Device* D3DDevice;
		ID3D11DeviceContext* D3DDeviceContext;

		// ������ ASE �ļ��� ������ �ִ´�. (�⺻������ ������ �� ���� �����Ǳ� �����̴�.)
		CASEParser* meshASEParser;
		// ������Ʈ�� ������Ʈ�� �Ѱ���� �����ϰ�.. Ư�� GeomObject�� �о��ִ� index�� ��������
			// ������ ������Ʈ ��ȣ�� �����ϰų�..
		int geomIdx;

		std::shared_ptr<ResourceManager> resourceManager;

	public:
		LegacyStaticBuilder()
			: D3DDevice(nullptr)
			, D3DDeviceContext(nullptr)
			, meshASEParser(nullptr)
			, geomIdx(-1)
			, resourceManager(nullptr)
		{}
		~LegacyStaticBuilder() override;

		virtual std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, uint64 objectID) override;
		virtual std::shared_ptr<ObjectResources> BuildGeometry(std::string objectName, uint64 objectID) override;
		virtual std::shared_ptr<IDXObject> AddTexture(std::shared_ptr<IDXObject> DXObj, uint64 textureID,
		                                              std::string textureName, std::wstring path, RenderingData::TextureMapType mapType) override;

		virtual void InitBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pDC) override;

	private:
		// �ϵ� �ڵ����� �־��� ������Ʈ �����
			// Object�� buildInfo�� ������ �־���ϴ� �ʿ伺
			// �������� BuildeInfo�� �����͸� �־��� ��, �ش� �����͸� ���� ���� �� �� ����ϱ� ������ �ݵ�� �ʿ��ϴ�.
			// ������Ʈ ���ҽ��� �޾Ƽ� ���ο��� ä���ְ� ��ȯ���ش�.

		// ASE���� ������ ���� Mesh�� Resources ������ �߰����ݴϴ�.
		std::shared_ptr<ObjectResources> BuildMeshASEResources(std::shared_ptr<ObjectResources> _objRes, uint64 objectID);
	};
}
