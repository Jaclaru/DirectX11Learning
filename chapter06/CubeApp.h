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
	// ��ʼ����ȾЧ��
	bool InitEffect();

	// ��ʼ����Ⱦ��Դ
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