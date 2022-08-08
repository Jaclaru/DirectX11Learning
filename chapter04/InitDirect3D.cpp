//***************************************************************************************
// Init Direct3D.cpp by Frank Luna (C) 2011 All Rights Reserved.
//
// Demonstrates the sample framework by initializing Direct3D, clearing 
// the screen, and displaying frame stats.
//
//***************************************************************************************

#include "D3DApp.h"
#include "D3DUtil.h"
#include "DXTrace.h"

class InitDirect3DApp : public D3DApp
{
public:
	InitDirect3DApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
	~InitDirect3DApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene(); 
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// 这些参数不使用
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(showCmd);

	// 允许在Debug版本进行运行时内存分配和泄漏检测
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	InitDirect3DApp theApp(hInstance, L"DirectX11 Initialization", 1280, 720);
	
	if( !theApp.Init() )
		return 0;
	
	return theApp.Run();
}

InitDirect3DApp::InitDirect3DApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight)
: D3DApp(hInstance, windowName, initWidth, initHeight)
{
}

InitDirect3DApp::~InitDirect3DApp()
{
}

bool InitDirect3DApp::Init()
{
	if(!D3DApp::Init())
		return false;

	return true;
}

void InitDirect3DApp::OnResize()
{
	D3DApp::OnResize();
}

void InitDirect3DApp::UpdateScene(float dt)
{

}

void InitDirect3DApp::DrawScene()
{
	assert(m_pd3dImmediateContext);
	assert(m_pSwapChain);

	static float blue[4] = {0.0f, 0.0f, 1.0f, 1.0f};
	m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), blue);
	m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	HR(m_pSwapChain->Present(0, 0));
}
