// Triangle_VS.hlsl
#include "Basic.hlsli"

VertexPosHColor VS(VertexPosColor vIn)
{
    const matrix worldViewProj = mul(mul(g_World, g_View), g_Proj);
    VertexPosHColor vOut;
    vOut.Color = vIn.Color;
    vOut.PosH = mul(float4(vIn.PosL, 1.0f), worldViewProj);
    return vOut;
}
