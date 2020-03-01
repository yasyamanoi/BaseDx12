#pragma once
#include "stdafx.h"

namespace basedx12 {

    DECLARE_DX12SHADER(VSPCSprite)
    DECLARE_DX12SHADER(PSPCSprite)

    DECLARE_DX12SHADER(VSPTSprite)
    DECLARE_DX12SHADER(PSPTSprite)




    //--------------------------------------------------------------------------------------
    ///	描画コンポーネント親
    //--------------------------------------------------------------------------------------
    class DrawComponent : public Component {
    protected:
        explicit DrawComponent(const shared_ptr<GameObject>& gameObjectPtr) :
            Component(gameObjectPtr) {}
        virtual ~DrawComponent() {}
        shared_ptr<Dx12Mesh> m_mesh;
        ComPtr<ID3D12PipelineState> m_pipelineState;
        //コンスタントバッファ
        shared_ptr<ConstantBuffer> m_constantBuffer;
        //コンスタントバッファのインデックス
        UINT m_constBuffIndex;
    public:
        void SetMesh(const shared_ptr<Dx12Mesh>& mesh) {
            m_mesh = mesh;
        }
    };

    //2Dコンスタントバッファ
    struct SpriteConstantBuffer
    {
        Mat4x4 World;
        Float4 Emissive;
        Float4 Diffuse;
        SpriteConstantBuffer() :
            World(),
            Emissive(0.0f),
            Diffuse(1.0f)
        {
        };
    };

    //--------------------------------------------------------------------------------------
    ///	2D描画コンポーネント
    //--------------------------------------------------------------------------------------
    class Draw2D : public DrawComponent {
    protected:
        //コンスタントバッファの実体
        SpriteConstantBuffer m_constantBufferData;
        explicit Draw2D(const shared_ptr<GameObject>& gameObjectPtr) :
            DrawComponent(gameObjectPtr)
        {}
        virtual ~Draw2D() {}
    public:
    };



	//--------------------------------------------------------------------------------------
	///	PC頂点Sprite描画コンポーネント
	//--------------------------------------------------------------------------------------
	class PCSpriteDraw : public Draw2D {
    public:
        explicit PCSpriteDraw(const shared_ptr<GameObject>& gameObjectPtr);
        virtual ~PCSpriteDraw();
        virtual void OnInit()override;
        virtual void OnUpdate()override;
        virtual void OnDraw()override;
        virtual void OnDestroy()override {}
    };

    //--------------------------------------------------------------------------------------
    ///	PT頂点Sprite描画コンポーネント
    //--------------------------------------------------------------------------------------
    class PTSpriteDraw : public Draw2D {
        //テクスチャ
        shared_ptr<Dx12Texture> m_texture;
        wstring m_textureFileName;
    public:
        explicit PTSpriteDraw(const shared_ptr<GameObject>& gameObjectPtr);
        virtual ~PTSpriteDraw();
        void SetTextureFile(const wstring& textureName);
        virtual void OnInit()override;
        virtual void OnUpdate()override {}
        virtual void OnDraw()override;
        virtual void OnDestroy()override {}
    };



}
//end basedx12
