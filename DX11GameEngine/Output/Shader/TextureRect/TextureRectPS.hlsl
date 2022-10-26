// �⺻���� ��ġ�� ������ ������ �ִ� PosColor�� pixel Shader �Դϴ�.

// ���Ŀ� �ʿ��ϴٸ� �߰��ϰ����� �� ���̴� �ڵ�� �⺻������ ���� ������ �����մϴ�
// �� ��� ���۸� �޾ƾ���.
#include "TextureRectTypes.hlsli"

// �ؽ��� ��
Texture2D RenderTargetTexture : register(t0);

// ���÷�
SamplerState Sampler : register(s0);

// �ȼ� ���̴�
float4 main(VS_Output pin) : SV_Target
{
    float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

    texColor = RenderTargetTexture.Sample(Sampler, pin.Tex);

    return texColor;
}