#pragma once
#include "Utils/d3d11Utils.h"

namespace GraphicsEngineSpace
{
	/**
	* \brief 오브젝트가 가지고 있는 각종 리소스들
	*
	* // 해당 내용은 빌더의 종류에 따라, 오브젝트의 종류에 따라. 빌더에서 세팅해서 오브젝트에 넣어준다.
	*
	* 2022.07.11(월) Dev.BlackFoot
	*/
	struct ObjectResources
	{
		// 버텍스와 인덱스의 오프셋과 카운트
		/*int VertexOffset;
		UINT IndexOffset;
		UINT IndexCount;

		// 버텍스 버퍼, 인덱스 버퍼
		ID3D11Buffer* VB;
		ID3D11Buffer* IB;

		// 레스터라이즈 스테이트
		ID3D11RasterizerState* RasterizerState;

		// 토폴로지
		D3D11_PRIMITIVE_TOPOLOGY Topology;*/

		// 해당 내용 Mesh로 대체.
			// 여기서는 ID만 가지고 있도록 한다.
		uint64 meshID;
		// ID 중복을 대처하기 위해 Set관련 bool 변수를 넣어준다.
		bool setMesh;

		/*
		std::wstring DiffuseMapPathName;
		std::wstring NormalMapPathName;
		 */
		// 각종 맵들의 ID를 가지고 있는 머테리얼의 ID를 가지고 있자
		uint64 materialID;
		bool setMaterial;

		// 오브젝트 이름
		std::string ObjName;

		// 렌더 타겟에서 사용해줄 Shader Resource View
		//ID3D11ShaderResourceView* RenderTargetTexture;

		ObjectResources()
			: meshID(0)
			, setMesh(false)
			, materialID(0)
			, setMaterial(false)
			, ObjName("NULL")
		{}

		~ObjectResources()
		{
			
		}
	};
}