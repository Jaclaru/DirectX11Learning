#pragma once

#include "D3DApp.h"

class HillsApp : public D3DApp
{
public:
	struct Vertex
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT4 Color;
		static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
	};

	struct ConstantBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};

public:
	HillsApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
	~HillsApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	// 初始化渲染效果
	bool InitEffect();

	// 计算高度
	float GetHeight(float x, float z) const;

	// 构建几何缓冲
	bool InitResource();

private:
	// 顶点缓冲区
	ComPtr<ID3D11Buffer> m_pVertexBuffer;

	// 索引缓冲区
	ComPtr<ID3D11Buffer> m_pIndexBuffer;

	// 常量缓存区
	ComPtr<ID3D11Buffer> m_pConstantBuffer;

	// 顶点着色器
	ComPtr<ID3D11VertexShader> m_pVertexShader;

	// 像素着色器
	ComPtr<ID3D11PixelShader> m_pPixelShader;

	// 顶点输入布局
	ComPtr<ID3D11InputLayout> m_pVertexLayout;

	// 用于修改GPU常量缓冲区的变量
	ConstantBuffer m_CBuffer;

	// 用于更新变换矩阵的一些数值
	float mTheta, mPhi, mRadius;

	// 网格索引计数
	UINT m_nGridIndexCount;

	// 用于记录鼠标位置
	POINT mLastMousePos;
};