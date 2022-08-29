#pragma once

#include "D3DApp.h"

class BoxApp : public D3DApp
{
public:
	struct Vectex
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
	BoxApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
	~BoxApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	bool InitEffect();
	bool InitResource();

private:
	// ���㻺����
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	// ����������
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	// ����������
	ComPtr<ID3D11Buffer> m_pConstantBuffer;

	// ������ɫ��
	ComPtr<ID3D11VertexShader> m_pVertexShader;	
	// ������ɫ��
	ComPtr<ID3D11PixelShader> m_pPixelShader;	

	// �������벼��
	ComPtr<ID3D11InputLayout> m_pVertexLayout;	
		
	// �����޸�GPU�����������ı���
	ConstantBuffer m_CBuffer;                       	
};