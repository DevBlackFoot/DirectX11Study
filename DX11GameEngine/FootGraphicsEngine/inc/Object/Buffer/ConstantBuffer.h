#pragma once

#include "Object/Buffer/BufferBase.h"

namespace GraphicsEngineSpace
{
	using GraphicsEngineSpace::BufferBase;

	class BufferManager;

	class ConstantBuffer : public BufferBase
	{
		ID3D11DeviceContext* deviceContext;
		ID3D11Buffer* buffer;
		UINT bufferSize;

	public:
		ConstantBuffer(ID3D11DeviceContext* deviceContext);
		~ConstantBuffer() override;

		bool SetUpBuffer(unsigned slot, void* data, ShaderType targetType) override;

	private:
		bool CreateBuffer(ID3D11Device* device, UINT typeSize);

		friend BufferManager;
	};

}