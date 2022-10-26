#pragma once

namespace GraphicsEngineSpace
{
	/// �ҿ� ������ InputLayoutŬ������ �����ؼ� ������ inputlayout Ŭ����.
	/// ���������� ���ؽ� ����ü�� �ٸ� ������Ʈ���� inputlayout�� ������ ������,
	///	�ʿ��� �� �����ϴ� ������ �Ѵ�. => Effect Ŭ������ ����� ����..
	///
	///	�⺻������ �� ������ object�� �پ��ִ� BuildVertexLayout�Լ� �κ��� Ŭ������ �̾��ذ�.
	///	2022.07.06(��) Dev.BlackFoot

	/**
	 * \brief ���ؽ� ����ü�� �������� ���� ��ǲ���̾ƿ� Desc�� ������ش�.
	 */
	class InputLayoutDesc
	{
	public:
		// ���� �⺻���� InputLayout ��ġ�� �÷� ����.
		static D3D11_INPUT_ELEMENT_DESC PosColor[2];

		// ���� Ÿ�� ���� �� UI ��� ���� ��ġ�� Tex����
		static D3D11_INPUT_ELEMENT_DESC TextureRect[2];

		// �븻 �� ������ �����ϴ� ���̾ƿ�, ��ġ, �븻, �ؽ���, ź��Ʈ
		static D3D11_INPUT_ELEMENT_DESC LegacyModel[4];

		// ��Ű�� ������ ��� ���̾ƿ� ��ġ, �븻, �ؽ���, ����ġ, �� index
		static D3D11_INPUT_ELEMENT_DESC SkinnedLayout[5];
	};

	/**
	 * \brief �ݺ��Ǵ� ��ǲ ���̾ƿ� ������ �ѹ��� ���ֱ� ���� InputLayout Ŭ����
	 *
	 * �⺻������ �����¿� �´� ��ǲ���̾ƿ��� �������ش�.
	 */
	class InputLayout
	{
	public:
		// Desc�� Inputlayout���� ������ �ִ� ������
		static ID3D11InputLayout* PosColor;
		static ID3D11InputLayout* LegacyModel;
		static ID3D11InputLayout* SkinnedLayout;
	public:
		// ���� Desc���� init�ϰ�. InputLayout ���·� ������ �� �ִ� �Ŵ����� Ŭ����
			// ��� init.
		//static void InitAll(ID3D11Device* pDevice);
		static void InitInputLayout(ID3D11Device* pDevice, ID3DBlob* shaderBlob);
		static void DestoryAll();

	};

}