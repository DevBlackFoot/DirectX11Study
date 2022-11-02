#pragma once
#include "Object/IDXObject.h"
#include "Resources/ObjectResources.h"

namespace GraphicsEngineSpace
{
	class ResourceManager;

	/**
	* \brief Grid, Axis등, 위치와 컬러 값만 가지는 기본적인 Pos ColorObj에 관한 오브젝트 클래스
	*
	* 2022.07.13(수) Dev.BlackFoot
	*/
	class LineObj : public IDXObject
	{
		// 멤버 변수 목록
		// 디바이스를 가지고 있자.
		ID3D11Device* D3DDevice;
		ID3D11DeviceContext* D3DDeviceContext;

		// 각종 정보가 포함된 리소스
		std::shared_ptr<ObjectResources> objectResources;

		// 캐싱용 리소스 매니저 변수
		std::shared_ptr<ResourceManager> resourceManager;

		// TM 행렬 => 트랜스 폼..?
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX proj;

		// 안에 들어가는 cb
		struct cbMatrix
		{
			SimpleMath::Matrix worldViewProj;
		};

	public:
		// 생성자는 Export
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
