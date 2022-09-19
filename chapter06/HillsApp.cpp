
#include "GeometryGenerator.h"
#include "HillsApp.h"

using namespace DirectX;

void HillApp::BuildGeometryBuffers()
{
	GeometryGenerator::MeshData grid;

	GeometryGenerator geoGen;

	geoGen.CreateGrid(160.0f, 160.0f, 50, 50, grid);

	m_nGridIndexCount = grid.Indices.size();

	// 提取出我所需要的顶点元素，并将高度计算函数作用在每一个顶点上。
	// 此外，根据顶点的高度将顶点着色，我们将得到山景。

	std::vector<Vertex> vertices(grid.Vertices.size());
	for (size_t i = 0; i < grid.Vertices.size(); ++i)
	{
		XMFLOAT3 p = grid.Vertices[i].Position;

		p.y = GetHeight(p.x, p.z);

		vertices[i].Pos = p;

		// 根据顶点的高度将顶点着色。
		if (p.y < -10.0f)
		{
			// 沙滩颜色
			vertices[i].Color = XMFLOAT4(1.0f, 0.96f, 0.62f, 1.0f);
		}
		else if(p.y < 5.0f)
		{
			// 亮黄绿色
			vertices[i].Color = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
		}
		else if (p.y < 12.0f)
		{
			// 暗黄绿色
			vertices[i].Color = XMFLOAT4(0.1f, 0.48f, 0.19f, 1.0f);
		}
		else if (p.y < 20.0f)
		{
			// 暗棕色
			vertices[i].Color = XMFLOAT4(0.45f, 0.39f, 0.34f, 1.0f);
		}
		else 
		{
			// 白雪
			vertices[i].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}


}