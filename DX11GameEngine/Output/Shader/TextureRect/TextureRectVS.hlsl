// �⺻���� ��ġ�� ������ ������ �ִ� PosColor�� vertex Shader �Դϴ�.

// ���Ŀ� �ʿ��ϴٸ� �߰��ϰ����� �� ���̴� �ڵ�� �⺻������ ���� ������ �����մϴ�
// �� ��� ���۸� �޾ƾ���.
#include "TextureRectTypes.hlsli"

// ����� View�� ������ �ǹ̰�..
	// ����� ������Ʈ��ü�� ��ǥ
	// View�� proj�� ī�޶� ���� ����� �� ����..
// �ϴ� ������ �̷��� �صΰ�, ���߿� �ʿ信 ���� �����ϴ� ������ �Ѵ�.

// �⺻���� ��� ����
cbuffer cbPerObject : register( b0 )
{
    float4x4 WorldViewProj;
    float4x4 World;
    float4x4 WorldInvTranspose;
}


// ���ؽ� ���̴��� ������ ��
struct VS_Input
{
    float3 PosL : POSITION;
    float2 Tex : TEXCOORD;
};

// ���ؽ� ���̴�
VS_Output main(VS_Input vin)
{
    VS_Output vout = (VS_Output)0;
    vout.Pos = mul(float4(vin.PosL, 1.0f), WorldViewProj);
    vout.Tex = vin.Tex.xy;

    return vout;
}