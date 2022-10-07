#pragma once

#include "D3DApp.h"

class CubeApp : public D3DApp
{
public:
	struct VertexPosColor
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT4 color;
		static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
	};

	struct ConstantBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
		DirectX::XMFLOAT4 color;
		uint32_t useCustomColor;
		uint32_t pads[3];
	};

public:
	CubeApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
	~CubeApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

private:
	// 初始化渲染效果
	bool InitEffect();

	// 初始化渲染资源
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
};