#pragma once

using namespace DirectX;

namespace GraphicsEngineSpace
{
	// ���� ����
	// �ش� ������ �� ���� �κ��� cb���۷� ����..
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

	// �� ���� ���� ����ü.
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

	// Ư�� ��ġ���� ������� ��
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

		// ������, Range���·� 4���� ���Ϳ� �ִ´�.
		XMFLOAT4 Position;
		XMFLOAT4 Diffuse;
		
		float Specular;
		// �������� �ȿ��� ���.
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

		// ������, Range���·� 4���� ���Ϳ� �ִ´�.
		XMFLOAT3 Position;
		float Range;

		// ����� spot ���·� 4���� ���Ϳ� �ִ´�.
		XMFLOAT3 Direction;
		float Spot;

		// Att, Pad. ���·� �ϳ��� 4���� ����.
		XMFLOAT3 Att;
		float Pad;
	};

}
