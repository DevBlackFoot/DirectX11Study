#pragma once
#include "Resources/ObjectResources.h"

/// Dll 매크로
#ifdef FOOTGRAPHICSENGINE_EXPORTS
#define GRAPHICSENGINE_DECLSPEC __declspec(dllexport)
#else
#define GRAPHICSENGINE_DECLSPEC __declspec(dllimport)
#endif

namespace GraphicsEngineSpace
{
	/**
	 * \brief Factory에서 생성하는 모든 오브젝트의 베이스가 되는 DXObj..
	 *
	 * 작동 방식을 Interface로 뽑아야한다.
	 * 기본적으로 Device, DC. 각종 리소스를 가지고 있어야 한다.(해당 내용은 이 인터페이스를 상속받은 오브젝트들에 넣어준다)
	 * 2022.06.21(화) Dev.BlackFoot
	 */
	class GRAPHICSENGINE_DECLSPEC IDXObject
	{
	public:
		IDXObject() = default;
		virtual ~IDXObject() = default;

		// 초기화. Device와 DC를 받아서? 생각해보면 해당 내용은.. 게임엔진에서는 알 필요가 없긴하지만..
		// 스마트 포인터로 바꿔줘도 무방할 듯. 
		virtual bool Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) abstract;
		// 각종 행렬을 받아서 업데이트
		virtual void Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj) abstract;
		// 빛 업데이트
		virtual void LightUpdate(const DirectionalLight& _dirLit, const PointLight& _pointLit, const SpotLight& _spotLit, const XMFLOAT3& _eyePosW) abstract;

		// 내부의 오브젝트가 멤버 변수로 각 디바이스의 포인터를 가지고 있는 것이 나을까..
		virtual void Render() abstract;

		// 파서 부분을 가져오는 것..은.. 또 어떻게 하지..
			// 빌더에서 만들어서 오브젝트를 Device에 넣어준다.
		// 오브젝트에 정보 넣어주기
		virtual void SetObjectResources(std::shared_ptr<ObjectResources> objResources) abstract;

		// 오브젝트를 분류할 수 있는 이름을 넣어주자..
		virtual std::string GetObjName() abstract;
		virtual std::shared_ptr<ObjectResources> GetObjectResources() abstract;
	};

}
