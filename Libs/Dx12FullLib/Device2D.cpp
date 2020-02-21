#include "stdafx.h"

namespace basedx12 {

    Default2DDivece::Default2DDivece(UINT frameCount) :
        Dx12Device(frameCount)
    {
    }

    //初期化
    void Default2DDivece::OnInit()
    {
        LoadPipeline();
        LoadAssets();
    }

    // パイプラインの準備
    void Default2DDivece::LoadPipeline()
    {
        //ファクトリ
        ComPtr<IDXGIFactory4> factory = Dx12Factory::CreateDirect();
        //デバイス
        m_device = D3D12Device::CreateDefault(factory, m_useWarpDevice);
        //コマンドキュー
        m_commandQueue = CommandQueue::CreateDefault();
        //スワップチェーン
        m_swapChain = SwapChain::CreateDefault(m_commandQueue, m_FrameCount);
        // This sample does not support fullscreen transitions.
        ThrowIfFailed(factory->MakeWindowAssociation(App::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));
        m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
        // デスクプリタヒープ
        {
            // レンダリングターゲットビュー
            m_rtvHeap = DescriptorHeap::CreateRtvHeap(m_FrameCount);
            m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

            //CbvSrvデスクプリタヒープ(コンスタントバッファとシェーダリソース)
            m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
            m_CbvSrvDescriptorHandleIncrementSize
                = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
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
    void Default2DDivece::LoadAssets()
    {
        // ルートシグネチャー
        {
            //コンスタントバッファ付ルートシグネチャ
            m_rootSignature = RootSignature::CreateCbvSrvSmp();
        }
        // 頂点などのリソース構築用のコマンドリスト
        m_commandList = CommandList::CreateSimple(m_commandAllocators[m_frameIndex]);
        //シーンに各オブジェクトの構築を任せる
        App::GetSceneBase().OnInitAssets();
        //コマンドラインクローズおよびキューの実行
        CommandList::Close(m_commandList);
        CommandList::Excute(m_commandQueue, m_commandList);
        //同期オブジェクトおよびＧＰＵの処理待ち
        SyncAndWaitForGpu();
    }

    //更新
    void Default2DDivece::OnUpdate()
    {
        //シーンに更新を任せるので何もしない
    }

    // 描画処理
    void Default2DDivece::OnRender()
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
    void Default2DDivece::OnDestroy()
    {
        //GPUの処理待ち
        WaitForGpu();
        CloseHandle(m_fenceEvent);
    }

    // 描画のためのコマンドリストを集める
    void Default2DDivece::PopulateCommandList()
    {
        ThrowIfFailed(m_commandAllocators[m_frameIndex]->Reset());

        //コマンドリストのリセット（パイプライン指定なし）
        CommandList::Reset(m_commandAllocators[m_frameIndex], m_commandList);
        // Set necessary state.
        m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
        m_commandList->RSSetViewports(1, &m_viewport);
        m_commandList->RSSetScissorRects(1, &m_scissorRect);

        ID3D12DescriptorHeap* ppHeaps[] = { m_CbvSrvUavDescriptorHeap.Get(), m_SamplerDescriptorHeap.Get() };
        m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

        for (UINT i = 0; i < m_GPUDescriptorHandleVec.size(); i++) {
            m_commandList->SetGraphicsRootDescriptorTable(i, m_GPUDescriptorHandleVec[i]);
        }

        // バックバッファを使うためのバリア
        m_commandList->ResourceBarrier(
            1,
            &CD3DX12_RESOURCE_BARRIER::Transition(
                m_renderTargets[m_frameIndex].Get(),
                D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET
            )
        );

        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
        m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
        const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
        m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
        // シーンの個別描画
        App::GetSceneBase().OnRender();
        // フロントバッファに転送するためのバリア
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
