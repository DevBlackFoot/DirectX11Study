#pragma once
#include "Object/IDXObject.h"
#include "Resources/ObjectResources.h"

namespace GraphicsEngineSpace
{
	class ResourceManager;

	/**
	 * \brief SkyBox ������Ʈ�Դϴ�.
	 *
	 * View Proj�� ���� �����̷��� ���� ������Ʈ�� ������ �ϱ� ������ ��������ϴ�.
	 */
	class SkyBox : public IDXObject
	{
		// ��� ���� ���
		// ����̽��� ������ ����.
		ID3D11Device* D3DDevice;
		ID3D11DeviceContext* D3DDeviceContext;

		// ���� ������ ���Ե� ���ҽ�
		std::shared_ptr<ObjectResources> objectResources;

		// ĳ�̿� ���ҽ� �Ŵ��� ����
		std::shared_ptr<ResourceManager> resourceManager;

		// TM ��� => Ʈ���� ��..?
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX proj;

		// �ȿ� ���� cb
		struct cbMatrix
		{
			SimpleMath::Matrix worldViewProj;
		};

	public:
		GRAPHICSENGINE_DECLSPEC SkyBox();
		~SkyBox() override;

		bool Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) override;
		void Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj) override;
		void LightUpdate(const DirectionalLight& _dirLit, const PointLight& _pointLit, const SpotLight& _spotLit, const XMFLOAT3& _eyePosW) override;
		void Render() override;
		void SetObjectResources(std::shared_ptr<ObjectResources> objResources) override;
		std::string GetObjName() override;
		std::shared_ptr<ObjectResources> GetObjectResources() override;
		
	};

}