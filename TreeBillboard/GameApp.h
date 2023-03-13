#ifndef GAMEAPP_H
#define GAMEAPP_H

#include <ctime>
#include "D3DApp.h"
#include "Camera.h"
#include "GameObject.h"

// 编程捕获帧(需支持DirectX 11.2 API)
//#if defined(DEBUG) | defined(_DEBUG)
//#include <DXGItype.h>  
//#include <dxgi1_2.h>  
//#include <dxgi1_3.h>  
//#include <DXProgrammableCapture.h>  
//#endif

class GameApp : public D3DApp
{
public:
	// 摄像机模式
    enum class CameraMode { FirstPerson, ThirdPerson, Free };
    
public:
    GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    ~GameApp() override;

    bool Init() override;
    void OnResize() override;
    void UpdateScene(float dt) override;
    void DrawScene() override;

private:
    bool InitResource();
	void InitPointSpritesBuffer();


private:

    ComPtr<ID3D11Buffer> mPointSpritesBuffer;				    // 点精灵顶点缓冲区
    ComPtr<ID3D11ShaderResourceView> mTreeTexArray;			    // 树的纹理数组
    Material m_TreeMat;										    // 树的材质

    GameObject m_Ground;										// 地面
    
    BasicEffect m_BasicEffect;							        // 对象渲染特效管理

    CameraMode m_CameraMode;									// 摄像机模式
    std::shared_ptr<Camera> m_pCamera;						    // 摄像机

    bool m_FogEnabled;										    // 是否开启雾效
    bool m_IsNight;											    // 是否黑夜
    bool m_EnableAlphaToCoverage;							    // 是否开启Alpha-To-Coverage

    float m_FogRange;										    // 雾效范围
};


#endif