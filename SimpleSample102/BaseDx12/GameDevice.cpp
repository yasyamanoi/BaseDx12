#include "stdafx.h"
#include "Scene.h"
#include "GameDevice.h"

namespace basedx12 {


	GameDevice::GameDevice(UINT frameCount) :
		BaseDevice(frameCount)
	{
	}

	//初期化
	void GameDevice::OnInit()
	{
		LoadPipeline();
		LoadAssets();
	}

	// パイプラインの準備
	void GameDevice::LoadPipeline()
	{
		//ファクトリ
		ComPtr<IDXGIFactory4> factory = Dx12Factory::CreateDirect();
		//デバイス
		m_device = D3D12Device::CreateDefault(factory, m_useWarpDevice);
		//コマンドキュー
		m_commandQueue = CommandQueue::CreateDefault();
		//スワップチェーン
		m_swapChain = SwapChain::CreateDefault(factory, m_commandQueue, m_frameCount);
		m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
		// デスクプリタヒープ
		{
			// レンダリングターゲットビュー
			m_rtvHeap = DescriptorHeap::CreateRtvHeap(m_frameCount);
			m_rtvDescriptorHandleIncrementSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			//CbvSrvUavデスクプリタヒープ
			//(コンスタントバッファとシェーダリソースと順序不定のアクセスビュー)
			//CbvSrvUavデスクプリタヒープの数はGetCbvSrvUavMax()により取得する
			m_cbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(GetCbvSrvUavMax());
			m_cbvSrvUavDescriptorHandleIncrementSize
			= m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			//サンプラーデスクリプタヒープ
			m_samplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
		}
		// RTVとコマンドアロケータ
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
		for (UINT n = 0; n < m_frameCount; n++)
		{
			ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
			m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
			rtvHandle.Offset(1, m_rtvDescriptorHandleIncrementSize);
			//コマンドアロケータ
			m_commandAllocators[n] = CommandAllocator::CreateDefault();
		}
	}

	// 個別アセットの構築
	void GameDevice::LoadAssets()
	{
		// ルートシグネチャー
		{
			//SrvとSmpとCbv付ルートシグネチャ
			m_rootSignature = RootSignature::CreateSrvSmpCbv();
		}
		// 頂点などのリソース構築用のコマンドリスト
		m_commandList = CommandList::CreateSimple(m_commandAllocators[m_frameIndex]);
		//シーンに各オブジェクトの構築を任せる
		App::GetSceneBase()->OnInitAssets();
		//コマンドラインクローズおよびキューの実行
		CommandList::Close(m_commandList);
		CommandList::Excute(m_commandQueue, m_commandList);
		//同期オブジェクトおよびＧＰＵの処理待ち
		SyncAndWaitForGpu();
	}

	//更新
	void GameDevice::OnUpdate()
	{
		//シーンに更新を任せる
		App::GetSceneBase()->OnUpdate();
	}

	// 描画処理
	void GameDevice::OnDraw()
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
	void GameDevice::OnDestroy()
	{
		//GPUの処理待ち
		WaitForGpu();
		CloseHandle(m_fenceEvent);
	}

	// 描画のためのコマンドリストを集める
	void GameDevice::PopulateCommandList()
	{
		ThrowIfFailed(m_commandAllocators[m_frameIndex]->Reset());
		//コマンドリストのリセット（パイプライン指定なし）
		CommandList::Reset(m_commandAllocators[m_frameIndex], m_commandList);
		// Set necessary state.
		m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
		m_commandList->RSSetViewports(1, &m_viewport);
		m_commandList->RSSetScissorRects(1, &m_scissorRect);
		// バックバッファを使うためのバリア
		m_commandList->ResourceBarrier(
			1,
			&CD3DX12_RESOURCE_BARRIER::Transition(
				m_renderTargets[m_frameIndex].Get(),
				D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET
			)
		);
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
			m_rtvHeap->GetCPUDescriptorHandleForHeapStart(),
			m_frameIndex,
			m_rtvDescriptorHandleIncrementSize
		);
		m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
		const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		ID3D12DescriptorHeap* ppHeaps[] = {
			m_cbvSrvUavDescriptorHeap.Get(),
			m_samplerDescriptorHeap.Get() 
		};
		m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		// シーンの個別描画
		App::GetSceneBase()->OnDraw();
		// フロントバッファに転送するためのバリア
		m_commandList->ResourceBarrier(
			1, 
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
