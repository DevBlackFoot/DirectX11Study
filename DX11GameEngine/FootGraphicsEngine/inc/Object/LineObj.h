#pragma once
#include "Object/IDXObject.h"
#include "Resources/ObjectResources.h"

namespace GraphicsEngineSpace
{
	class ResourceManager;

	/**
	* \brief Grid, Axis��, ��ġ�� �÷� ���� ������ �⺻���� Pos ColorObj�� ���� ������Ʈ Ŭ����
	*
	* 2022.07.13(��) Dev.BlackFoot
	*/
	class LineObj : public IDXObject
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

		// �ȿ� ���� cb��.
			// ������ ����ϴ� ���� worldViewProj �ۿ� ��������..
		struct cbMatrix
		{
			SimpleMath::Matrix worldViewProj;
			SimpleMath::Matrix worldTransform;
			SimpleMath::Matrix worldInvTranspose;
		};

	public:
		// �����ڴ� Export
		GRAPHICSENGINE_DECLSPEC LineObj();
		~LineObj() override;

		bool Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) override;
		void Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj) override;
		void LightUpdate(const DirectionalLight& _dirLit, const PointLight& _pointLit, const SpotLight& _spotLit, const XMFLOAT3& _eyePosW) override;
		void Render() override;
		void SetObjectResources(std::shared_ptr<ObjectResources> objResources) override;
		std::string GetObjName() override;
		std::shared_ptr<ObjectResources> GetObjectResources() override;

	};
}
