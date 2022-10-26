#pragma once

using namespace DirectX;

namespace GraphicsEngineSpace
{
	// 각종 버텍스 구조체 정보.
	namespace VertexStruct
	{
		// 가장 기본적인 색과 위치 정보를 가지고 있는 버텍스 구조체
		struct ColorVertex
		{
			XMFLOAT3 Pos;
			XMFLOAT4 Color;
		};

		// 위치와 텍스쳐 좌표 정보를 가지고 있는 TexVertex
		struct TexVertex
		{
			XMFLOAT3 Pos;
			XMFLOAT2 Tex;
		};

		// 빛 만 사용하는 LightVertex
		struct LightVertex
		{
			XMFLOAT3 Pos;
			XMFLOAT3 Normal;
		};

		// 빛과 텍스쳐 정보를 포함하는 Basic32 구조체.
		struct Basic32
		{
			XMFLOAT3 Pos;
			XMFLOAT3 Normal;
			XMFLOAT2 Tex;
		};

		// 노말 맵 정보(탄젠트)를 포함하는 normalVertex
		struct NormalVertex
		{
			XMFLOAT3 Pos;
			XMFLOAT3 Normal;
			XMFLOAT3 TangentU;
			XMFLOAT2 Tex;
		};

		// 스키닝 정보를 담는 버텍스 스트럭쳐.
		struct SkinnedVertex
		{
			XMFLOAT3 Pos;
			XMFLOAT3 Normal;
			XMFLOAT2 Tex;
			XMFLOAT3 Weights;		// 본 하나 하나의 가중치 값을 담는다. 마지막은 쉐이더 코드에서 1 - 로 구한다.
			XMUINT4 BoneIndices;		// 각각의 가중치가 들어간 bone의 인덱스 정보.
		};
	}

}