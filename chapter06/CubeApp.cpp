#include "CubeApp.h"
#include "D3DUtil.h"
#include "DXTrace.h"
#include "MathHelper.h"

using namespace DirectX;

const D3D11_INPUT_ELEMENT_DESC CubeApp::VertexPosColor::inputLayout[2] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

CubeApp::CubeApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight):
	D3DApp(hInstance, windowName, initWidth, initHeight),
	m_CBuffer()
{
}

CubeApp::~CubeApp()
{

}

bool CubeApp::Init()
{
	if(!D3DApp::Init())
		return false;

	if (!InitEffect())
		return false;

	if (!InitResource())
		return false;

	return true;
}

void CubeApp::OnResize()
{
	D3DApp::OnResize();

	//// ������ڴ�С�ı��ˣ���ô��Ҫ���º��ݱȣ�������Ҫ���¼���ͶӰ����
	//m_CBuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));
}

void CubeApp::UpdateScene(float dt)
{
	// ImGui�ڲ�ʾ������
	/*ImGui::ShowAboutWindow();
	ImGui::ShowDemoWindow();
	ImGui::ShowUserGuide();*/

	// ��ȡIO�¼�
	ImGuiIO& io = ImGui::GetIO();

	//
	// �Զ��崰�������
	//
	static float tx = 0.0f, ty = 0.0f, phi = 0.0f, theta = 0.0f, scale = 1.0f, fov = XM_PIDIV2;
	static bool animateCube = true, customColor = false;
	if (animateCube)
	{
		phi += 0.3f * dt, theta += 0.37f * dt;
		phi = XMScalarModAngle(phi);
		theta = XMScalarModAngle(theta);
	}
	if (ImGui::Begin("Use ImGui"))
	{
		ImGui::Checkbox("Animate Cube", &animateCube);
		ImGui::SameLine(0.0f, 25.0f);
		if (ImGui::Button("Reset Params"))
		{
			tx = ty = phi = theta = 0.0f;
			scale = 1.0f;
			fov = XM_PIDIV2;
		}
		ImGui::SliderFloat("Scale", &scale, 0.2f, 2.0f);

		ImGui::Text("Phi: %.2f degrees", XMConvertToDegrees(phi));
		ImGui::SliderFloat("##1", &phi, -XM_PI, XM_PI, "");		// ����ʾ���֣��������ظ���ǩ
		ImGui::Text("Theta: %.2f degrees", XMConvertToDegrees(theta));
		ImGui::SliderFloat("##2", &theta, -XM_PI, XM_PI, "");

		ImGui::Text("Position: (%.1f, %.1f, 0.0)", tx, ty);

		if (ImGui::Checkbox("Use Custom Color", &customColor))
			m_CBuffer.useCustomColor = customColor;
		if (customColor)
		{
			ImGui::ColorEdit3("Color", reinterpret_cast<float*>(&m_CBuffer.color));
		}
	}
	ImGui::End();

	// �������ڲ���UIʱ��������
	if (!ImGui::IsAnyItemActive())
	{
		// �������϶�ƽ��
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		{
			tx += io.MouseDelta.x * 0.01f;
			ty -= io.MouseDelta.y * 0.01f;
		}
		// ����Ҽ��϶���ת
		else if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
		{
			phi -= io.MouseDelta.y * 0.01f;
			theta -= io.MouseDelta.x * 0.01f;
			phi = XMScalarModAngle(phi);
			theta = XMScalarModAngle(theta);
		}
		// ����������
		else if (io.MouseWheel != 0.0f)
		{
			scale += 0.02f * io.MouseWheel;
			if (scale > 2.0f)
				scale = 2.0f;
			else if (scale < 0.2f)
				scale = 0.2f;
		}
	}

	m_CBuffer.world = XMMatrixTranspose(
		XMMatrixScalingFromVector(XMVectorReplicate(scale)) *
		XMMatrixRotationX(phi) * XMMatrixRotationY(theta) * 
		XMMatrixTranslation(tx, ty, 0.0f));
	m_CBuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(fov, AspectRatio(), 1.0f, 1000.0f));

	// ���³�������������������ת����
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pd3dImmediateContext->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(m_CBuffer), &m_CBuffer, sizeof(m_CBuffer));
	m_pd3dImmediateContext->Unmap(m_pConstantBuffer.Get(), 0);
}

void CubeApp::DrawScene()
{
	assert(m_pd3dImmediateContext);
	assert(m_pSwapChain);

	static float black[4] = {0.0f, 0.0f, 0.0f, 1.0f}; // RGBA = (0, 0, 0, 255)
	m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), reinterpret_cast<const float*>(&black));
	m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// ����������
	m_pd3dImmediateContext->DrawIndexed(36, 0, 0);

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	HR(m_pSwapChain->Present(0, 0));
}

bool CubeApp::InitEffect()
{
	// A generic chunk of memory that has two methods: GetBufferPointer(), GetBufferSize();
	ComPtr<ID3DBlob> blob;

	// ����������ɫ��
	HR(CreateShaderFromFile(L"HLSL\\Cube_VS.cso", L"HLSL\\Cube_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader.GetAddressOf()));

	// �������㲼��
	HR(m_pd3dDevice->CreateInputLayout(VertexPosColor::inputLayout, ARRAYSIZE(VertexPosColor::inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout.GetAddressOf()));

	// ����������ɫ��
	HR(CreateShaderFromFile(L"HLSL\\Cube_PS.cso", L"HLSL\\Cube_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader.GetAddressOf()));

	return true;
}

bool CubeApp::InitResource()
{
	// ******************
   // ���������嶥��
   //    5________ 6
   //    /|      /|
   //   /_|_____/ |
   //  1|4|_ _ 2|_|7
   //   | /     | /
   //   |/______|/
   //  0       3
	VertexPosColor vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), (XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }
	};

	// ���ö��㻺��������
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof vertices;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;

	// �½����㻺����
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	HR(m_pd3dDevice->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf()));

	// ******************
	// ��������
	//
	DWORD indices[] = {
		// ����
		0, 1, 2,
		2, 3, 0,
		// ����
		4, 5, 1,
		1, 0, 4,
		// ����
		1, 5, 6,
		6, 2, 1,
		// ����
		7, 6, 5,
		5, 4, 7,
		// ����
		3, 2, 6,
		6, 7, 3,
		// ����
		4, 0, 3,
		3, 7, 4
	};
	// ������������������
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof indices;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// �½�����������
	InitData.pSysMem = indices;
	HR(m_pd3dDevice->CreateBuffer(&ibd, &InitData, m_pIndexBuffer.GetAddressOf()));

	// ******************
    // ���ó�������������
    //
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// �½���������������ʹ�ó�ʼ����
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffer.GetAddressOf()));

	// ��ʼ��������������ֵ
	m_CBuffer.world = XMMatrixIdentity();
	m_CBuffer.view = XMMatrixTranspose(XMMatrixLookAtLH(
		XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	));
	m_CBuffer.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_CBuffer.useCustomColor = false;

	// ******************
	// ����Ⱦ���߸����׶ΰ󶨺�������Դ
	//

	// ����װ��׶εĶ��㻺��������
	UINT stride = sizeof(VertexPosColor);   // ��Խ�ֽ���
	UINT offset = 0;				// ��ʼƫ����

	m_pd3dImmediateContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	m_pd3dImmediateContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	// ����ͼԪ���ͣ��趨���벼��
	m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout.Get());
	// ����ɫ���󶨵���Ⱦ����
	m_pd3dImmediateContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	// �����ºõĳ����������󶨵�������ɫ��
	m_pd3dImmediateContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());

	// ��������ɫ���󶨵���Ⱦ����
	m_pd3dImmediateContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

	// ******************
   // ���õ��Զ�����
   //
	D3D11SetDebugObjectName(m_pVertexLayout.Get(), "VertexPosColorLayout");
	D3D11SetDebugObjectName(m_pVertexBuffer.Get(), "VertexBuffer");
	D3D11SetDebugObjectName(m_pIndexBuffer.Get(), "IndexBuffer");
	D3D11SetDebugObjectName(m_pConstantBuffer.Get(), "ConstantBuffer");
	D3D11SetDebugObjectName(m_pVertexShader.Get(), "Cube_VS");
	D3D11SetDebugObjectName(m_pPixelShader.Get(), "Cube_PS");

	return true;
}