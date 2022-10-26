#pragma once
#include "Object/IDXObject.h"
#include "Resources/ObjectResources.h"

namespace GraphicsEngineSpace
{
	/**
	 * \brief �⺻������ Basic32 ���̴��� �������� ��������� Basic32 Obj Ŭ����
	 *
	 * �ϴ��� �ϵ� �ڵ����� ��������� Obj
	 * + ���Ŀ� ASE �ļ��� ���� ��������� ������Ʈ�� �� Node�� ���� Obj���� ��ǥ�ϴ� Ŭ����. => �� ������ MeshObj�� �ش��ϴ� ������ �߰��ص� �� �� ����.
	 *
	 * 2022.07.11(��) Dev.BlackFoot
	 */
	class LegacyObj : public IDXObject
	{
		// ��� ���� ���
		// ����̽��� ������ ����.
		ID3D11Device* D3DDevice;
		ID3D11DeviceContext* D3DDeviceContext;

		// ���� ������ ���Ե� ���ҽ�
		std::shared_ptr<ObjectResources> objectResources;

		///------------- ������� ������ prefab ���������� ConstantBufferSetting���� ResourceManager���� �����ǰ� �־���
		// �ؽ��� ���ҽ���
		ID3D11ShaderResourceView* diffuseMapSRV;
		ID3D11ShaderResourceView* normalMapSRV;

		// ������ ���� ������
			// ���� ������ �������� �־��ְ�? ���������� �����´�.
		Material mat;

		// Ŭ������ ������ �ִ� ����ü
			// ���ο��� �޾ƿ´�.
		DirectionalLight dirLight;
		PointLight pointLight;
		SpotLight spotLight;
		XMFLOAT3 eyePosW;	// ���̴������� ViewPosition;

		// TM ��� => ������ transform ����ü�� ���� ���� ���� �� ����.
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX proj;
		///------------- �� ��������.


		// ���̴��� ������ ��Ʈ����
		struct cbMatrix
		{
			SimpleMath::Matrix worldViewProj;
			SimpleMath::Matrix worldTransform;
			SimpleMath::Matrix worldInvTranspose;
		};



	public:
		// �ܺο��� �����ϱ� ������.. �����ڴ� dll export�� �����ش�..!
		GRAPHICSENGINE_DECLSPEC LegacyObj();
		~LegacyObj() override;

		bool Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) override;
		void Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj) override;
		void LightUpdate(const DirectionalLight& _dirLit, const PointLight& _pointLit, const SpotLight& _spotLit, const XMFLOAT3& _eyePosW) override;
		void Render() override;
		void SetObjectResources(std::shared_ptr<ObjectResources> objResources) override;
		std::string GetObjName() override;

	private:
		// Render���� ����
			// TODO : �� �κ� ���߿� hlsl �ڵ�� ���� �ؾ���
		//void SetFXVariable();

		// �ؽ��� ����
		void InitTexture();
		void CreateSRV(std::wstring path, ID3D11ShaderResourceView** srv);
	};

}