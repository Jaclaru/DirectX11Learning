#include "Basic.hlsli"

float4 PS(BillboardVertex pIn) : SV_Target
{
    // ÿ4����һ��ѭ����������֤���ֲ�ͬ����
    float4 texColor = g_TexArray.Sample(g_Sam, float3(pIn.tex, pIn.PrimID % 4));
    // ��ǰ����Alpha�ü����Բ�����Ҫ������ؿ��Ա����������
    clip(texColor.a - 0.05f);

    // ��׼��������
    pIn.normalW = normalize(pIn.normalW);

    // �������ָ���۾����������Լ��������۾��ľ���
    float3 toEyeW = normalize(g_EyePosW - pIn.posW);
    float distToEye = distance(g_EyePosW, pIn.posW);

    // ��ʼ��Ϊ0 
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 A = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 D = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 S = float4(0.0f, 0.0f, 0.0f, 0.0f);

    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        ComputeDirectionalLight(g_Material, g_DirLight[i], pIn.normalW, toEyeW, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }

    float4 litColor = texColor * (ambient + diffuse) + spec;

    // ��Ч����
    [flatten]
    if (g_FogEnabled)
    {
        // �޶���0.0f��1.0f��Χ
        float fogLerp = saturate((distToEye - g_FogStart) / g_FogRange);
        // ������ɫ�͹�����ɫ�������Բ�ֵ
        litColor = lerp(litColor, g_FogColor, fogLerp);
    }

    litColor.a = texColor.a * g_Material.diffuse.a;
    return litColor;
}