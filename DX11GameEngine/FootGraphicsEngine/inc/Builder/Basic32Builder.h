#pragma once
#include "Builder/IBuilder.h"

class CASEParser;

namespace GraphicsEngineSpace
{
	/**
	 * \brief Basic32쉐이더를 사용하여 텍스쳐, 라이팅을 받는 오브젝트를 만드는 빌더입니다.
	 *
	 * 하드 코딩을 받았지만.. filePath를 받아서 ASE파싱을 할수도 있도록 만들고자 합니다.
	 * 2022.07.07(목) Dev.BlackFoot
	 */
	class Basic32Builder : public IBuilder
	{
		// 내부적으로 Device를 가지고 있어야 한다.
			// 멤버 함수가 여러개가 되면서 여러 부분에서 사용하는 각종 멤버 변수가 필요해졌다.
		ID3D11Device* m_D3DDevice;
		ID3D11DeviceContext* m_D3DDeviceContext;

		// 빌더가 ASE 파서를 가지고 있는다. (기본적으로 빌더는 한 번만 생성되기 때문이다.)
		CASEParser* m_MeshASEParser;
		// 지오메트리 오브젝트가 한개라고 가정하고.. 특정 GeomObject를 읽어주는 index를 관리하자
			// 맵으로 오브젝트 번호를 관리하거나..
		int m_GeomIdx;

	public:
		Basic32Builder()
			: m_D3DDevice(nullptr)
			, m_D3DDeviceContext(nullptr)
			, m_MeshASEParser(nullptr)
			, m_GeomIdx(-1)
		{}
		~Basic32Builder() override;

		// BuildDXObj는 여러개가 될 수 있지만
		virtual std::shared_ptr <IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName) override;
		virtual std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, std::wstring _texFile) override;
		virtual std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, std::wstring diffuseMap, std::wstring normalMap) override;

		// 내부적으로 하는 것이 같은 얘네는 사실상 하나로 합쳐도 될것 같은데..
		virtual std::shared_ptr<ObjectResources> BuildGeometry(std::string objectName) override;

		virtual void InitBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pDC) override;

	private:
		// 하드 코딩으로 넣어줄 오브젝트 만들기
			// Object가 buildInfo를 가지고 있어야하는 필요성
			// 빌더에서 BuildeInfo에 데이터를 넣어준 뒤, 해당 데이터를 실제 렌더 할 때 사용하기 때문에 반드시 필요하다.
			// 오브젝트 리소스를 받아서 내부에서 채워주고 반환해준다.
		//ObjectResources* BuildBoxResources(ObjectResources* _objRes);

		// ASE에서 가져온 정적 Mesh의 Resources 정보를 추가해줍니다.
		std::shared_ptr<ObjectResources> BuildMeshASEResources(std::shared_ptr<ObjectResources> _objRes);
	};
}
