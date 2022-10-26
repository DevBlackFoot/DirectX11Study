#pragma once
#include "Object/IDXObject.h"
#include "Resources/ObjectResources.h"

namespace GraphicsEngineSpace
{
	/**
	 * \brief �ؽ��ķ� ��ȯ�� ����Ÿ���� ȭ�鿡 �׸��� ���� Ŭ�����Դϴ�.
	 *
	 * �⺻������ �ٸ� ������Ʈ��� ���� ������ ������ݴϴ�
	 *
	 * ����� ������ Ȯ���ϴ� ������ �մϴ�.
	 */
	class RenderTargetObj : public IDXObject
	{
	private:
		ID3D11Device* D3DDevice;
		ID3D11DeviceContext* D3DDeviceContext;

		// ���� ������ ���Ե� ���ҽ�
		std::shared_ptr<ObjectResources> objectResources;

		// �⺻���� TM���
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX proj;

		// �ȿ� ���� cb.
		struct cbMatrix
		{
			SimpleMath::Matrix worldViewProj;
			SimpleMath::Matrix world;
			SimpleMath::Matrix worldInvTranspose;
		};

	public:
		// �����ڸ� Export
		GRAPHICSENGINE_DECLSPEC RenderTargetObj();
		~RenderTargetObj() override;

		bool Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) override;
		void Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj) override;
		void LightUpdate(const DirectionalLight& _dirLit, const PointLight& _pointLit, const SpotLight& _spotLit, const XMFLOAT3& _eyePosW) override;
		void Render() override;
		void SetObjectResources(std::shared_ptr<ObjectResources> objResources) override;
		std::string GetObjName() override;

	};
}