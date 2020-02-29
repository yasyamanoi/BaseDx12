#pragma once
#include "stdafx.h"

namespace basedx12 {

    DECLARE_DX12SHADER(VSPCSprite)
    DECLARE_DX12SHADER(PSPCSprite)

    DECLARE_DX12SHADER(VSPTSprite)
    DECLARE_DX12SHADER(PSPTSprite)


    //�R���X�^���g�o�b�t�@
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
    ///	�`��R���|�[�l���g�e
    //--------------------------------------------------------------------------------------
    class DrawComponent : public Component {
    protected:
        explicit DrawComponent(const shared_ptr<GameObject>& gameObjectPtr) :
            Component(gameObjectPtr) {}
        virtual ~DrawComponent() {}
        shared_ptr<Dx12Mesh> m_mesh;
        ComPtr<ID3D12PipelineState> m_pipelineState;
        //�R���X�^���g�o�b�t�@�̎���
        SpriteConstantBuffer m_constantBufferData;
        //�R���X�^���g�o�b�t�@
        shared_ptr<ConstantBuffer> m_constantBuffer;
        //�R���X�^���g�o�b�t�@�̃C���f�b�N�X
        UINT m_consrBuffIndex;
    public:
        void SetMesh(const shared_ptr<Dx12Mesh>& mesh) {
            m_mesh = mesh;
        }
    };


	//--------------------------------------------------------------------------------------
	///	PC���_Sprite�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PCSpriteDraw : public DrawComponent {
    public:
        explicit PCSpriteDraw(const shared_ptr<GameObject>& gameObjectPtr);
        virtual ~PCSpriteDraw();
        virtual void OnInit()override;
        virtual void OnUpdate()override;
        virtual void OnDraw()override;
        virtual void OnDestroy()override {}
    };

    //--------------------------------------------------------------------------------------
    ///	PT���_Sprite�`��R���|�[�l���g
    //--------------------------------------------------------------------------------------
    class PTSpriteDraw : public DrawComponent {
        //�e�N�X�`��
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
