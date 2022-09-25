#pragma once

#include "D3DApp.h"

class ShapesApp : public D3DApp
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
	ShapesApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
	~ShapesApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

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

	// ��Ⱦ����
	ComPtr<ID3D11RasterizerState> m_pWireFrameRS;

	// �����޸�GPU�����������ı���
	ConstantBuffer m_CBuffer;

	// ���ڸ��±任�����һЩ��ֵ
	float mTheta, mPhi, mRadius;

	// ���ڼ�¼���λ��
	POINT mLastMousePos;

	// Define transformations from local spaces to world space.
	DirectX::XMFLOAT4X4 mSphereWorld[10];
	DirectX::XMFLOAT4X4 mCylWorld[10];
	DirectX::XMFLOAT4X4 mBoxWorld;
	DirectX::XMFLOAT4X4 mGridWorld;
	DirectX::XMFLOAT4X4 mCenterSphere;

	int mBoxVertexOffset;
	int mGridVertexOffset;
	int mSphereVertexOffset;
	int mCylinderVertexOffset;

	UINT mBoxIndexOffset;
	UINT mGridIndexOffset;
	UINT mSphereIndexOffset;
	UINT mCylinderIndexOffset;

	UINT mBoxIndexCount;
	UINT mGridIndexCount;
	UINT mSphereIndexCount;
	UINT mCylinderIndexCount;
};