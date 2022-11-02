#include "GraphicsPch.h"
#include "GraphicsCore//RasterizerState.h"


namespace GraphicsEngineSpace
{
	// ����ƽ ���� �ʱ�ȭ
	ID3D11RasterizerState* RasterizerState::wireFrameRS = nullptr;
	ID3D11RasterizerState* RasterizerState::solidRS = nullptr;

	bool RasterizerState::InitAllRS(ID3D11Device* pDevice)
	{
		// ���� ������ �۾��̱⿡ ����� ������ ���� ����.
		D3D11_RASTERIZER_DESC rsDesc;
		ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
		rsDesc.FillMode = D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_NONE;
		rsDesc.FrontCounterClockwise = false;
		rsDesc.DepthClipEnable = true;

		HR(pDevice->CreateRasterizerState(&rsDesc, &wireFrameRS));

		rsDesc.FillMode = D3D11_FILL_SOLID;
		HR(pDevice->CreateRasterizerState(&rsDesc, &solidRS));

		return true;
	}

	void RasterizerState::DestroyAll()
	{
		ReleaseCOM(wireFrameRS)
		ReleaseCOM(solidRS)
	}
}
