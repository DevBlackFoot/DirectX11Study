#pragma once
#include "Resources/ObjectResources.h"

/// Dll ��ũ��
#ifdef FOOTGRAPHICSENGINE_EXPORTS
#define GRAPHICSENGINE_DECLSPEC __declspec(dllexport)
#else
#define GRAPHICSENGINE_DECLSPEC __declspec(dllimport)
#endif

namespace GraphicsEngineSpace
{
	/**
	 * \brief Factory���� �����ϴ� ��� ������Ʈ�� ���̽��� �Ǵ� DXObj..
	 *
	 * �۵� ����� Interface�� �̾ƾ��Ѵ�.
	 * �⺻������ Device, DC. ���� ���ҽ��� ������ �־�� �Ѵ�.(�ش� ������ �� �������̽��� ��ӹ��� ������Ʈ�鿡 �־��ش�)
	 * 2022.06.21(ȭ) Dev.BlackFoot
	 */
	class GRAPHICSENGINE_DECLSPEC IDXObject
	{
	public:
		IDXObject() = default;
		virtual ~IDXObject() = default;

		// �ʱ�ȭ. Device�� DC�� �޾Ƽ�? �����غ��� �ش� ������.. ���ӿ��������� �� �ʿ䰡 ����������..
		// ����Ʈ �����ͷ� �ٲ��൵ ������ ��. 
		virtual bool Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) abstract;
		// ���� ����� �޾Ƽ� ������Ʈ
		virtual void Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj) abstract;
		// �� ������Ʈ
		virtual void LightUpdate(const DirectionalLight& _dirLit, const PointLight& _pointLit, const SpotLight& _spotLit, const XMFLOAT3& _eyePosW) abstract;

		// ������ ������Ʈ�� ��� ������ �� ����̽��� �����͸� ������ �ִ� ���� ������..
		virtual void Render() abstract;

		// �ļ� �κ��� �������� ��..��.. �� ��� ����..
			// �������� ���� ������Ʈ�� Device�� �־��ش�.
		// ������Ʈ�� ���� �־��ֱ�
		virtual void SetObjectResources(std::shared_ptr<ObjectResources> objResources) abstract;

		// ������Ʈ�� �з��� �� �ִ� �̸��� �־�����..
		virtual std::string GetObjName() abstract;
		virtual std::shared_ptr<ObjectResources> GetObjectResources() abstract;
	};

}
