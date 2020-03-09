#include "stdafx.h"
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
        //コンスタントバッファ無し
        m_pcPipelineState
            = PipelineState::CreateDefault2D<VertexPositionColor, VSPCSprite, PSPCSprite>(Device->GetRootSignature(), PipeLineDesc);
        vector<VertexPositionColor> vertex =
        {
            { Float3(0.0f, 0.25f * aspectRatio, 0.0f), Float4(1.0f, 0.0f, 0.0f, 1.0f) },
            { Float3(0.25f, -0.25f * aspectRatio, 0.0f), Float4(0.0f, 1.0f, 0.0f, 1.0f) },
            { Float3(-0.25f, -0.25f * aspectRatio, 0.0f), Float4(0.0f, 0.0f, 1.0f, 1.0f) }
        };
        //三角形メッシュ作成
        m_pcTriangleMesh = Dx12Mesh::CreateDx12Mesh<VertexPositionColor>(vertex);

    }

    void FixedTriangle::OnRender() {
        auto Device = App::GetDx12Device();
        auto commandList = Device->GetCommandList();
        //GraphicsRootDescriptorTableの設定はしなくてもよい（SRVもSamplerもCSVもシェーダに渡さないので）
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
            { Float3(0.0f, 0.25f * aspectRatio, 0.0f), Float4(0.0f, 1.0f, 0.0f, 1.0f) },
            { Float3(0.25f, -0.25f * aspectRatio, 0.0f), Float4(0.0f, 0.0f, 1.0f, 1.0f) },
            { Float3(-0.25f, -0.25f * aspectRatio, 0.0f), Float4(1.0f, 0.0f, 0.0f, 1.0f) }
        };
        //三角形メッシュ作成
        m_pcTriangleMesh = Dx12Mesh::CreateDx12Mesh<VertexPositionColor>(vertex);

        //コンスタントバッファハンドルを作成
        m_constBuffIndex = Device->GetCbvSrvUavNextIndex();
        CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
            Device->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
            m_constBuffIndex,
            Device->GetCbvSrvDescriptorHandleIncrementSize()
        );
        m_ConstantBuffer = ConstantBuffer::CreateDirect(Handle, m_constantBufferData);
    }
    void MoveTriangle::OnUpdate() {
        const float translationSpeed = 0.005f;
        const float offsetBounds = 1.25f;
        m_constantBufferData.offset.x += translationSpeed;
        if (m_constantBufferData.offset.x > offsetBounds)
        {
            m_constantBufferData.offset.x = -offsetBounds;
        }
        m_ConstantBuffer->Copy(m_constantBufferData);
    }
    void MoveTriangle::OnRender() {
        auto Device = App::GetDx12Device();
        auto commandList = Device->GetCommandList();
        //GraphicsRootDescriptorTableへセット。
        //Cbv
        CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
            Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
            m_constBuffIndex,
            Device->GetCbvSrvDescriptorHandleIncrementSize()
        );
        //csv。RootSignature上のIDは2番
        commandList->SetGraphicsRootDescriptorTable(2, CbvHandle);

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
        //メッシュ
        {
            float HelfSize = 0.2f;
            //頂点配列
            vector<VertexPositionTexture> vertices = {
                { VertexPositionTexture(Float3(-HelfSize, HelfSize, 0),	Float2(0.0f, 0.0f)) },
                { VertexPositionTexture(Float3(HelfSize, HelfSize, 0),	Float2(1.0f, 0.0f)) },
                { VertexPositionTexture(Float3(-HelfSize, -HelfSize, 0),	Float2(0.0f, 1.0f)) },
                { VertexPositionTexture(Float3(HelfSize, -HelfSize, 0),	Float2(1.0f, 1.0f)) },
            };
            //インデックス配列
            vector<uint32_t> indices = { 0, 1, 2, 1, 3, 2 };
            //四角形メッシュの作成
            m_ptSquareMesh = Dx12Mesh::CreateDx12Mesh<VertexPositionTexture>(vertices, indices);
        }
        //テクスチャ
        {
            auto TexFile = App::GetRelativeAssetsPath() + L"sky.jpg";
            //テクスチャの作成
            //シェーダリソースハンドルを作成
            m_srvIndex = Device->GetCbvSrvUavNextIndex();
            CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(
                Device->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
                m_srvIndex,
                Device->GetCbvSrvDescriptorHandleIncrementSize()
            );
            //画像ファイルをもとにテクスチャを作成
            m_SkyTexture = Dx12Texture::CreateDx12Texture(TexFile, srvHandle);
        }
        //サンプラー
        {
            auto SamplerDescriptorHandle = Device->GetSamplerDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
            Sampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
        }
        //コンスタントバッファハンドルを作成
        m_constBuffIndex = Device->GetCbvSrvUavNextIndex();
        CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
            Device->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
            m_constBuffIndex,
            Device->GetCbvSrvDescriptorHandleIncrementSize()
        );
        m_ConstantBuffer = ConstantBuffer::CreateDirect(Handle, m_constantBufferData);

    }
    void MoveSquare::OnUpdate() {
        const float translationSpeed = 0.01f;
        const float offsetBounds = 1.25f;
        m_constantBufferData.offset.x += translationSpeed;
        if (m_constantBufferData.offset.x > offsetBounds)
        {
            m_constantBufferData.offset.x = -offsetBounds;
        }
        m_ConstantBuffer->Copy(m_constantBufferData);
    }
    void MoveSquare::OnRender() {
        auto Device = App::GetDx12Device();
        auto commandList = Device->GetCommandList();

        //GraphicsRootDescriptorTableへセット。
        //Srv
        CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
            Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
            m_srvIndex,
            Device->GetCbvSrvDescriptorHandleIncrementSize()
        );
        //srv。RootSignature上のIDは0番
        commandList->SetGraphicsRootDescriptorTable(0, SrvHandle);
        //Sampler
        CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
            Device->GetSamplerDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
            0,
            0
        );
        //sampler。RootSignature上のIDは1番
        commandList->SetGraphicsRootDescriptorTable(1, SamplerHandle);
        //Cbv
        CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
            Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
            m_constBuffIndex,
            Device->GetCbvSrvDescriptorHandleIncrementSize()
        );
        //csv。RootSignature上のIDは2番
        commandList->SetGraphicsRootDescriptorTable(2, CbvHandle);

        commandList->SetPipelineState(m_ptConstPipelineState.Get());
        m_SkyTexture->UpdateSRAndCreateSRV(commandList);
        commandList->IASetVertexBuffers(0, 1, &m_ptSquareMesh->GetVertexBufferView());
        commandList->IASetIndexBuffer(&m_ptSquareMesh->GetIndexBufferView());
        commandList->DrawIndexedInstanced(m_ptSquareMesh->GetNumIndices(), 1, 0, 0, 0);

    }

	void Scene::OnInit() {
		ResetActiveDx12Device<GameDivece>(3);
	}
	void Scene::OnInitAssets() {

		//ここでシーン上のオブジェクトを構築
        //必要なパイプラインオブジェクトをデバイスから取得
        auto Device = App::GetDx12Device();
        auto commandList = Device->GetCommandList();
        // それぞれのオブジェクトの初期化
        {
            m_FixedTriangle.OnInit();
            m_MoveTriangle.OnInit();
            m_MoveSquare.OnInit();
        }
	}

	void Scene::OnUpdate() {
        m_MoveTriangle.OnUpdate();
        m_MoveSquare.OnUpdate();
    }
	void Scene::OnDraw() {
        //m_ConstantBuffer->Copy(m_constantBufferData);
        auto Device = App::GetDx12Device();
        auto commandList = Device->GetCommandList();
        commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_FixedTriangle.OnRender();
        m_MoveTriangle.OnRender();
        m_MoveSquare.OnRender();
	}
	void Scene::OnDestroy() {
	}

}
//end badedx12


