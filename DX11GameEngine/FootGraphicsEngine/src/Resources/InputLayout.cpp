#include "GraphicsPch.h"
#include "Resources/InputLayout.h"

namespace GraphicsEngineSpace
{
	// 각종 스태틱 변수 초기화
	// Inputlayout Desc 정의

#pragma region InputLayoutDesc Init
	D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosColor[2] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::TextureRect[2] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::LegacyModel[4] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::SkinnedLayout[5] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"WEIGHTS", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT,
		0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
#pragma endregion

#pragma region InputLayout
	// 스태틱변수 초기화
	ID3D11InputLayout* InputLayout::PosColor = nullptr;
	ID3D11InputLayout* InputLayout::LegacyModel = nullptr;
	ID3D11InputLayout* InputLayout::SkinnedLayout = nullptr;

	/*void InputLayout::InitAll(ID3D11Device* pDevice)
	{
		// TODO : Effect 구조를 hlsl구조로 변경해준다.


		// 재활용할 PassDesc 변수
		//D3DX11_PASS_DESC passDesc;

		// 쉐이더 블롭필요. => 이걸 쉐이더 블롭이 있는 곳에서 호출하면 좋을 것 같다..
		//HRESULT hr = pDevice->CreateInputLayout(InputLayoutDesc::PosColor, 2);
		
		// 노말 맵과 스키닝.. 추후 추가


	}*/

	// 어떤 레이아웃을 초기화할것인지..
		// 이게 어떻게 보면 버텍스쉐이더 종속적인것이라 버텍스 쉐이더 안에 넣어뒀구나..
	void InputLayout::InitInputLayout(ID3D11Device* pDevice, ID3DBlob* shaderBlob)
	{
		// 특정 쉐이더를 컴파일한 Blob과 Device를 받아서 레이아웃 지정.

	}

	void InputLayout::DestoryAll()
	{
		ReleaseCOM(PosColor)
		ReleaseCOM(LegacyModel)
		ReleaseCOM(SkinnedLayout)
	}

}