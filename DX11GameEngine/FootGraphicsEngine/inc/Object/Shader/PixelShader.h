#pragma once

// �ȼ� ���̴��� ���õ� ��ҵ��� ���ִ� Ŭ����
#include "ShaderBase.h"

// ���ؽ� ���̴��� ���õ� ��ҵ��� ���ִ� Ŭ����
namespace GraphicsEngineSpace
{
	class PixelShader : public ShaderBase
	{
		// �ȼ� ���̴� �ʿ�.
		ID3D11DeviceContext* deviceContext;
		ID3D11PixelShader* shader;
		
	public:
		PixelShader(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, ID3DBlob* shaderBlob);
		~PixelShader() override;

		bool SetUpShader() override;

		Shader* GetShader() override;

	private:
		bool CreateShader(ID3D11Device* _device, ID3DBlob* shaderBlob);
	};
}