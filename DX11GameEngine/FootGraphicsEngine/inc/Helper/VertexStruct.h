#pragma once

using namespace DirectX;

namespace GraphicsEngineSpace
{
	// ���� ���ؽ� ����ü ����.
	namespace VertexStruct
	{
		// ���� �⺻���� ���� ��ġ ������ ������ �ִ� ���ؽ� ����ü
		struct ColorVertex
		{
			XMFLOAT3 Pos;
			XMFLOAT4 Color;
		};

		// ��ġ�� �ؽ��� ��ǥ ������ ������ �ִ� TexVertex
		struct TexVertex
		{
			XMFLOAT3 Pos;
			XMFLOAT2 Tex;
		};

		// �� �� ����ϴ� LightVertex
		struct LightVertex
		{
			XMFLOAT3 Pos;
			XMFLOAT3 Normal;
		};

		// ���� �ؽ��� ������ �����ϴ� Basic32 ����ü.
		struct Basic32
		{
			XMFLOAT3 Pos;
			XMFLOAT3 Normal;
			XMFLOAT2 Tex;
		};

		// �븻 �� ����(ź��Ʈ)�� �����ϴ� normalVertex
		struct NormalVertex
		{
			XMFLOAT3 Pos;
			XMFLOAT3 Normal;
			XMFLOAT3 TangentU;
			XMFLOAT2 Tex;
		};

		// ��Ű�� ������ ��� ���ؽ� ��Ʈ����.
		struct SkinnedVertex
		{
			XMFLOAT3 Pos;
			XMFLOAT3 Normal;
			XMFLOAT2 Tex;
			XMFLOAT3 Weights;		// �� �ϳ� �ϳ��� ����ġ ���� ��´�. �������� ���̴� �ڵ忡�� 1 - �� ���Ѵ�.
			XMUINT4 BoneIndices;		// ������ ����ġ�� �� bone�� �ε��� ����.
		};
	}

}