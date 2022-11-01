#pragma once
#include "GraphicsCore/DX11GraphicsCore.h"

namespace GraphicsEngineSpace
{
	class Mesh
	{
	private:
		uint64 resourceID = 0;

		ComPtr<ID3D11Buffer> vertexBuffer;

		ComPtr<ID3D11Buffer> indexBuffer;

		unsigned int stride;
	
		unsigned int indexBufferSize;

		ComPtr<ID3D11RasterizerState> rasterState;

		D3D11_PRIMITIVE_TOPOLOGY topology;

	public:
		Mesh()
			: resourceID(0)
			, stride(0)
			, indexBufferSize(0)
		{};
		~Mesh() {};

	public:
		template <typename T>
		void CreateVertexBuffer(const std::vector<T>& vertices);

		inline void CreateIndexBuffer(const std::vector<unsigned int>& indexBuffer);

		const uint64& GetResourceID() { return resourceID; }

		void SetResourceID(uint64 _resourceID) { resourceID = _resourceID; }

		unsigned int& GetIdxBufferSize() { return indexBufferSize; }

		const ComPtr<ID3D11Buffer>& GetVertexBuffer() { return vertexBuffer; }

		const ComPtr<ID3D11Buffer>& GetIndexBuffer() { return indexBuffer; }

		const ComPtr<ID3D11RasterizerState>& GetRasterState() { return rasterState; }

		D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() { return topology; }

		void SetStride(unsigned int stride) { this->stride = stride; }

		void SetRenderState(ComPtr<ID3D11RasterizerState> rasterState) { this->rasterState = rasterState; }

		void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology) { this->topology = topology; }

		void SetIndexBufferSize(unsigned int indexBufferSize) { this->indexBufferSize = indexBufferSize; }
	};

	template<typename T>
	inline void Mesh::CreateVertexBuffer(const std::vector<T>& vertices)
	{
		D3D11_BUFFER_DESC vertexBufferDesc;
		vertexBufferDesc.ByteWidth = sizeof(T) * vertices.size();
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexInitData;
		vertexInitData.pSysMem = vertices.data();

		GraphicsEngineSpace::DX11GraphicsCore::GetInstance()->GetDevice()->
			CreateBuffer(&vertexBufferDesc, &vertexInitData, vertexBuffer.GetAddressOf());
	}

	inline void GraphicsEngineSpace::Mesh::CreateIndexBuffer(const std::vector<unsigned int>& indices)
	{
		D3D11_BUFFER_DESC indexBufferDesc;
		indexBufferDesc.ByteWidth = sizeof(unsigned int) * indices.size();
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexInitData;
		indexInitData.pSysMem = indices.data();

		indexBufferSize = indices.size();

		GraphicsEngineSpace::DX11GraphicsCore::GetInstance()->GetDevice()->
			CreateBuffer(&indexBufferDesc, &indexInitData, indexBuffer.GetAddressOf());
	}
}
