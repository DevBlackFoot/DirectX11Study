#pragma once
#include "Builder/IBuilder.h"

class CASEParser;

namespace GraphicsEngineSpace
{
	class ResourceManager;

	/**
	 * \brief Basic32쉐이더를 사용하여 텍스쳐, 라이팅을 받는 오브젝트를 만드는 빌더입니다.
	 *
	 * 하드 코딩을 받았지만.. filePath를 받아서 ASE파싱을 할수도 있도록 만들고자 합니다.
	 * 2022.07.07(목) Dev.BlackFoot
	 */
	class LegacyStaticBuilder : public IBuilder
	{
		// 내부적으로 Device를 가지고 있어야 한다.
			// 멤버 함수가 여러개가 되면서 여러 부분에서 사용하는 각종 멤버 변수가 필요해졌다.
		ID3D11Device* D3DDevice;
		ID3D11DeviceContext* D3DDeviceContext;

		// 빌더가 ASE 파서를 가지고 있는다. (기본적으로 빌더는 한 번만 생성되기 때문이다.)
		CASEParser* meshASEParser;
		// 지오메트리 오브젝트가 한개라고 가정하고.. 특정 GeomObject를 읽어주는 index를 관리하자
			// 맵으로 오브젝트 번호를 관리하거나..
		int geomIdx;

		std::shared_ptr<ResourceManager> resourceManager;

	public:
		LegacyStaticBuilder()
			: D3DDevice(nullptr)
			, D3DDeviceContext(nullptr)
			, meshASEParser(nullptr)
			, geomIdx(-1)
			, resourceManager(nullptr)
		{}
		~LegacyStaticBuilder() override;

		virtual std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, uint64 objectID) override;
		virtual std::shared_ptr<ObjectResources> BuildGeometry(std::string objectName, uint64 objectID) override;
		virtual std::shared_ptr<IDXObject> AddTexture(std::shared_ptr<IDXObject> DXObj, uint64 textureID,
		                                              std::string textureName, std::wstring path, RenderingData::TextureMapType mapType) override;

		virtual void InitBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pDC) override;

	private:
		// 하드 코딩으로 넣어줄 오브젝트 만들기
			// Object가 buildInfo를 가지고 있어야하는 필요성
			// 빌더에서 BuildeInfo에 데이터를 넣어준 뒤, 해당 데이터를 실제 렌더 할 때 사용하기 때문에 반드시 필요하다.
			// 오브젝트 리소스를 받아서 내부에서 채워주고 반환해준다.

		// ASE에서 가져온 정적 Mesh의 Resources 정보를 추가해줍니다.
		std::shared_ptr<ObjectResources> BuildMeshASEResources(std::shared_ptr<ObjectResources> _objRes, uint64 objectID);
	};
}
