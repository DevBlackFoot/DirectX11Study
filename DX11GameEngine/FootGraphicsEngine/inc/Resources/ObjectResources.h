#pragma once
#include "Utils/d3d11Utils.h"

namespace GraphicsEngineSpace
{
	/**
	* \brief ������Ʈ�� ������ �ִ� ���� ���ҽ���
	*
	* // �ش� ������ ������ ������ ����, ������Ʈ�� ������ ����. �������� �����ؼ� ������Ʈ�� �־��ش�.
	*
	* 2022.07.11(��) Dev.BlackFoot
	*/
	struct ObjectResources
	{
		// ���ؽ��� �ε����� �����°� ī��Ʈ
		int VertexOffset;
		UINT IndexOffset;
		UINT IndexCount;

		// ���ؽ� ����, �ε��� ����
		ID3D11Buffer* VB;
		ID3D11Buffer* IB;

		// �����Ͷ����� ������Ʈ
		ID3D11RasterizerState* RasterizerState;

		// ��������
		D3D11_PRIMITIVE_TOPOLOGY Topology;

		// �ؽ��ĸ� ����Ѵٸ�.. ���� �н�
		std::wstring DiffuseMapPathName;
		std::wstring NormalMapPathName;

		// ������Ʈ �̸�
		std::string ObjName;

		// ���� Ÿ�ٿ��� ������� Shader Resource View
		ID3D11ShaderResourceView* RenderTargetTexture;

		ObjectResources()
			: VertexOffset(0)
			, IndexOffset(0)
			, IndexCount(0)
			, VB(nullptr)
			, IB(nullptr)
			, RasterizerState(nullptr)
			, Topology()
			, DiffuseMapPathName(L"NONE")
			, NormalMapPathName(L"NONE")
			, ObjName("NULL")
			, RenderTargetTexture(nullptr)
		{}

		~ObjectResources()
		{
			ReleaseCOM(VB)
			ReleaseCOM(IB)
		}
	};
}