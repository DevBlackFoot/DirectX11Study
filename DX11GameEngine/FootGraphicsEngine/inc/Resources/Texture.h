#pragma once

namespace GraphicsEngineSpace
{
	class Texture
	{
	private:
		uint64 resourceID;

		std::string textureName;

		ComPtr<ID3D11ShaderResourceView> mapSRV;

		ScratchImage image;

		ComPtr<ID3D11Texture2D> texResource;

		DXGI_FORMAT format;

		unsigned int levels;

		unsigned int width;

		unsigned int height;

	public:
		Texture()
			: mapSRV(nullptr)
			, image(ScratchImage())
			, texResource(nullptr)
			, resourceID(0)
			, textureName("None")
		{};

		~Texture() {};

	public:
		const uint64& GetResourceID() { return resourceID; }

		void SetResourceID(uint64 _resourceID) { resourceID = _resourceID; }

		ComPtr<ID3D11ShaderResourceView> GetMapSRV() { return mapSRV; }

		void Initialize(const std::string& _textureName, const std::wstring& path);

		void Release();
	};
}