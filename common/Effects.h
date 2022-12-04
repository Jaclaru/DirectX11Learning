//***************************************************************************************
// Effects.h by X_Jun(MKXJun) (C) 2018-2022 All Rights Reserved.
// Licensed under the MIT License.
//
// ������Ч������
// Simple effect management framework.
//***************************************************************************************

#ifndef EFFECTS_H
#define EFFECTS_H

#include <memory>
#include <LightHelper.h>
#include <RenderStates.h>

class IEffect
{
public:
    // ʹ��ģ�����(C++11)��������
    template <class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;

    IEffect() = default;
    virtual ~IEffect() = default;
    // ���������������ƶ�
    IEffect(const IEffect&) = delete;
    IEffect& operator=(const IEffect&) = delete;
    IEffect(IEffect&&) = default;
    IEffect& operator=(IEffect&&) = default;

    // ���²��󶨳���������
    virtual void Apply(ID3D11DeviceContext* deviceContext) = 0;
};


class BasicEffect : public IEffect
{
public:

    BasicEffect();
    virtual ~BasicEffect() override;

    BasicEffect(BasicEffect&& moveFrom) noexcept;
    BasicEffect& operator=(BasicEffect&& moveFrom) noexcept;

    // ��ȡ����
    static BasicEffect& Get();



    // ��ʼ��������Դ
    bool InitAll(ID3D11Device* device);


    //
    // ��Ⱦģʽ�ı��
    //

    // Ĭ��״̬������
    void SetRenderDefault(ID3D11DeviceContext* deviceContext);
    // Alpha��ϻ���
    void SetRenderAlphaBlend(ID3D11DeviceContext* deviceContext);
    // �޶��λ��
    void SetRenderNoDoubleBlend(ID3D11DeviceContext* deviceContext, UINT stencilRef);
    // ��д��ģ��ֵ
    void SetWriteStencilOnly(ID3D11DeviceContext* deviceContext, UINT stencilRef);
    // ��ָ��ģ��ֵ��������л��ƣ�����Ĭ��״̬
    void SetRenderDefaultWithStencil(ID3D11DeviceContext* deviceContext, UINT stencilRef);
    // ��ָ��ģ��ֵ��������л��ƣ�����Alpha���
    void SetRenderAlphaBlendWithStencil(ID3D11DeviceContext* deviceContext, UINT stencilRef);
    // 2DĬ��״̬����
    void Set2DRenderDefault(ID3D11DeviceContext* deviceContext);
    // 2D��ϻ���
    void Set2DRenderAlphaBlend(ID3D11DeviceContext* deviceContext);



    //
    // ��������
    //

    void XM_CALLCONV SetWorldMatrix(DirectX::FXMMATRIX W);
    void XM_CALLCONV SetViewMatrix(DirectX::FXMMATRIX V);
    void XM_CALLCONV SetProjMatrix(DirectX::FXMMATRIX P);

    void XM_CALLCONV SetReflectionMatrix(DirectX::FXMMATRIX R);
    void XM_CALLCONV SetShadowMatrix(DirectX::FXMMATRIX S);
    void XM_CALLCONV SetRefShadowMatrix(DirectX::FXMMATRIX RefS);

    //
    // ���ա����ʺ������������
    //

    // �������͵ƹ�����������Ŀ
    static const int maxLights = 5;

    void SetDirLight(size_t pos, const DirectionalLight& dirLight);
    void SetPointLight(size_t pos, const PointLight& pointLight);
    void SetSpotLight(size_t pos, const SpotLight& spotLight);

    void SetMaterial(const Material& material);

    void SetTexture(ID3D11ShaderResourceView* texture);

    void SetEyePos(const DirectX::XMFLOAT3& eyePos);



    //
    // ״̬��������
    //

    void SetReflectionState(bool isOn);
    void SetShadowState(bool isOn);


    // Ӧ�ó�����������������Դ�ı��
    void Apply(ID3D11DeviceContext* deviceContext) override;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};










#endif