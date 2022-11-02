#pragma once

namespace GraphicsEngineSpace
{
	// ���÷� ������Ʈ�� �̸� ���� �־�� ���÷� ������Ʈ Ŭ����
	class SamplerManager
	{
	private:
		static std::shared_ptr<SamplerManager> instance;

		ID3D11SamplerState* wrapSamplerState;
		ID3D11SamplerState* clampSamplerState;
		ID3D11SamplerState* triLinearSamplerState;

	public:
		SamplerManager()
			: wrapSamplerState(nullptr)
			, clampSamplerState(nullptr)
			, triLinearSamplerState(nullptr)
		{
			
		}

		void Init(ID3D11Device* device);

		void Release();

		static std::shared_ptr<SamplerManager> GetInstance();

		ID3D11SamplerState* GetWrapSamplerState() { return wrapSamplerState; }
		ID3D11SamplerState* GetClampSamplerState() { return clampSamplerState; }
		ID3D11SamplerState* GetTriLinearSamplerState() { return triLinearSamplerState; }
	};

}