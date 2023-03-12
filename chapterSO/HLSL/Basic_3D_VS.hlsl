#include "Basic.hlsli"

// ������ɫ��(3D)
VertexPosHWNormalTex VS(VertexPosNormalTex vIn)
{
    VertexPosHWNormalTex vOut;
    
    matrix viewProj = mul(g_View, g_Proj);
    float4 posW = mul(float4(vIn.posL, 1.0f), g_World);
    float3 normalW = mul(vIn.normalL, (float3x3) g_WorldInvTranspose);
    // ����ǰ�ڻ��Ʒ������壬�Ƚ��з������
    [flatten]
    if (g_IsReflection)
    {
        posW = mul(posW, g_Reflection);
        normalW = mul(normalW, (float3x3) g_Reflection);
    }
    // ����ǰ�ڻ�����Ӱ���Ƚ���ͶӰ����
    [flatten]
    if (g_IsShadow)
    {
        posW = (g_IsReflection ? mul(posW, g_RefShadow) : mul(posW, g_Shadow));
    }
    vOut.posH = mul(posW, viewProj);
    vOut.posW = posW.xyz;
    vOut.normalW = normalW;
    vOut.tex = vIn.tex;
    return vOut;
}