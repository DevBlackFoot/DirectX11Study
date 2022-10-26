#pragma once

using namespace DirectX;

namespace GraphicsEngineSpace
{
	// 재질 정보
	// 해당 영역의 세 가지 부분이 cb버퍼로 들어간다..
	struct Material
	{
		Material()
			: Ambient()
			, Diffuse()
			, Specular()
		{
			ZeroMemory(this, sizeof(this));
		}

		XMFLOAT4 Ambient;
		XMFLOAT4 Diffuse;
		XMFLOAT4 Specular;
	};

	// 각 빛의 정보 구조체.
	struct DirectionalLight
	{
		DirectionalLight()
			: Direction()
			, Diffuse()
			, Ambient()
			, Specular(0.0f)
		{
			ZeroMemory(this, sizeof(this));
		}

		XMFLOAT3 Direction;
		float Specular;
		XMFLOAT4 Diffuse;
		XMFLOAT4 Ambient;
	};

	// 특정 위치에서 뻗어나가는 빛
	struct PointLight
	{
		PointLight()
			: Position()
			, Diffuse()
			, Specular(0.f)
			, Intensity(0.f)
		{
			ZeroMemory(this, sizeof(this));
		}

		// 포지션, Range형태로 4차원 벡터에 넣는다.
		XMFLOAT4 Position;
		XMFLOAT4 Diffuse;
		
		float Specular;
		// 감쇠율은 안에서 계산.
		float Intensity;
	};

	struct SpotLight
	{
		SpotLight()
			: Ambient()
			, Diffuse()
			, Specular()
			, Position()
			, Range(0.0f)
			, Direction()
			, Spot(0.0f)
			, Att()
			, Pad(0.0f)
		{
			ZeroMemory(this, sizeof(this));
		}

		XMFLOAT4 Ambient;
		XMFLOAT4 Diffuse;
		XMFLOAT4 Specular;

		// 포지션, Range형태로 4차원 벡터에 넣는다.
		XMFLOAT3 Position;
		float Range;

		// 방향과 spot 형태로 4차원 벡터에 넣는다.
		XMFLOAT3 Direction;
		float Spot;

		// Att, Pad. 형태로 하나의 4차원 벡터.
		XMFLOAT3 Att;
		float Pad;
	};

}
