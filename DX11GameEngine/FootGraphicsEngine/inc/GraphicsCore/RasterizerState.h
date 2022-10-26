#pragma once

namespace GraphicsEngineSpace
{

	/**
	* \brief 자주 변경 될 것 같은 RaterizerState를 뽑아둔 클래스.
	*
	* 앞으로 Effect처럼 RS역시 여기서 뽑아준다.
	*
	* 2022.07.12(화) Dev.BlackFoot
	*/
	class RasterizerState
	{
	private:
		static ID3D11RasterizerState* m_WireFrameRS;
		static ID3D11RasterizerState* m_SolidRS;

	public:
		static bool InitAllRS(ID3D11Device* pDevice);

		static ID3D11RasterizerState* GetWireFrameRS() { return m_WireFrameRS; }
		static ID3D11RasterizerState* GetSolidRS() { return m_SolidRS; }
		static void DestroyAll();
	};

}
