#include "stdafx.h"
#include "Scene.h"
#include "GameDevice.h"

namespace basedx12 {

    IMPLEMENT_DX12SHADER(VSPCSprite, App::GetShadersPath() + L"vshader.cso")
    IMPLEMENT_DX12SHADER(PSPCSprite, App::GetShadersPath() + L"pshader.cso")


    GameDivece::GameDivece() :
        Dx12Device(),
        m_frameIndex(0),
        m_viewport(0.0f, 0.0f, static_cast<float>(App::GetGameWidth()), static_cast<float>(App::GetGameHeight())),
        m_scissorRect(0, 0, static_cast<LONG>(App::GetGameWidth()), static_cast<LONG>(App::GetGameHeight())),
        m_fenceValues{},
        m_rtvDescriptorSize(0)
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
        }

        // フレームリーソース（フレームごとに作成する）
        {
            CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

            // Create a RTV and a command allocator for each frame.
            for (UINT n = 0; n < FrameCount; n++)
            {
                ThrowIfFailed(GetIDXGISwapChain3()->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
                GetID3D12Device()->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
                rtvHandle.Offset(1, m_rtvDescriptorSize);
                //コマンドアロケータ
                m_commandAllocators[n] = CommandAllocator::CreateDefault();
            }
        }
    }

    // 個別アセットの構築
    void GameDivece::LoadAssets()
    {
        // ルートシグネチャー
        {
            //一番シンプル
            m_rootSignature = RootSignature::CreateSimple();
        }
        // ２Ｄの基本的なパイプライン
        {
            D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
            m_pipelineState
                = PipelineState::CreateDefault2D<VertexPositionColor, VSPCSprite, PSPCSprite>(m_rootSignature, PipeLineDesc);
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
            //メッシュ作成
            m_Dx12Mesh = Dx12Mesh::CreateDx12Mesh<VertexPositionColor>(m_commandList, vertex);
        }

        //コマンドラインクローズおよびキューの実行
        ThrowIfFailed(m_commandList->Close());
        ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
        m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

        //同期オブジェクトおよびＧＰＵの処理待ち
        {
            ThrowIfFailed(GetID3D12Device()->CreateFence(m_fenceValues[m_frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
            m_fenceValues[m_frameIndex]++;

            // Create an event handle to use for frame synchronization.
            m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
            if (m_fenceEvent == nullptr)
            {
                ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
            }

            //GPUの処理待ち
            WaitForGpu();
        }
    }

    // Update frame-based values.
    void GameDivece::OnUpdate()
    {
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

//        ThrowIfFailed(m_commandList->Reset(m_commandAllocators[m_frameIndex].Get(), m_pipelineState.Get()));
        //コマンドリストのリセット
        CommandList::Reset(m_commandAllocators[m_frameIndex], m_commandList, m_pipelineState);


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
        m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_commandList->IASetVertexBuffers(0, 1, &m_Dx12Mesh->GetVertexBufferView());
        m_commandList->DrawInstanced(3, 1, 0, 0);

        // Indicate that the back buffer will now be used to present.
        m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

        ThrowIfFailed(m_commandList->Close());
    }

    // Wait for pending GPU work to complete.
    void GameDivece::WaitForGpu()
    {
        // Schedule a Signal command in the queue.
        ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), m_fenceValues[m_frameIndex]));

        // Wait until the fence has been processed.
        ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_frameIndex], m_fenceEvent));
        WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);

        // Increment the fence value for the current frame.
        m_fenceValues[m_frameIndex]++;
    }

    // Prepare to render the next frame.
    void GameDivece::MoveToNextFrame()
    {
        // Schedule a Signal command in the queue.
        const UINT64 currentFenceValue = m_fenceValues[m_frameIndex];
        ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), currentFenceValue));

        // Update the frame index.
        m_frameIndex = GetIDXGISwapChain3()->GetCurrentBackBufferIndex();

        // If the next frame is not ready to be rendered yet, wait until it is ready.
        if (m_fence->GetCompletedValue() < m_fenceValues[m_frameIndex])
        {
            ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_frameIndex], m_fenceEvent));
            WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);
        }

        // Set the fence value for the next frame.
        m_fenceValues[m_frameIndex] = currentFenceValue + 1;
    }


}
//end basedx12
