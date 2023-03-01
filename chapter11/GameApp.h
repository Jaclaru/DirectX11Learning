#ifndef GAMEAPP_H
#define GAMEAPP_H

#include <D3DApp.h>

#include "Effects.h"


class GameApp : public D3DApp
{
public:
    enum class Mode { SplitedTriangle, CylinderNoCap, CylinderNoCapWithNormal };

public:
    GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    ~GameApp() override;

    bool Init() override;
    void OnResize() override;
    void UpdateScene(float dt) override;
    void DrawScene() override;

private:
    bool InitResource();

    void ResetTriangle();
    void ResetRoundWire();

private:

    ComPtr<ID3D11Buffer> m_vertex_buffer_ptr_;                       // 顶点缓冲
    UINT m_vertex_count_uint_;                                         // 顶点数目
    Mode m_show_mode_;

    BasicEffect m_BasicEffect;								    // 对象渲染特效管理

};


#endif