#include "Basic.hlsli"

// ������ɫ��(2D)
VertexPosHTex VS(VertexPosTex vIn)
{
    VertexPosHTex vOut;
    vOut.posH = float4(vIn.posL, 1.0f);
    vOut.tex = vIn.tex;
    return vOut;
}
