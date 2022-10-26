#pragma once
#include "Builder/IBuilder.h"

class CASEParser;

namespace GraphicsEngineSpace
{
	/**
	 * \brief Basic32���̴��� ����Ͽ� �ؽ���, �������� �޴� ������Ʈ�� ����� �����Դϴ�.
	 *
	 * �ϵ� �ڵ��� �޾�����.. filePath�� �޾Ƽ� ASE�Ľ��� �Ҽ��� �ֵ��� ������� �մϴ�.
	 * 2022.07.07(��) Dev.BlackFoot
	 */
	class Basic32Builder : public IBuilder
	{
		// ���������� Device�� ������ �־�� �Ѵ�.
			// ��� �Լ��� �������� �Ǹ鼭 ���� �κп��� ����ϴ� ���� ��� ������ �ʿ�������.
		ID3D11Device* m_D3DDevice;
		ID3D11DeviceContext* m_D3DDeviceContext;

		// ������ ASE �ļ��� ������ �ִ´�. (�⺻������ ������ �� ���� �����Ǳ� �����̴�.)
		CASEParser* m_MeshASEParser;
		// ������Ʈ�� ������Ʈ�� �Ѱ���� �����ϰ�.. Ư�� GeomObject�� �о��ִ� index�� ��������
			// ������ ������Ʈ ��ȣ�� �����ϰų�..
		int m_GeomIdx;

	public:
		Basic32Builder()
			: m_D3DDevice(nullptr)
			, m_D3DDeviceContext(nullptr)
			, m_MeshASEParser(nullptr)
			, m_GeomIdx(-1)
		{}
		~Basic32Builder() override;

		// BuildDXObj�� �������� �� �� ������
		virtual std::shared_ptr <IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName) override;
		virtual std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, std::wstring _texFile) override;
		virtual std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, std::wstring diffuseMap, std::wstring normalMap) override;

		// ���������� �ϴ� ���� ���� ��״� ��ǻ� �ϳ��� ���ĵ� �ɰ� ������..
		virtual std::shared_ptr<ObjectResources> BuildGeometry(std::string objectName) override;

		virtual void InitBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pDC) override;

	private:
		// �ϵ� �ڵ����� �־��� ������Ʈ �����
			// Object�� buildInfo�� ������ �־���ϴ� �ʿ伺
			// �������� BuildeInfo�� �����͸� �־��� ��, �ش� �����͸� ���� ���� �� �� ����ϱ� ������ �ݵ�� �ʿ��ϴ�.
			// ������Ʈ ���ҽ��� �޾Ƽ� ���ο��� ä���ְ� ��ȯ���ش�.
		//ObjectResources* BuildBoxResources(ObjectResources* _objRes);

		// ASE���� ������ ���� Mesh�� Resources ������ �߰����ݴϴ�.
		std::shared_ptr<ObjectResources> BuildMeshASEResources(std::shared_ptr<ObjectResources> _objRes);
	};
}
