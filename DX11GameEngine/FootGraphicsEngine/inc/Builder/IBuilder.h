#pragma once
#include "Resources/ObjectResources.h"

namespace GraphicsEngineSpace
{

	class IDXObject;

	/**
	* \brief 빌더의 동작을 다형적으로 정의한 IBuilder 클래스.
	*/
	class IBuilder
	{
	public:
		virtual ~IBuilder() {}

		// 생성된 오브젝트를 받아서, 오브젝트를 세팅해주는 함수를 만든다
		virtual std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName) abstract;

		// 텍스쳐 파일까지 받는 버전
			// 이 방식에는 문제가 많아서 (함수가 무한정으로 늘어날 수 있다..)
			// 이후에 파서와 파싱 데이터 영역을 분리할 수 있으면, 파싱 데이터 영역의 Mat정보를 이용해서
			// 디퓨즈 맵, 노말 맵 등을 추가한다..
		virtual std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, std::wstring _texFile) abstract;
		virtual std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, std::wstring diffuseMap, std::wstring normalMap) abstract;

		// 텍스쳐를 바로 받는 버전
			// 반드시 구현할 필요는 없다.
		virtual std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, ID3D11ShaderResourceView* shaderResource)
		{
			return nullptr;
		}

		// 빌더들의 내부에 존재하는 멤버 변수를 초기화 해주는 Init 함수
			// 과거 팩토리에 있던 Init을 가져온다고 생각.
		virtual void InitBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pDC) abstract;

	protected:
		// 외부에 드러낼 필요가 없어보이는 함수.
		// 실제로는 오브젝트가 아니라 빌더 내부에서 지오메트리 정보들을 추가해서 오브젝트에 넣어준다.
			// 오브젝트는 관련 정보를 전달해주는 컨테이너(BuildInfo)를 가지고 있고
			// 해당 컨테이너의 내용을 빌더에서 채워서 반환 한 뒤.
			// 오브젝트에 세팅해준다.
			// 소연 누나의 구조.
		virtual std::shared_ptr<ObjectResources> BuildGeometry(std::string objectName) abstract;
	};

}