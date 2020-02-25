#include "stdafx.h"

namespace basedx12 {

    IMPLEMENT_DX12SHADER(VSPCSprite, App::GetShadersPath() + L"vshader.cso")
    IMPLEMENT_DX12SHADER(PSPCSprite, App::GetShadersPath() + L"pshader.cso")

    IMPLEMENT_DX12SHADER(VSPCConstSprite, App::GetShadersPath() + L"VsPCConst.cso")
    IMPLEMENT_DX12SHADER(PSPCConstSprite, App::GetShadersPath() + L"PsPCConst.cso")

    IMPLEMENT_DX12SHADER(VSPTSprite, App::GetShadersPath() + L"VSPTSprite.cso")
    IMPLEMENT_DX12SHADER(PSPTSprite, App::GetShadersPath() + L"PSPTSprite.cso")

    //--------------------------------------------------------------------------------------
    ///	描画コンポーネント親
    //--------------------------------------------------------------------------------------


    //--------------------------------------------------------------------------------------
    ///	PC頂点Sprite描画コンポーネント
    //--------------------------------------------------------------------------------------
    PCSpriteDraw::PCSpriteDraw(const shared_ptr<GameObject>& gameObjectPtr) :
        DrawComponent(gameObjectPtr)
	{}
    PCSpriteDraw::~PCSpriteDraw() {}

    void PCSpriteDraw::OnInit() {
        auto Device = App::GetDx12Device();
        auto commandList = Device->GetCommandList();
        auto aspectRatio = Device->GetAspectRatio();
        D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
        //パイプライステート
        m_pipelineState
            = PipelineState::CreateDefault2D<VertexPositionColor, VSPCConstSprite, PSPCConstSprite>(Device->GetRootSignature(), PipeLineDesc);
        //コンスタントバッファ
        //コンスタントバッファハンドルを作成
        m_consrBuffIndex = Device->GetConstBuffNextIndex();
        CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
            Device->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
            m_consrBuffIndex,
            Device->GetCbvSrvDescriptorHandleIncrementSize()
        );
        m_constantBuffer = ConstantBuffer::CreateDirect(Handle, m_constantBufferData);
    }

    void PCSpriteDraw::Move(float speed) {
        float translationSpeed = speed;
        const float offsetBounds = 1.25f;
        m_constantBufferData.offset.x += translationSpeed;
        if (m_constantBufferData.offset.x > offsetBounds)
        {
            m_constantBufferData.offset.x = -offsetBounds;
        }
        m_constantBuffer->Copy(m_constantBufferData);
    }


    void PCSpriteDraw::OnUpdate() {
       //const float translationSpeed = 0.005f;
       // const float offsetBounds = 1.25f;
       // m_constantBufferData.offset.x += translationSpeed;
       // if (m_constantBufferData.offset.x > offsetBounds)
       // {
       //     m_constantBufferData.offset.x = -offsetBounds;
       // }
    }

    void PCSpriteDraw::OnDraw() {
        auto Device = App::GetDx12Device();
        //auto frameidx = Device->GetFrameIndex();
        //auto Allocator = Device->GetCommandAllocators();
        //ComPtr<ID3D12GraphicsCommandList> commandList;
        //CommandList::Reset(Allocator[frameidx], commandList);


        auto commandList = Device->GetCommandList();

        

        //Srv
        CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
            Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
            0,
            0
        );
        //srv
        commandList->SetGraphicsRootDescriptorTable(0, SrvHandle);
        //Sampler
        CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
            Device->GetSamplerDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
            0,
            0
        );
        //sampler
        commandList->SetGraphicsRootDescriptorTable(1, SamplerHandle);
        //Cbv
        CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
            Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
            m_consrBuffIndex,
            Device->GetCbvSrvDescriptorHandleIncrementSize()
        );
        //csv
        commandList->SetGraphicsRootDescriptorTable(2, CbvHandle);



        commandList->SetPipelineState(m_pipelineState.Get());
        commandList->IASetVertexBuffers(0, 1, &m_mesh->GetVertexBufferView());
        commandList->DrawInstanced(3, 1, 0, 0);

   //     ThrowIfFailed(commandList->Close());


    }

    //--------------------------------------------------------------------------------------
    ///	PT頂点Sprite描画コンポーネント
    //--------------------------------------------------------------------------------------
    PTSpriteDraw::PTSpriteDraw(const shared_ptr<GameObject>& gameObjectPtr) :
        DrawComponent(gameObjectPtr)
    {}
    PTSpriteDraw::~PTSpriteDraw() {}

    void PTSpriteDraw::OnInit() {
        auto Device = App::GetDx12Device();
        auto commandList = Device->GetCommandList();
        auto aspectRatio = Device->GetAspectRatio();
        D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
        //パイプライステート
        m_pipelineState
            = PipelineState::CreateDefault2D<VertexPositionTexture, VSPTSprite, PSPTSprite>(Device->GetRootSignature(), PipeLineDesc);
        //コンスタントバッファ
        //コンスタントバッファハンドルを作成
        m_consrBuffIndex = Device->GetConstBuffNextIndex();
        CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
            Device->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
            m_consrBuffIndex,
            Device->GetCbvSrvDescriptorHandleIncrementSize()
        );
        m_constantBuffer = ConstantBuffer::CreateDirect(Handle, m_constantBufferData);

        auto SamplerDescriptorHandle = Device->GetSamplerDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
        Sampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);

    }

    void PTSpriteDraw::SetTextureFile(const wstring& textureName) {
        m_textureFileName = textureName;
        auto Device = App::GetDx12Device();
        //テクスチャの作成
        //シェーダリソースハンドルを作成
        CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(
            Device->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
            0,
            0
        );
        //画像ファイルをもとにテクスチャを作成
        m_texture = Dx12Texture::CreateDx12Texture(m_textureFileName, srvHandle);
    }

    void PTSpriteDraw::Move(float speed) {
        float translationSpeed = speed;
        const float offsetBounds = 1.25f;
        m_constantBufferData.offset.x += translationSpeed;
        if (m_constantBufferData.offset.x > offsetBounds)
        {
            m_constantBufferData.offset.x = -offsetBounds;
        }
        m_constantBuffer->Copy(m_constantBufferData);
    }

    void PTSpriteDraw::OnDraw() {
        auto Device = App::GetDx12Device();
        auto commandList = Device->GetCommandList();
        //Srv
        CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
            Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
            0,
            0
        );
        //srv
        commandList->SetGraphicsRootDescriptorTable(0, SrvHandle);
        //Sampler
        CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
            Device->GetSamplerDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
            0,
            0
        );
        //sampler
        commandList->SetGraphicsRootDescriptorTable(1, SamplerHandle);
        //Cbv
        CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
            Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
            m_consrBuffIndex,
            Device->GetCbvSrvDescriptorHandleIncrementSize()
        );
        //csv
        commandList->SetGraphicsRootDescriptorTable(2, CbvHandle);
        commandList->SetPipelineState(m_pipelineState.Get());

        m_texture->UpdateSRAndCreateSRV(commandList);
        commandList->IASetVertexBuffers(0, 1, &m_mesh->GetVertexBufferView());
        commandList->IASetIndexBuffer(&m_mesh->GetIndexBufferView());
        commandList->DrawIndexedInstanced(m_mesh->GetNumIndices(), 1, 0, 0, 0);

    }



}
//end basedx12
