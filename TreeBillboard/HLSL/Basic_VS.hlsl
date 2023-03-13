#include "Basic.hlsli"

VertexPosHWNormalTex VS(VertexPosNormalTex vIn)
{
    VertexPosHWNormalTex vOut;
    const matrix viewProj = mul(g_View, g_Proj);
    const vector posW = mul(float4(vIn.posL, 1.0f), g_World);

    vOut.posW = posW.xyz;
    vOut.posH = mul(posW, viewProj);
    vOut.normalW = mul(vIn.normalL, (float3x3) g_WorldInvTranspose);
    vOut.tex = vIn.tex;
    return  vOut;
}
