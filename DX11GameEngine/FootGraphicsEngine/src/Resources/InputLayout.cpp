#include "GraphicsPch.h"
#include "Resources/InputLayout.h"

namespace GraphicsEngineSpace
{
	// ���� ����ƽ ���� �ʱ�ȭ
	// Inputlayout Desc ����

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
	// ����ƽ���� �ʱ�ȭ
	ID3D11InputLayout* InputLayout::PosColor = nullptr;
	ID3D11InputLayout* InputLayout::LegacyModel = nullptr;
	ID3D11InputLayout* InputLayout::SkinnedLayout = nullptr;

	/*void InputLayout::InitAll(ID3D11Device* pDevice)
	{
		// TODO : Effect ������ hlsl������ �������ش�.


		// ��Ȱ���� PassDesc ����
		//D3DX11_PASS_DESC passDesc;

		// ���̴� ����ʿ�. => �̰� ���̴� ����� �ִ� ������ ȣ���ϸ� ���� �� ����..
		//HRESULT hr = pDevice->CreateInputLayout(InputLayoutDesc::PosColor, 2);
		
		// �븻 �ʰ� ��Ű��.. ���� �߰�


	}*/

	// � ���̾ƿ��� �ʱ�ȭ�Ұ�����..
		// �̰� ��� ���� ���ؽ����̴� �������ΰ��̶� ���ؽ� ���̴� �ȿ� �־�ױ���..
	void InputLayout::InitInputLayout(ID3D11Device* pDevice, ID3DBlob* shaderBlob)
	{
		// Ư�� ���̴��� �������� Blob�� Device�� �޾Ƽ� ���̾ƿ� ����.

	}

	void InputLayout::DestoryAll()
	{
		ReleaseCOM(PosColor)
		ReleaseCOM(LegacyModel)
		ReleaseCOM(SkinnedLayout)
	}

}