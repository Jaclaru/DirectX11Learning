
#include "D3DApp.h"
#include "D3DUtil.h"
#include "D3DApp.h"
#include "DXTrace.h"

using namespace DirectX;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance,
    _In_ LPSTR cmdLine, _In_ int showCmd)
{
    // ��Щ������ʹ��
    UNREFERENCED_PARAMETER(prevInstance);
    UNREFERENCED_PARAMETER(cmdLine);
    UNREFERENCED_PARAMETER(showCmd);
    // ������Debug�汾��������ʱ�ڴ�����й©���
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    /*ShapesApp theApp(hInstance, L"Rendering a Box", 1280, 720);

    if (!theApp.Init())
        return 0;

    return theApp.Run();*/
}