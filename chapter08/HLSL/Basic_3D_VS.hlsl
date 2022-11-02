#include "Basic.hlsli"

// ¶¥µã×ÅÉ«Æ÷(3D)
VertexPosHWNormalTex VS(VertexPosNormalTex vIn)
{
    VertexPosHWNormalTex vOut;
    matrix viewProj = mul(g_View, g_Proj);
    float4 posW = mul(float4(vIn.PosL, 1.0f), g_World);
    
    vOut.posH = mul(posW, viewProj);
    vOut.posW = posW.xyz;
    vOut.normalW = mul(vIn.NormalL, (float3x3) g_WorldInvTranspose);
    vOut.Tex = vIn.Tex;
    return vOut;
}