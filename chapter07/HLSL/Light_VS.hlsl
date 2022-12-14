#include "Light.hlsli"

// 顶点着色器
VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;

    matrix viewProj = mul(g_View, g_Proj);
    // 转换到世界空间
    float4 posW = mul(float4(vIn.posL, 1.0f), g_World);
    // 转换到齐次裁剪空间
    vOut.posH = mul(posW, viewProj);
    vOut.posW = posW.xyz;
    vOut.normalW = mul(vIn.normalL, (float3x3)g_WorldInvTranspose);
    vOut.color = vIn.color; // 这里alpha通道的值默认为1.0
    return vOut;
}
