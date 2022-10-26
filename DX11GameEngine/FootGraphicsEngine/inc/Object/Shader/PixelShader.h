#pragma once

// 픽셀 쉐이더와 관련된 요소들이 모여있는 클래스
#include "ShaderBase.h"

// 버텍스 쉐이더와 관련된 요소들이 모여있는 클래스
namespace GraphicsEngineSpace
{
	class PixelShader : public ShaderBase
	{
		// 픽셀 셰이더 필요.
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