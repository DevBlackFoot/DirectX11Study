// LegacyModel�� pixel Shader �Դϴ�.

// ���Ŀ� �ʿ��ϴٸ� �߰��ϰ����� �� ���̴� �ڵ�� �⺻������ ���� ������ �����մϴ�
#include "LegacyModelTypes.hlsli"

// ���⼭ �ȼ� ���꿡 �ʿ��� ���� ������ �޾ƿɴϴ�
	// TexTransform..�̳�(�����غ��� �̰� Ư���� ��� �ƴϸ� �⺻������ Identity�� �ȹ����� ������..)

// light ��, material.. ���

// �ؽ��� ��
Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t1);

// ���÷�
	// �ܺο��� ���� �־��ִ� ����.
SamplerState Sampler : register(s0);

// View..
// �� ������ ���� cbView
cbuffer cbView : register(b0)
{
    float3 ViewPosition;
}

// ���� lightHelper.fx �� �ִ� ����ü��
// directLight
cbuffer cbDirectionalLight : register(b1)
{
    float3 DLightDir;
    float4 DLightDiffuse;
    float4 DLightAmbient;
    float DLightSpecular;
}

// point Light
cbuffer cbPointLight : register(b2)
{
    float4 PLightPos;
    float4 PLightDiffuse;
    float PLightSpecular;
    float PLightIntensity;
}

// Material
cbuffer cbMaterial : register(b3)
{
    float4 MaterialAmbient;
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
}

// outPut
	// �����ؼ� ���� �۾��� �ϴ� ���ϱ�.. ref by ������ ��
	// ������ MRT�� ���ؼ� �ȼ����̴� ������ ���� �̾��ִ� ����.
struct PS_Output
{
    // ��ũ���� ������ ��ġ..?
    float4 Screen : SV_TARGET0;
    // depth
    float4 Depth : SV_TARGET1;
    // normal
    float4 Normal : SV_TARGET2;
    // albedo => Texture��
    float4 Albedo : SV_TARGET3;
    // ����
    float4 WorldPos : SV_TARGET4;
};

// �ȼ� ���̴�
PS_Output main(VS_Output pin) : SV_Target
{
    float3 toEye = ViewPosition - pin.PosW;

    // ���� �� �븻������..
    float distToEye = length(toEye);
    toEye /= distToEye;

    // �⺻ �ؽ��� ������ ������ �׵��
    float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

    // ������ ����Ѵٰ� ����
    texColor = DiffuseMap.Sample(Sampler, pin.Tex);
    
    // ��ָ�
    float3 bumpedNormalW;
    float3 normalMapSample = NormalMap.Sample(Sampler, pin.Tex).rgb;

    // lighthelper�� �ִ� �� ��������.
    // �� ������ -1, 1�� ���
    float3 normalT = 2.0f * normalMapSample - 1.0f;

    // ���� ���� ����
    float3 N = pin.NormalW;
    float3 T = normalize(pin.TangentW - dot(pin.TangentW, N) * N);
    float3 B = cross(N, T);

    float3x3 TBN = float3x3(T, B, N);

    bumpedNormalW = mul(normalT, TBN);

    // �� ����
    // �ϴ��� TexColor�� �޾Ƽ� ���� ����
    float4 litColor = texColor;

    // ���� �Ӽ�
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // �븻���� ����Ѵ� ����
    // Dirlight
    float3 lightVec = -DLightDir;

    // �ֺ������� ���Ѵ�.
    ambient += MaterialAmbient * DLightAmbient;

    // Ȯ�걤 ���
		// �븻���� ���� ���� ���.
    float diffuseFactor = dot(lightVec, bumpedNormalW);

    // Ȯ�걤�� ��������..
    [flatten]
    if(diffuseFactor > 0.0f)
    {
	    // specular
        float3 v = reflect(-lightVec, bumpedNormalW);
        float specFactor = pow(max(dot(v, toEye), 0.0f), MaterialSpecular.w);

        diffuse += diffuseFactor * MaterialDiffuse * DLightDiffuse;
        specular += specFactor * MaterialSpecular * DLightSpecular;
    }

    // PointLight
    lightVec = PLightPos.xyz - pin.PosW;

    // ǥ�鿡���� ���� ���� ������ �Ÿ�
    float d = length(lightVec);
    lightVec = normalize(lightVec);

    // ���̰� ���� ���� ���� ����.
    [flatten]
    if (d < PLightIntensity)
    {
        lightVec /= d;

        // �����Ʈ => ���ǰ� �״�� ���ٰ� ����..
        // ���� ������ �ʰ� ǥ�鿡 �����Ѵٰ� �����ϰ�..
        diffuseFactor = dot(lightVec, bumpedNormalW);


        [flatten]
        if(diffuseFactor > 0.0f)
        {
			float intensity = 1.0f - (d / PLightIntensity);
            float3 v = reflect(-lightVec, bumpedNormalW);
            float specFactor = pow(max(dot(v, toEye), 0.0f), MaterialSpecular.w);

            diffuse += diffuseFactor * MaterialDiffuse * PLightDiffuse * intensity;
            specular += specFactor * MaterialSpecular * PLightSpecular * intensity;
        }

    }

    // ������ ����
    // ���� Correction
    litColor = pow(litColor, 2.2f);
    litColor = texColor * (ambient + diffuse) + specular;
    litColor = pow(litColor, 1 / 2.2f);

    litColor.a = MaterialDiffuse.a * texColor.a;

    PS_Output output = (PS_Output)0;

    output.Screen = litColor;
    output.Depth = pin.PosH.z;
    output.Normal = float4(bumpedNormalW, 1.0f);
    output.Albedo = DiffuseMap.Sample(Sampler, pin.Tex);
    output.WorldPos = float4(pin.PosW, 1.0f);

    return output;
}