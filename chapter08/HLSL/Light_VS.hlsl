#include "Light.hlsli"

// ������ɫ��
VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;

    matrix viewProj = mul(g_View, g_Proj);
    // ת��������ռ�
    float4 posW = mul(float4(vIn.posL, 1.0f), g_World);
    // ת������βü��ռ�
    vOut.posH = mul(posW, viewProj);
    vOut.posW = posW.xyz;
    vOut.normalW = mul(vIn.normalL, (float3x3)g_WorldInvTranspose);
    vOut.color = vIn.color; // ����alphaͨ����ֵĬ��Ϊ1.0
    return vOut;
}