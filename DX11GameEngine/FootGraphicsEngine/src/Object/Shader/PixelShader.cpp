#include "GraphicsPch.h"
#include "Object/Shader/PixelShader.h"

namespace GraphicsEngineSpace
{
	PixelShader::PixelShader(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, ID3DBlob* shaderBlob)
		: ShaderBase(ShaderType::PIXEL)
		, deviceContext(_deviceContext)
		, shader(nullptr)
	{
		if(CreateShader(_device, shaderBlob) != true)
		{
			assert(0);
		}
	}

	PixelShader::~PixelShader()
	{
		ReleaseCOM(shader);
	}

	bool PixelShader::SetUpShader()
	{
		deviceContext->PSSetShader(shader, nullptr, 0);

		return true;
	}

	Shader* PixelShader::GetShader()
	{
		return reinterpret_cast<Shader*>(shader);
	}

	bool PixelShader::CreateShader(ID3D11Device* _device, ID3DBlob* shaderBlob)
	{
		HRESULT hr = _device->CreatePixelShader
		(
			shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize(),
			nullptr,
			&shader
		);

		return SUCCEEDED(hr);
	}
}
