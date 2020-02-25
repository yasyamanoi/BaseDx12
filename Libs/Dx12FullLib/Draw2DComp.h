#pragma once
#include "stdafx.h"

namespace basedx12 {

    DECLARE_DX12SHADER(VSPCSprite)
    DECLARE_DX12SHADER(PSPCSprite)

    DECLARE_DX12SHADER(VSPCConstSprite)
    DECLARE_DX12SHADER(PSPCConstSprite)

    DECLARE_DX12SHADER(VSPTSprite)
    DECLARE_DX12SHADER(PSPTSprite)

    struct SpriteConstantBuffer
    {
        XMFLOAT4 offset;
        SpriteConstantBuffer() :
            offset(0.0f, 0, 0, 0)
        {}
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
        void Move(float speed);
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
        void Move(float speed);
        virtual void OnInit()override;
        virtual void OnUpdate()override {}
        virtual void OnDraw()override;
        virtual void OnDestroy()override {}
    };



}
//end basedx12
