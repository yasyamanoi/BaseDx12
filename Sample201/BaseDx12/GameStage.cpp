#include "stdafx.h"
#include "GameStage.h"
#include "Scene.h"


namespace basedx12 {

    IMPLEMENT_DX12SHADER(VSPCSprite, App::GetShadersPath() + L"vshader.cso")
    IMPLEMENT_DX12SHADER(PSPCSprite, App::GetShadersPath() + L"pshader.cso")

    IMPLEMENT_DX12SHADER(VSPCConstSprite, App::GetShadersPath() + L"VsPCConst.cso")
    IMPLEMENT_DX12SHADER(PSPCConstSprite, App::GetShadersPath() + L"PsPCConst.cso")

    IMPLEMENT_DX12SHADER(VSPTSprite, App::GetShadersPath() + L"VSPTSprite.cso")
    IMPLEMENT_DX12SHADER(PSPTSprite, App::GetShadersPath() + L"PSPTSprite.cso")

    void FixedTriangle::OnInit() {
        auto Device = App::GetDx12Device();
        auto commandList = Device->GetCommandList();
        auto aspectRatio = Device->GetAspectRatio();

        D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
        //�R���X�^���g�o�b�t�@����
        m_pcPipelineState
            = PipelineState::CreateDefault2D<VertexPositionColor, VSPCSprite, PSPCSprite>(Device->GetRootSignature(), PipeLineDesc);
        vector<VertexPositionColor> vertex =
        {
            { Float3(0.0f, 0.25f * aspectRatio, 0.0f), Float4(1.0f, 0.0f, 0.0f, 1.0f) },
            { Float3(0.25f, -0.25f * aspectRatio, 0.0f), Float4(0.0f, 1.0f, 0.0f, 1.0f) },
            { Float3(-0.25f, -0.25f * aspectRatio, 0.0f), Float4(0.0f, 0.0f, 1.0f, 1.0f) }
        };
        //�O�p�`���b�V���쐬
        m_pcTriangleMesh = Dx12Mesh::CreateDx12Mesh<VertexPositionColor>(commandList, vertex);

    }

    void FixedTriangle::OnRender() {
        auto Device = App::GetDx12Device();
        auto commandList = Device->GetCommandList();
        commandList->SetPipelineState(m_pcPipelineState.Get());
        commandList->IASetVertexBuffers(0, 1, &m_pcTriangleMesh->GetVertexBufferView());
        commandList->DrawInstanced(3, 1, 0, 0);
    }


    void MoveTriangle::OnInit() {
        auto Device = App::GetDx12Device();
        auto commandList = Device->GetCommandList();
        auto aspectRatio = Device->GetAspectRatio();
        D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
        m_pcConstPipelineState
            = PipelineState::CreateDefault2D<VertexPositionColor, VSPCConstSprite, PSPCConstSprite>(Device->GetRootSignature(), PipeLineDesc);
        vector<VertexPositionColor> vertex =
        {
            { Float3(0.0f, 0.25f * aspectRatio, 0.0f), Float4(1.0f, 0.0f, 0.0f, 1.0f) },
            { Float3(0.25f, -0.25f * aspectRatio, 0.0f), Float4(0.0f, 1.0f, 0.0f, 1.0f) },
            { Float3(-0.25f, -0.25f * aspectRatio, 0.0f), Float4(0.0f, 0.0f, 1.0f, 1.0f) }
        };
        //�O�p�`���b�V���쐬
        m_pcTriangleMesh = Dx12Mesh::CreateDx12Mesh<VertexPositionColor>(commandList, vertex);

    }
    void MoveTriangle::OnUpdate() {
    }
    void MoveTriangle::OnRender() {
        auto Device = App::GetDx12Device();
        auto commandList = Device->GetCommandList();
        commandList->SetPipelineState(m_pcConstPipelineState.Get());
        commandList->IASetVertexBuffers(0, 1, &m_pcTriangleMesh->GetVertexBufferView());
        commandList->DrawInstanced(3, 1, 0, 0);
    }

    void MoveSquare::OnInit() {
        auto Device = App::GetDx12Device();
        auto commandList = Device->GetCommandList();
        auto aspectRatio = Device->GetAspectRatio();
        D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
        m_ptConstPipelineState
            = PipelineState::CreateDefault2D<VertexPositionTexture, VSPTSprite, PSPTSprite>(Device->GetRootSignature(), PipeLineDesc);
        //���b�V��
        {
            float HelfSize = 0.2f;
            //���_�z��
            vector<VertexPositionTexture> vertices = {
                { VertexPositionTexture(Float3(-HelfSize, HelfSize, 0),Float2(0.0f, 0.0f)) },
                { VertexPositionTexture(Float3(HelfSize, HelfSize, 0),	Float2(1.0f, 0.0f)) },
                { VertexPositionTexture(Float3(-HelfSize, -HelfSize, 0),	Float2(0.0f, 1.0f)) },
                { VertexPositionTexture(Float3(HelfSize, -HelfSize, 0),	Float2(1.0f, 1.0f)) },
            };
            //�C���f�b�N�X�z��
            vector<uint32_t> indices = { 0, 1, 2, 1, 3, 2 };
            //�l�p�`���b�V���̍쐬
            m_ptSquareMesh = Dx12Mesh::CreateDx12Mesh<VertexPositionTexture>(commandList, vertices, indices);
        }
        //�e�N�X�`��
        {
            auto TexFile = App::GetDataPath() + L"sky.jpg";
            //�e�N�X�`���̍쐬
            //�V�F�[�_���\�[�X�n���h�����쐬
            CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(
                Device->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
                1,
                Device->GetCbvSrvDescriptorHandleIncrementSize()
            );
            //�摜�t�@�C�������ƂɃe�N�X�`�����쐬
            m_SkyTexture = Dx12Texture::CreateDx12Texture(TexFile, srvHandle);
        }
        //�T���v���[
        {
            auto SamplerDescriptorHandle = Device->GetSamplerDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
            Sampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
        }
    }
    void MoveSquare::OnUpdate() {

    }
    void MoveSquare::OnRender() {
        auto Device = App::GetDx12Device();
        auto commandList = Device->GetCommandList();
        commandList->SetPipelineState(m_ptConstPipelineState.Get());
        m_SkyTexture->UpdateSRAndCreateSRV(commandList);
        commandList->IASetVertexBuffers(0, 1, &m_ptSquareMesh->GetVertexBufferView());
        commandList->IASetIndexBuffer(&m_ptSquareMesh->GetIndexBufferView());
        commandList->DrawIndexedInstanced(m_ptSquareMesh->GetNumIndices(), 1, 0, 0, 0);

    }


	void GameStage::OnInit() {

        //�����ŃV�[����̃I�u�W�F�N�g���\�z
        //�K�v�ȃp�C�v���C���I�u�W�F�N�g���f�o�C�X����擾
        auto Device = App::GetDx12Device();
        auto commandList = Device->GetCommandList();
        // ���ꂼ��̃I�u�W�F�N�g�̏�����
        {
            m_FixedTriangle.OnInit();
            m_MoveTriangle.OnInit();
            m_MoveSquare.OnInit();
        }
        //�R���X�^���g�o�b�t�@
        {
            //�R���X�^���g�o�b�t�@�n���h�����쐬
            //�O�p�`�l�p�`����
            CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
                Device->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
                0,
                0
            );
            m_ConstantBuffer = ConstantBuffer::CreateDirect(Handle, m_constantBufferData);
        }

	}

	void GameStage::OnUpdate() {
        const float translationSpeed = 0.005f;
        const float offsetBounds = 1.25f;
        m_constantBufferData.offset.x += translationSpeed;
        if (m_constantBufferData.offset.x > offsetBounds)
        {
            m_constantBufferData.offset.x = -offsetBounds;
        }
        m_ConstantBuffer->Copy(m_constantBufferData);
	}

	void GameStage::OnRender() {
        auto Device = App::GetDx12Device();
        auto commandList = Device->GetCommandList();
        commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_FixedTriangle.OnRender();
        m_MoveTriangle.OnRender();
        m_MoveSquare.OnRender();
	}



}
//end basedx12
