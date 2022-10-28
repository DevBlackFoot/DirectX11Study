#include "GraphicsPch.h"
#include "Manager/SamplerManager.h"

namespace GraphicsEngineSpace
{
	std::shared_ptr<SamplerManager> SamplerManager::instance = nullptr;


	void SamplerManager::Init(ID3D11Device* device)
	{
		// 모든 샘플러 스테이트 초기화
		D3D11_SAMPLER_DESC samDesc;

		samDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		samDesc.MipLODBias = 0.f;
		samDesc.MaxAnisotropy = 2;
		samDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samDesc.BorderColor[0] = 0.f;
		samDesc.BorderColor[1] = 0.f;
		samDesc.BorderColor[2] = 0.f;
		samDesc.BorderColor[3] = 0.f;

		samDesc.MinLOD = -FLT_MAX;
		samDesc.MaxLOD = FLT_MAX;

		HRESULT hr = device->CreateSamplerState(&samDesc, &wrapSamplerState);

		if (FAILED(hr) == true)
			::MessageBoxA(nullptr, "Sampler Create Failed!", nullptr, MB_OK);

		D3D11_SAMPLER_DESC samDesc2;

		samDesc2.Filter = D3D11_FILTER_ANISOTROPIC;
		samDesc2.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samDesc2.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samDesc2.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

		samDesc2.MipLODBias = 0.f;
		samDesc2.MaxAnisotropy = 2;
		samDesc2.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samDesc2.BorderColor[0] = 0.f;
		samDesc2.BorderColor[1] = 0.f;
		samDesc2.BorderColor[2] = 0.f;
		samDesc2.BorderColor[3] = 0.f;

		samDesc2.MinLOD = -FLT_MAX;
		samDesc2.MaxLOD = FLT_MAX;

		hr = device->CreateSamplerState(&samDesc2, &clampSamplerState);

		if (FAILED(hr) == true)
			::MessageBoxA(nullptr, "Sampler Create Failed!", nullptr, MB_OK);
	}

	void SamplerManager::Release()
	{
		ReleaseCOM(wrapSamplerState);
		ReleaseCOM(clampSamplerState);

		SafeReset(instance);
	}

	std::shared_ptr<SamplerManager> SamplerManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<SamplerManager>();

		return instance;
	}
}
