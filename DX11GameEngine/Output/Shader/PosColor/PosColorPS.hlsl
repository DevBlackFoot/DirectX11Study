// �⺻���� ��ġ�� ������ ������ �ִ� PosColor�� pixel Shader �Դϴ�.

// ���Ŀ� �ʿ��ϴٸ� �߰��ϰ����� �� ���̴� �ڵ�� �⺻������ ���� ������ �����մϴ�
// �� ��� ���۸� �޾ƾ���.
#include "PosColorTypes.hlsli"

// �ȼ� ���̴�
float4 main(VS_Output pin) : SV_Target
{
    return pin.Color;
}