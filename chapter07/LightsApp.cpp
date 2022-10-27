#include "LightsApp.h"
#include "D3DUtil.h"
#include "DXTrace.h"

using namespace DirectX;

GameApp::GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight)
    : D3DApp(hInstance, windowName, initWidth, initHeight),
    m_IndexCount(),
    m_VSConstantBuffer(),
    m_PSConstantBuffer(),
    m_DirLight(),
    m_PointLight(),
    m_SpotLight(),
    m_IsWireframeMode(false)
{
}

GameApp::~GameApp()
{
}

bool GameApp::Init()
{
    if (!D3DApp::Init())
        return false;

    if (!InitEffect())
        return false;

    if (!InitResource())
        return false;

    return true;
}

void GameApp::OnResize()
{
    assert(m_pd2dFactory);
    assert(m_pdwriteFactory);
    // �ͷ�D2D�������Դ
    m_pColorBrush.Reset();
    m_pd2dRenderTarget.Reset();

    D3DApp::OnResize();

    // ΪD2D����DXGI������ȾĿ��
    ComPtr<IDXGISurface> surface;
    HR(m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(surface.GetAddressOf())));
    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
    HRESULT hr = m_pd2dFactory->CreateDxgiSurfaceRenderTarget(surface.Get(), &props, m_pd2dRenderTarget.GetAddressOf());
    surface.Reset();

    if (hr == E_NOINTERFACE)
    {
        OutputDebugString(L"\n���棺Direct2D��Direct3D�������Թ������ޣ��㽫�޷������ı���Ϣ�����ṩ������ѡ������\n"
            "1. ����Win7ϵͳ����Ҫ������Win7 SP1������װKB2670838������֧��Direct2D��ʾ��\n"
            "2. �������Direct3D 10.1��Direct2D�Ľ�����������ģ�"
            "https://docs.microsoft.com/zh-cn/windows/desktop/Direct2D/direct2d-and-direct3d-interoperation-overview""\n"
            "3. ʹ�ñ������⣬����FreeType��\n\n");
    }
    else if (hr == S_OK)
    {
        // �����̶���ɫˢ���ı���ʽ
        HR(m_pd2dRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::White),
            m_pColorBrush.GetAddressOf()));
        HR(m_pdwriteFactory->CreateTextFormat(L"����", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"zh-cn",
            m_pTextFormat.GetAddressOf()));
    }
    else
    {
        // �����쳣����
        assert(m_pd2dRenderTarget);
    }

}

void GameApp::UpdateScene(float dt)
{
    static float phi = 0.0f, theta = 0.0f;
    phi += 0.3f * dt, theta += 0.37f * dt;
    XMMATRIX W = XMMatrixRotationX(phi) * XMMatrixRotationY(theta);
    m_VSConstantBuffer.world = XMMatrixTranspose(W);
    m_VSConstantBuffer.worldInvTranspose = XMMatrixTranspose(InverseTranspose(W));

    if (ImGui::Begin("Lighting"))
    {
        static int curr_mesh_item = 0;
        const char* mesh_strs[] = {
            "Box",
            "Sphere",
            "Cylinder",
            "Cone"
        };
        if (ImGui::Combo("Mesh", &curr_mesh_item, mesh_strs, ARRAYSIZE(mesh_strs)))
        {
            Geometry::MeshData<VertexPosNormalColor> meshData;
            switch (curr_mesh_item)
            {
            case 0: meshData = Geometry::CreateBox<VertexPosNormalColor>(); break;
            case 1: meshData = Geometry::CreateSphere<VertexPosNormalColor>(); break;
            case 2: meshData = Geometry::CreateCylinder<VertexPosNormalColor>(); break;
            case 3: meshData = Geometry::CreateCone<VertexPosNormalColor>(); break;
            }
            ResetMesh(meshData);
        }
        bool mat_changed = false;
        ImGui::Text("Material");
        ImGui::PushID(3);
        ImGui::ColorEdit3("Ambient", &m_PSConstantBuffer.material.ambient.x);
        ImGui::ColorEdit3("Diffuse", &m_PSConstantBuffer.material.diffuse.x);
        ImGui::ColorEdit3("Specular", &m_PSConstantBuffer.material.specular.x);
        ImGui::PopID();

        static int curr_light_item = 0;
        static const char* light_modes[] = {
            "Directional Light",
            "Point Light",
            "Spot Light"
        };
        ImGui::Text("Light");
        if (ImGui::Combo("Light Type", &curr_light_item, light_modes, ARRAYSIZE(light_modes)))
        {
            m_PSConstantBuffer.dirLight = (curr_light_item == 0 ? m_DirLight : DirectionalLight());
            m_PSConstantBuffer.pointLight = (curr_light_item == 1 ? m_PointLight : PointLight());
            m_PSConstantBuffer.spotLight = (curr_light_item == 2 ? m_SpotLight : SpotLight());
        }

        bool light_changed = false;
        // ���ID����ͬ���ؼ�
        ImGui::PushID(curr_light_item);
        if (curr_light_item == 0)
        {
            ImGui::ColorEdit3("Ambient", &m_PSConstantBuffer.dirLight.ambient.x);
            ImGui::ColorEdit3("Diffuse", &m_PSConstantBuffer.dirLight.diffuse.x);
            ImGui::ColorEdit3("Specular", &m_PSConstantBuffer.dirLight.specular.x);
        }
        else if (curr_light_item == 1)
        {
            ImGui::ColorEdit3("Ambient", &m_PSConstantBuffer.pointLight.ambient.x);
            ImGui::ColorEdit3("Diffuse", &m_PSConstantBuffer.pointLight.diffuse.x);
            ImGui::ColorEdit3("Specular", &m_PSConstantBuffer.pointLight.specular.x);
            ImGui::InputFloat("Range", &m_PSConstantBuffer.pointLight.range);
            ImGui::InputFloat3("Attenutation", &m_PSConstantBuffer.pointLight.att.x);
        }
        else
        {
            ImGui::ColorEdit3("Ambient", &m_PSConstantBuffer.spotLight.ambient.x);
            ImGui::ColorEdit3("Diffuse", &m_PSConstantBuffer.spotLight.diffuse.x);
            ImGui::ColorEdit3("Specular", &m_PSConstantBuffer.spotLight.specular.x);
            ImGui::InputFloat("Spot", &m_PSConstantBuffer.spotLight.spot);
            ImGui::InputFloat("Range", &m_PSConstantBuffer.spotLight.range);
            ImGui::InputFloat3("Attenutation", &m_PSConstantBuffer.spotLight.att.x);
        }
        ImGui::PopID();


        if (ImGui::Checkbox("WireFrame Mode", &m_IsWireframeMode))
        {
            m_pd3dImmediateContext->RSSetState(m_IsWireframeMode ? m_pRSWireframe.Get() : nullptr);
        }
    }
    ImGui::End();
    ImGui::Render();

    // ���³�������������������ת����
    D3D11_MAPPED_SUBRESOURCE mappedData;
    HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[0].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
    memcpy_s(mappedData.pData, sizeof(VSConstantBuffer), &m_VSConstantBuffer, sizeof(VSConstantBuffer));
    m_pd3dImmediateContext->Unmap(m_pConstantBuffers[0].Get(), 0);

    HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[1].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
    memcpy_s(mappedData.pData, sizeof(PSConstantBuffer), &m_PSConstantBuffer, sizeof(PSConstantBuffer));
    m_pd3dImmediateContext->Unmap(m_pConstantBuffers[1].Get(), 0);
}

void GameApp::DrawScene()
{
    assert(m_pd3dImmediateContext);
    assert(m_pSwapChain);

    m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), reinterpret_cast<const float*>(&Colors::Black));
    m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // ���Ƽ���ģ��
    m_pd3dImmediateContext->DrawIndexed(m_IndexCount, 0, 0);

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // ����Direct2D����
    if (m_pd2dRenderTarget != nullptr)
    {
        m_pd2dRenderTarget->BeginDraw();
        std::wstring textStr = L"�л��ƹ�����: 1-ƽ�й� 2-��� 3-�۹��\n"
            "�л�ģ��: Q-������ W-���� E-Բ���� R-Բ׶��\n"
            "S-�л�ģʽ ��ǰģʽ: ";
        if(m_IsWireframeMode)
            textStr += L"�߿�ģʽ";
        else
            textStr += L"��ģʽ";
        m_pd2dRenderTarget->DrawTextW(textStr.c_str(), textStr.size(), m_pTextFormat.Get(),
            D2D1_RECT_F{0.0f, 0.0f, 600.0f, 200.0f }, m_pColorBrush.Get());
        HR(m_pd2dRenderTarget->EndDraw());
    }

    HR(m_pSwapChain->Present(0, 0));
}

bool GameApp::InitEffect()
{
    ComPtr<ID3DBlob> blob;

    // ����������ɫ��
    HR(CreateShaderFromFile(L"HLSL\\Light_VS.cso", L"HLSL\\Light_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
    HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader.GetAddressOf()));
    // �������󶨶��㲼��
    HR(m_pd3dDevice->CreateInputLayout(VertexPosNormalColor::inputLayout, ARRAYSIZE(VertexPosNormalColor::inputLayout),
        blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout.GetAddressOf()));

    // ����������ɫ��
    HR(CreateShaderFromFile(L"HLSL\\Light_PS.cso", L"HLSL\\Light_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
    HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader.GetAddressOf()));

    return true;
}

bool GameApp::InitResource()
{
    // ******************
    // ��ʼ������ģ��
    //
    auto meshData = Geometry::CreateBox<VertexPosNormalColor>();
    ResetMesh(meshData);


    // ******************
    // ���ó�������������
    //
    D3D11_BUFFER_DESC cbd;
    ZeroMemory(&cbd, sizeof(cbd));
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.ByteWidth = sizeof(VSConstantBuffer);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    // �½�����VS��PS�ĳ���������
    HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[0].GetAddressOf()));
    cbd.ByteWidth = sizeof(PSConstantBuffer);
    HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[1].GetAddressOf()));

    // ******************
    // ��ʼ��Ĭ�Ϲ���
    // �����
    m_DirLight.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    m_DirLight.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
    m_DirLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_DirLight.direction = XMFLOAT3(-0.577f, -0.577f, 0.577f);
    // ���
    m_PointLight.position = XMFLOAT3(0.0f, 0.0f, -10.0f);
    m_PointLight.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
    m_PointLight.diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
    m_PointLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_PointLight.att = XMFLOAT3(0.0f, 0.1f, 0.0f);
    m_PointLight.range = 25.0f;
    // �۹��
    m_SpotLight.position = XMFLOAT3(0.0f, 0.0f, -5.0f);
    m_SpotLight.direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
    m_SpotLight.ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    m_SpotLight.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_SpotLight.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_SpotLight.att = XMFLOAT3(1.0f, 0.0f, 0.0f);
    m_SpotLight.spot = 12.0f;
    m_SpotLight.range = 10000.0f;
    // ��ʼ������VS�ĳ�����������ֵ
    m_VSConstantBuffer.world = XMMatrixIdentity();
    m_VSConstantBuffer.view = XMMatrixTranspose(XMMatrixLookAtLH(
        XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
        XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    ));
    m_VSConstantBuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));
    m_VSConstantBuffer.worldInvTranspose = XMMatrixIdentity();

    // ��ʼ������PS�ĳ�����������ֵ
    m_PSConstantBuffer.material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_PSConstantBuffer.material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_PSConstantBuffer.material.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 5.0f);
    // ʹ��Ĭ��ƽ�й�
    m_PSConstantBuffer.dirLight = m_DirLight;
    // ע�ⲻҪ�������ô˴��Ĺ۲�λ�ã�����������ֻ�������
    m_PSConstantBuffer.eyePos = XMFLOAT4(0.0f, 0.0f, -5.0f, 0.0f);

    // ����PS������������Դ
    D3D11_MAPPED_SUBRESOURCE mappedData;
    HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[1].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
    memcpy_s(mappedData.pData, sizeof(PSConstantBuffer), &m_PSConstantBuffer, sizeof(PSConstantBuffer));
    m_pd3dImmediateContext->Unmap(m_pConstantBuffers[1].Get(), 0);

    // ******************
    // ��ʼ����դ��״̬
    //
    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
    rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.DepthClipEnable = true;
    HR(m_pd3dDevice->CreateRasterizerState(&rasterizerDesc, m_pRSWireframe.GetAddressOf()));

    // ******************
    // ����Ⱦ���߸����׶ΰ󶨺�������Դ
    //

    // ����ͼԪ���ͣ��趨���벼��
    m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout.Get());
    // ����ɫ���󶨵���Ⱦ����
    m_pd3dImmediateContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
    // VS������������ӦHLSL�Ĵ���b0�ĳ���������
    m_pd3dImmediateContext->VSSetConstantBuffers(0, 1, m_pConstantBuffers[0].GetAddressOf());
    // PS������������ӦHLSL�Ĵ���b1�ĳ���������
    m_pd3dImmediateContext->PSSetConstantBuffers(1, 1, m_pConstantBuffers[1].GetAddressOf());
    m_pd3dImmediateContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

    // ******************
    // ���õ��Զ�����
    //
    D3D11SetDebugObjectName(m_pVertexLayout.Get(), "VertexPosNormalTexLayout");
    D3D11SetDebugObjectName(m_pConstantBuffers[0].Get(), "VSConstantBuffer");
    D3D11SetDebugObjectName(m_pConstantBuffers[1].Get(), "PSConstantBuffer");
    D3D11SetDebugObjectName(m_pVertexShader.Get(), "Light_VS");
    D3D11SetDebugObjectName(m_pPixelShader.Get(), "Light_PS");

    return true;
}

bool GameApp::ResetMesh(const Geometry::MeshData<VertexPosNormalColor>& meshData)
{
	// �ͷž���Դ
	m_pVertexBuffer.Reset();
	m_pIndexBuffer.Reset();

	// ���ö��㻺��������
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = (UINT)meshData.vertexVec.size() * sizeof(VertexPosNormalColor);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	// �½����㻺����
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = meshData.vertexVec.data();
	HR(m_pd3dDevice->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf()));

	// ����װ��׶εĶ��㻺��������
	UINT stride = sizeof(VertexPosNormalColor);
	UINT offset = 0;

	m_pd3dImmediateContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

	// ��������������
	m_IndexCount = (UINT)meshData.indexVec.size();
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = m_IndexCount * sizeof(DWORD);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// �½�����������
	InitData.pSysMem = meshData.indexVec.data();
	HR(m_pd3dDevice->CreateBuffer(&ibd, &InitData, m_pIndexBuffer.GetAddressOf()));
	// ����װ��׶ε���������������
	m_pd3dImmediateContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);


    // ���õ��Զ�����
    D3D11SetDebugObjectName(m_pVertexBuffer.Get(), "VertexBuffer");
    D3D11SetDebugObjectName(m_pIndexBuffer.Get(), "IndexBuffer");

	return true;
}