#pragma once
#include "ShaderBase.h"

// ���ؽ� ���̴��� ���õ� ��ҵ��� ���ִ� Ŭ����
namespace GraphicsEngineSpace
{
	class VertexShader : public ShaderBase
	{
		// ���ؽ����� inputLayout �ʿ�.
		ID3D11DeviceContext* deviceContext;
		ID3D11VertexShader* shader;
		ID3D11InputLayout* layout;

	public:
		VertexShader(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, ID3DBlob* shaderBlob, D3D11_INPUT_ELEMENT_DESC* desc, UINT elementsSize);
		~VertexShader() override;

		bool SetUpShader() override;

		Shader* GetShader() override;

	private:
		// inputLayoutClass ���� ������ ����
		bool CreateLayout(ID3D11Device* _device, ID3DBlob* shaderBlob, D3D11_INPUT_ELEMENT_DESC* desc, UINT elementsSize);
		bool CreateShader(ID3D11Device* _device, ID3DBlob* shaderBlob);
	};
}