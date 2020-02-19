#include "stdafx.h"
#include "Scene.h"
#include "GameDevice.h"

namespace basedx12 {

    IMPLEMENT_DX12SHADER(VSPCSprite, App::GetShadersPath() + L"vshader.cso")
    IMPLEMENT_DX12SHADER(PSPCSprite, App::GetShadersPath() + L"pshader.cso")

    IMPLEMENT_DX12SHADER(VSPCConstSprite, App::GetShadersPath() + L"VsPCConst.cso")
    IMPLEMENT_DX12SHADER(PSPCConstSprite, App::GetShadersPath() + L"PsPCConst.cso")

    IMPLEMENT_DX12SHADER(VSPTSprite, App::GetShadersPath() + L"VSPTSprite.cso")
    IMPLEMENT_DX12SHADER(PSPTSprite, App::GetShadersPath() + L"PSPTSprite.cso")


    GameDivece::GameDivece() :
        Dx12Device()
    {
    }

    //初期化
    void GameDivece::OnInit()
    {
        LoadPipeline();
        LoadAssets();
    }

    // パイプラインの準備
    void GameDivece::LoadPipeline()
    {
//ファクトリ
        ComPtr<IDXGIFactory4> factory = Dx12Factory::CreateDirect();
        //デバイス(親クラスにある)
        SetID3D12Device(D3D12Device::CreateDefault(factory, m_useWarpDevice));
//コマンドキュー
        m_commandQueue = CommandQueue::CreateDefault();
        //スワップチェーン(親クラスにある)
        SetIDXGISwapChain3(SwapChain::CreateDefault(m_commandQueue, FrameCount));

        // This sample does not support fullscreen transitions.
        ThrowIfFailed(factory->MakeWindowAssociation(App::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));

        m_frameIndex = GetIDXGISwapChain3()->GetCurrentBackBufferIndex();

        // デスクプリタヒープ
        {
            // レンダリングターゲットビュー
            m_rtvHeap = DescriptorHeap::CreateRtvHeap(FrameCount);
            m_rtvDescriptorSize = GetID3D12Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

            //CbvSrvデスクプリタヒープ(コンスタントバッファとシェーダリソース)
            m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
            m_CbvSrvDescriptorHandleIncrementSize
                = GetID3D12Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
            //サンプラーデスクリプタヒープ
            m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);

            //GPU側デスクプリタヒープのハンドルの配列の作成
            m_GPUDescriptorHandleVec.clear();
            //Cbv
            CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
                m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
                0,
                0
            );
            m_GPUDescriptorHandleVec.push_back(SrvHandle);

            //Srv
            CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
                m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
                1,
                m_CbvSrvDescriptorHandleIncrementSize
            );
            m_GPUDescriptorHandleVec.push_back(CbvHandle);

            //Sampler
            CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
                m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
                0,
                0
            );
            m_GPUDescriptorHandleVec.push_back(SamplerHandle);

        }

        // RTVとコマンドアロケータ
        CreateRTVandCmdAllocators();
    }

    // 個別アセットの構築
    void GameDivece::LoadAssets()
    {
        // ルートシグネチャー
        {
            //コンスタントバッファ付ルートシグネチャ
            m_rootSignature = RootSignature::CreateCbvSrvSmp();
        }
        // ２Ｄのパイプライン
        {
            D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
            //コンスタントバッファ無し
            m_pcPipelineState
                = PipelineState::CreateDefault2D<VertexPositionColor, VSPCSprite, PSPCSprite>(m_rootSignature, PipeLineDesc);

            m_pcConstPipelineState
                = PipelineState::CreateDefault2D<VertexPositionColor, VSPCConstSprite, PSPCConstSprite>(m_rootSignature, PipeLineDesc);

            m_ptConstPipelineState
                = PipelineState::CreateDefault2D<VertexPositionTexture, VSPTSprite, PSPTSprite>(m_rootSignature, PipeLineDesc);

        }
        // 頂点などのリソース構築用のコマンドリスト
        m_commandList = CommandList::CreateSimple(m_commandAllocators[m_frameIndex]);
        // メッシュ
        {
            vector<VertexPositionColor> vertex =
            {
                { Vec3( 0.0f, 0.25f * m_aspectRatio, 0.0f ), Vec4(1.0f, 0.0f, 0.0f, 1.0f ) },
                { Vec3(0.25f, -0.25f * m_aspectRatio, 0.0f ), Vec4(0.0f, 1.0f, 0.0f, 1.0f ) },
                { Vec3(-0.25f, -0.25f * m_aspectRatio, 0.0f ), Vec4(0.0f, 0.0f, 1.0f, 1.0f ) }
            };
            //三角形メッシュ作成
            m_pcTriangleMesh = Dx12Mesh::CreateDx12Mesh<VertexPositionColor>(m_commandList, vertex);
            float HelfSize = 0.1f;
            //頂点配列
            vector<VertexPositionTexture> vertices = {
                { VertexPositionTexture(Vec3(-HelfSize, HelfSize, 0),	Vec2(0.0f, 0.0f)) },
                { VertexPositionTexture(Vec3(HelfSize, HelfSize, 0),	Vec2(1.0f, 0.0f)) },
                { VertexPositionTexture(Vec3(-HelfSize, -HelfSize, 0),	Vec2(0.0f, 1.0f)) },
                { VertexPositionTexture(Vec3(HelfSize, -HelfSize, 0),	Vec2(1.0f, 1.0f)) },
            };
            //インデックス配列
            vector<uint32_t> indices = { 0, 1, 2, 1, 3, 2 };
            //四角形メッシュの作成
            m_pntSquareMesh = Dx12Mesh::CreateDx12Mesh<VertexPositionTexture>(m_commandList, vertices, indices);

        }
        //コンスタントバッファ
        {
            //コンスタントバッファハンドルを作成
            //三角形四角形共通
            CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
                m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
                0,
                0
            );
            m_ConstantBuffer = ConstantBuffer::CreateDirect(Handle, m_constantBufferData);
        }
        //テクスチャ
        {
            auto TexFile = App::GetDataPath() + L"sky.jpg";
            //テクスチャの作成
            //シェーダリソースハンドルを作成
            CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(
                m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
                1,
                m_CbvSrvDescriptorHandleIncrementSize
            );
            //画像ファイルをもとにテクスチャを作成
            m_SkyTexture = Dx12Texture::CreateDx12Texture(TexFile, srvHandle);
        }
        //サンプラー
        {
            auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
            Sampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
        }
        //コマンドラインクローズおよびキューの実行
        CommandList::Close(m_commandList);
        CommandList::Excute(m_commandQueue, m_commandList);
        //同期オブジェクトおよびＧＰＵの処理待ち
        SyncAndWaitForGpu();
    }

    // Update frame-based values.
    void GameDivece::OnUpdate()
    {
        const float translationSpeed = 0.005f;
        const float offsetBounds = 1.25f;

        m_constantBufferData.offset.x += translationSpeed;
        if (m_constantBufferData.offset.x > offsetBounds)
        {
            m_constantBufferData.offset.x = -offsetBounds;
        }
        m_ConstantBuffer->Copy(m_constantBufferData);
    }

    // 描画処理
    void GameDivece::OnRender()
    {
        // 描画のためのコマンドリストを集める
        PopulateCommandList();

        // 描画用コマンドリスト実行
        ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
        m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

        // フロントバッファに表示
        ThrowIfFailed(GetIDXGISwapChain3()->Present(1, 0));

        //次のフレームに移動
        MoveToNextFrame();
    }
    //後始末
    void GameDivece::OnDestroy()
    {
        //GPUの処理待ち
        WaitForGpu();
        CloseHandle(m_fenceEvent);
    }

    // 描画のためのコマンドリストを集める
    void GameDivece::PopulateCommandList()
    {
        ThrowIfFailed(m_commandAllocators[m_frameIndex]->Reset());

        //コマンドリストのリセット
        CommandList::Reset(m_commandAllocators[m_frameIndex], m_commandList, m_pcPipelineState);
        // Set necessary state.
        m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
        m_commandList->RSSetViewports(1, &m_viewport);
        m_commandList->RSSetScissorRects(1, &m_scissorRect);

        ID3D12DescriptorHeap* ppHeaps[] = { m_CbvSrvUavDescriptorHeap.Get(), m_SamplerDescriptorHeap.Get() };
        m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

        for (UINT i = 0; i < m_GPUDescriptorHandleVec.size(); i++) {
            m_commandList->SetGraphicsRootDescriptorTable(i, m_GPUDescriptorHandleVec[i]);
        }

        // Indicate that the back buffer will be used as a render target.
        m_commandList->ResourceBarrier(
            1,
            &CD3DX12_RESOURCE_BARRIER::Transition(
                m_renderTargets[m_frameIndex].Get(),
                D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET
            )
        );

        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
        m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

        // Record commands.
        const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
        m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
        m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_commandList->IASetVertexBuffers(0, 1, &m_pcTriangleMesh->GetVertexBufferView());
        m_commandList->DrawInstanced(3, 1, 0, 0);
        m_commandList->SetPipelineState(m_pcConstPipelineState.Get());
        m_commandList->DrawInstanced(3, 1, 0, 0);
        m_commandList->SetPipelineState(m_ptConstPipelineState.Get());
        m_SkyTexture->UpdateSRAndCreateSRV(m_commandList);
        m_commandList->IASetVertexBuffers(0, 1, &m_pntSquareMesh->GetVertexBufferView());
        m_commandList->IASetIndexBuffer(&m_pntSquareMesh->GetIndexBufferView());
        m_commandList->DrawIndexedInstanced(m_pntSquareMesh->GetNumIndices(), 1, 0, 0, 0);

        // Indicate that the back buffer will now be used to present.
        m_commandList->ResourceBarrier(1, 
            &CD3DX12_RESOURCE_BARRIER::Transition(
                m_renderTargets[m_frameIndex].Get(),
                D3D12_RESOURCE_STATE_RENDER_TARGET, 
                D3D12_RESOURCE_STATE_PRESENT
            )
        );

        ThrowIfFailed(m_commandList->Close());
    }



}
//end basedx12
