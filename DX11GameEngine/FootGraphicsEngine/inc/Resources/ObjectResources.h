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
		/*int VertexOffset;
		UINT IndexOffset;
		UINT IndexCount;

		// ���ؽ� ����, �ε��� ����
		ID3D11Buffer* VB;
		ID3D11Buffer* IB;

		// �����Ͷ����� ������Ʈ
		ID3D11RasterizerState* RasterizerState;

		// ��������
		D3D11_PRIMITIVE_TOPOLOGY Topology;*/

		// �ش� ���� Mesh�� ��ü.
			// ���⼭�� ID�� ������ �ֵ��� �Ѵ�.
		uint64 meshID;
		// ID �ߺ��� ��ó�ϱ� ���� Set���� bool ������ �־��ش�.
		bool setMesh;

		/*
		std::wstring DiffuseMapPathName;
		std::wstring NormalMapPathName;
		 */
		// ���� �ʵ��� ID�� ������ �ִ� ���׸����� ID�� ������ ����
		uint64 materialID;
		bool setMaterial;

		// ������Ʈ �̸�
		std::string ObjName;

		// ���� Ÿ�ٿ��� ������� Shader Resource View
		//ID3D11ShaderResourceView* RenderTargetTexture;

		ObjectResources()
			: meshID(0)
			, setMesh(false)
			, materialID(0)
			, setMaterial(false)
			, ObjName("NULL")
		{}

		~ObjectResources()
		{
			
		}
	};
}