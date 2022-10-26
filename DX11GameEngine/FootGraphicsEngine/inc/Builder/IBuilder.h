#pragma once
#include "Resources/ObjectResources.h"

namespace GraphicsEngineSpace
{

	class IDXObject;

	/**
	* \brief ������ ������ ���������� ������ IBuilder Ŭ����.
	*/
	class IBuilder
	{
	public:
		virtual ~IBuilder() {}

		// ������ ������Ʈ�� �޾Ƽ�, ������Ʈ�� �������ִ� �Լ��� �����
		virtual std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName) abstract;

		// �ؽ��� ���ϱ��� �޴� ����
			// �� ��Ŀ��� ������ ���Ƽ� (�Լ��� ���������� �þ �� �ִ�..)
			// ���Ŀ� �ļ��� �Ľ� ������ ������ �и��� �� ������, �Ľ� ������ ������ Mat������ �̿��ؼ�
			// ��ǻ�� ��, �븻 �� ���� �߰��Ѵ�..
		virtual std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, std::wstring _texFile) abstract;
		virtual std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, std::wstring diffuseMap, std::wstring normalMap) abstract;

		// �ؽ��ĸ� �ٷ� �޴� ����
			// �ݵ�� ������ �ʿ�� ����.
		virtual std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, ID3D11ShaderResourceView* shaderResource)
		{
			return nullptr;
		}

		// �������� ���ο� �����ϴ� ��� ������ �ʱ�ȭ ���ִ� Init �Լ�
			// ���� ���丮�� �ִ� Init�� �����´ٰ� ����.
		virtual void InitBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pDC) abstract;

	protected:
		// �ܺο� �巯�� �ʿ䰡 ����̴� �Լ�.
		// �����δ� ������Ʈ�� �ƴ϶� ���� ���ο��� ������Ʈ�� �������� �߰��ؼ� ������Ʈ�� �־��ش�.
			// ������Ʈ�� ���� ������ �������ִ� �����̳�(BuildInfo)�� ������ �ְ�
			// �ش� �����̳��� ������ �������� ä���� ��ȯ �� ��.
			// ������Ʈ�� �������ش�.
			// �ҿ� ������ ����.
		virtual std::shared_ptr<ObjectResources> BuildGeometry(std::string objectName) abstract;
	};

}