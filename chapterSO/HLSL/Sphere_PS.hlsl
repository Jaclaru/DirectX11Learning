#include "Basic.hlsli"

float4 PS(VertexPosHWNormalColor pIn) : SV_Target
{
    // ��׼��������
    pIn.normalW = normalize(pIn.normalW);

    // ����ָ���۾�������
    float3 toEyeW = normalize(g_EyePosW - pIn.posW);

    // ��ʼ��Ϊ0 
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // ֻ���㷽���
    ComputeDirectionalLight(g_Material, g_DirLight[0], pIn.normalW, toEyeW, ambient, diffuse, spec);

    return pIn.color * (ambient + diffuse) + spec;
}
