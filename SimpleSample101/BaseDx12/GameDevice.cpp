#include "stdafx.h"
#include "Scene.h"
#include "GameDevice.h"

namespace basedx12 {


    GameDivece::GameDivece(UINT frameCount) :
        Dx12Device(frameCount)
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
        m_device = D3D12Device::CreateDefault(factory, m_useWarpDevice);
//コマンドキュー
        m_commandQueue = CommandQueue::CreateDefault();
        //スワップチェーン(親クラスにある)
        m_swapChain = SwapChain::CreateDefault(m_commandQueue, m_FrameCount);

        // This sample does not support fullscreen transitions.
        ThrowIfFailed(factory->MakeWindowAssociation(App::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));

        m_frameIndex = GetIDXGISwapChain3()->GetCurrentBackBufferIndex();

        // デスクプリタヒープ
        {
            // レンダリングターゲットビュー
            m_rtvHeap = DescriptorHeap::CreateRtvHeap(m_FrameCount);
            m_rtvDescriptorSize = GetID3D12Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        }
        // RTVとコマンドアロケータ
        CreateRTVandCmdAllocators();
    }

    // 個別アセットの構築
    void GameDivece::LoadAssets()
    {
        // ルートシグネチャー
        {
            //一番シンプル
            m_rootSignature = RootSignature::CreateSimple();
        }
        // 頂点などのリソース構築用のコマンドリスト
        m_commandList = CommandList::CreateSimple(m_commandAllocators[m_frameIndex]);
        //シーンに各オブジェクトの構築を任せる
        App::GetSceneBase().OnInitAssets();
        //コマンドラインクローズおよびキューの実行
        ThrowIfFailed(m_commandList->Close());
        ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
        m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

        //同期オブジェクトおよびＧＰＵの処理待ち
        SyncAndWaitForGpu();
    }

    // Update frame-based values.
    void GameDivece::OnUpdate()
    {
        App::GetSceneBase().OnUpdate();
    }

    // 描画処理
    void GameDivece::OnDraw()
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

        //コマンドリストのリセット（パイプライン指定なし）
        CommandList::Reset(m_commandAllocators[m_frameIndex], m_commandList);
        // Set necessary state.
        m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
        m_commandList->RSSetViewports(1, &m_viewport);
        m_commandList->RSSetScissorRects(1, &m_scissorRect);
        // Indicate that the back buffer will be used as a render target.
        m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
        m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

        // Record commands.
        const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
        m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
        // シーンに個別描画を任せる
        App::GetSceneBase().OnDraw();
        // Indicate that the back buffer will now be used to present.
        m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

        ThrowIfFailed(m_commandList->Close());
    }


}
//end basedx12
