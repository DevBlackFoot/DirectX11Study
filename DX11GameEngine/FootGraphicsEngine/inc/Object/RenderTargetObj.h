#pragma once
#include "Object/IDXObject.h"
#include "Resources/ObjectResources.h"

namespace GraphicsEngineSpace
{
	/**
	 * \brief 텍스쳐로 변환한 렌더타겟을 화면에 그리기 위한 클래스입니다.
	 *
	 * 기본적으로 다른 오브젝트들과 같은 식으로 만들어줍니다
	 *
	 * 디버그 정보를 확인하는 역할을 합니다.
	 */
	class RenderTargetObj : public IDXObject
	{
	private:
		ID3D11Device* D3DDevice;
		ID3D11DeviceContext* D3DDeviceContext;

		// 각종 정보가 포함된 리소스
		std::shared_ptr<ObjectResources> objectResources;

		// 기본적인 TM행렬
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX proj;

		// 안에 들어가는 cb.
		struct cbMatrix
		{
			SimpleMath::Matrix worldViewProj;
			SimpleMath::Matrix world;
			SimpleMath::Matrix worldInvTranspose;
		};

	public:
		// 생성자만 Export
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