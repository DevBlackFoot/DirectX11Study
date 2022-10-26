#include "GraphicsPch.h"
#include "Object/Shader/VertexShader.h"

namespace GraphicsEngineSpace
{
	VertexShader::VertexShader(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, ID3DBlob* shaderBlob, D3D11_INPUT_ELEMENT_DESC* desc, UINT elementsSize)
		: ShaderBase(ShaderType::VERTEX)
		, deviceContext(_deviceContext)
		, shader(nullptr)
		, layout(nullptr)
	{
		if (CreateShader(_device, shaderBlob) != true)
		{
			// 예외 처리
			assert(0);
		}

		if (CreateLayout(_device, shaderBlob, desc, elementsSize) != true)
		{
			assert(0);
		}

	}

	VertexShader::~VertexShader()
	{
		ReleaseCOM(shader);
		ReleaseCOM(layout);

	}

	bool VertexShader::SetUpShader()
	{
		deviceContext->VSSetShader(shader, nullptr, 0);
		// 여기에서 inputLayout까지 세팅해줍니다.
		deviceContext->IASetInputLayout(layout);

		return true;
	}

	Shader* VertexShader::GetShader()
	{
		return reinterpret_cast<Shader*>(shader);
	}

	bool VertexShader::CreateLayout(ID3D11Device* _device, ID3DBlob* shaderBlob, D3D11_INPUT_ELEMENT_DESC* desc, UINT elementsSize)
	{
		HRESULT hr = _device->CreateInputLayout
		(
			desc,
			elementsSize,
			shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize(),
			&layout
		);

		return SUCCEEDED(hr);
	}

	bool VertexShader::CreateShader(ID3D11Device* _device, ID3DBlob* shaderBlob)
	{
		HRESULT hr = _device->CreateVertexShader
		(
			shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize(),
			nullptr,
			&shader
		);

		return SUCCEEDED(hr);
	}
}
