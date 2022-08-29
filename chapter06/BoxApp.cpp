#include "BoxApp.h"

BoxApp::BoxApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight):
	D3DApp(hInstance, windowName, initWidth, initHeight),
	m_CBuffer()
{

}

BoxApp::~BoxApp()
{

}

bool BoxApp::Init()
{
	if(!D3DApp::Init())
		return false;

	if (!InitEffect())
		return false;

	if (!InitResource())
		return false;

	return true;
}

void BoxApp::OnResize()
{
	D3DApp::OnResize();
}