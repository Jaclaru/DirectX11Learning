#include "Basic.hlsli"

// 一个v0v1线段输出6个三角形顶点
[maxvertexcount(12)]
void GS(triangle VertexPosNormalColor input[3], inout TriangleStream<VertexPosNormalColor> output)
{
	//
    // 将一个三角形分裂成四个三角形，但同时顶点v3, v4, v5也需要在球面上
    //       v1
    //       /\
    //      /  \
    //   v3/____\v4
    //    /\xxxx/\
    //   /  \xx/  \
    //  /____\/____\
    // v0    v5    v2

    VertexPosNormalColor vertices[6];
    const matrix viewProj = mul(g_View, g_Proj);
    [unroll]
    for(int i = 0; i < 3; ++i)
    {
	    vertices[i] = input[i];
        vertices[i + 3].color = lerp(input[i].color, input[(i + 1) % 3].color, 0.5f);
        vertices[i + 3].normalL = normalize(input[i].normalL + input[(i + 1) % 3].normalL);
        vertices[i + 3].posL = g_SphereCenter + g_SphereRadius * vertices[i + 3].normalL;
    }

    output.Append(vertices[0]);
    output.Append(vertices[3]);
    output.Append(vertices[5]);
    output.RestartStrip();

    output.Append(vertices[3]);
    output.Append(vertices[4]);
    output.Append(vertices[5]);
    output.RestartStrip();

    output.Append(vertices[5]);
    output.Append(vertices[4]);
    output.Append(vertices[2]);
    output.RestartStrip();

    output.Append(vertices[3]);
    output.Append(vertices[1]);
    output.Append(vertices[4]);
}