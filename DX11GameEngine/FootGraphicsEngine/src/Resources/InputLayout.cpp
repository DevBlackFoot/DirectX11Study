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

	D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::LegacyStatic[4] =
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

	D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::LegacySkinned[5] =
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
	ID3D11InputLayout* InputLayout::LegacyStatic = nullptr;
	ID3D11InputLayout* InputLayout::LegacySkinned = nullptr;

	

	void InputLayout::DestoryAll()
	{
		ReleaseCOM(PosColor)
		ReleaseCOM(LegacyStatic)
		ReleaseCOM(LegacySkinned)
	}

}