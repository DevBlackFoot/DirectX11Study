#pragma once

namespace GraphicsEngineSpace
{

	/**
	* \brief ���� ���� �� �� ���� RaterizerState�� �̾Ƶ� Ŭ����.
	*
	* ������ Effectó�� RS���� ���⼭ �̾��ش�.
	*
	* 2022.07.12(ȭ) Dev.BlackFoot
	*/
	class RasterizerState
	{
	private:
		static ID3D11RasterizerState* wireFrameRS;
		static ID3D11RasterizerState* solidRS;

	public:
		static bool InitAllRS(ID3D11Device* pDevice);

		static ID3D11RasterizerState* GetWireFrameRS() { return wireFrameRS; }
		static ID3D11RasterizerState* GetSolidRS() { return solidRS; }
		static void DestroyAll();
	};

}
