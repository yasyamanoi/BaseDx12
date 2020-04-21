#include "stdafx.h"
#include "Scene.h"
#include "GameDevice.h"

namespace basedx12 {


	GameDevice::GameDevice(UINT frameCount) :
		BaseDevice(frameCount)
	{
		m_Is3DDevice = true;
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
		//フレームインデックスの初期値
		m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
		// デスクプリタヒープ
		{
			// レンダリングターゲットビュー
			m_rtvHeap = DescriptorHeap::CreateRtvHeap(m_frameCount);
			m_rtvDescriptorHandleIncrementSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			//デプスステンシルビュー
			m_dsvHeap = DescriptorHeap::CreateDsvHeap(1);
			m_dsvDescriptorHandleIncrementSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
			//Dsvリソース
			m_depthStencil = DepthStencil::CreateDefault(m_dsvHeap);
			//シャドウマップ用デプスステンシル
			m_shadowDsvHeap = DescriptorHeap::CreateDsvHeap(1);
			m_shadowDepthStencil = DepthStencil::CreateShadowmap(m_shadowDsvHeap,App::GetGameWidth(), App::GetGameHeight());
			//CbvSrvUavデスクプリタヒープ(コンスタントバッファとシェーダリソース)
			m_cbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(GetCbvSrvUavMax());
			m_cbvSrvUavDescriptorHandleIncrementSize
				= m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			//サンプラーデスクリプタヒープ通常と影用
			m_samplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(2);
			m_samplerDescriptorHandleIncrementSize
				= m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
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
			m_shadowmapCommandAllocators[n] = CommandAllocator::CreateDefault();
		}
		//シャドウ描画用のシェーダリソースの作成（オブジェクト間共有）
		{
			m_ShadowSRVIndex = GetCbvSrvUavNextIndex();
			CD3DX12_CPU_DESCRIPTOR_HANDLE ShadowHandle(
				m_cbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				m_ShadowSRVIndex,
				m_cbvSrvUavDescriptorHandleIncrementSize
			);
			D3D12_SHADER_RESOURCE_VIEW_DESC shadowSrvDesc = {};
			shadowSrvDesc.Format = DXGI_FORMAT_R32_FLOAT;
			shadowSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			shadowSrvDesc.Texture2D.MipLevels = 1;
			shadowSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			m_device->CreateShaderResourceView(m_shadowDepthStencil.Get(), &shadowSrvDesc, ShadowHandle);
		}
	}

	// 個別アセットの構築
	void GameDevice::LoadAssets()
	{
		// ルートシグネチャー
		{
			//コンスタントバッファ付ルートシグネチャ
			m_rootSignature = RootSignature::CreateSrv2Smp2Cbv();
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
	void GameDevice::OnUpdate()
	{
		//シーンに更新を任せる
		App::GetSceneBase().OnUpdate();
	}

	// 描画処理
	void GameDevice::OnDraw()
	{
		// 描画のためのコマンドリストを集める
		PopulateCommandList();
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
		//シャドウマップコマンドアロケータのリセット
		ThrowIfFailed(m_shadowmapCommandAllocators[m_frameIndex]->Reset());
		//コマンドリストのリセット（パイプライン指定なし）
		CommandList::Reset(m_shadowmapCommandAllocators[m_frameIndex], m_commandList);
		// Set necessary state.
		m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
		m_commandList->RSSetViewports(1, &m_viewport);
		m_commandList->RSSetScissorRects(1, &m_scissorRect);
		//ディスクリプタヒープの登録
		ID3D12DescriptorHeap* ppShadowHeaps[] = { m_cbvSrvUavDescriptorHeap.Get() };
		m_commandList->SetDescriptorHeaps(_countof(ppShadowHeaps), ppShadowHeaps);
		//シャドウマップの描画
		m_commandList->ResourceBarrier(
			1, 
			&CD3DX12_RESOURCE_BARRIER::Transition(
				m_shadowDepthStencil.Get(),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				D3D12_RESOURCE_STATE_DEPTH_WRITE
			)
		);
		CD3DX12_CPU_DESCRIPTOR_HANDLE shadowDsvHandle(m_shadowDsvHeap->GetCPUDescriptorHandleForHeapStart());
		//ハンドルをセット
		m_commandList->OMSetRenderTargets(0, nullptr, FALSE, &shadowDsvHandle);
		m_commandList->ClearDepthStencilView(m_shadowDsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		//シャドウマップパス
		App::GetSceneBase().OnDrawPath(0);
		//シャドウマップを読めるようにする
		m_commandList->ResourceBarrier(
			1,
			&CD3DX12_RESOURCE_BARRIER::Transition(
				m_shadowDepthStencil.Get(),
				D3D12_RESOURCE_STATE_DEPTH_WRITE,
				D3D12_RESOURCE_STATE_GENERIC_READ
			)
		);
		//シャドウマップコマンドリストのクローズ
		ThrowIfFailed(m_commandList->Close());
		// シャドウマップコマンドリスト実行
		ID3D12CommandList* ppShadowCommandLists[] = { m_commandList.Get() };
		m_commandQueue->ExecuteCommandLists(_countof(ppShadowCommandLists), ppShadowCommandLists);
		//GPU処理待ち
		WaitForGpu();

		ThrowIfFailed(m_commandAllocators[m_frameIndex]->Reset());
		//コマンドリストのリセット（パイプライン指定なし）
		CommandList::Reset(m_commandAllocators[m_frameIndex], m_commandList);
		// Set necessary state.
		m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
		m_commandList->RSSetViewports(1, &m_viewport);
		m_commandList->RSSetScissorRects(1, &m_scissorRect);
		//ディスクリプタヒープの登録
		ID3D12DescriptorHeap* ppHeaps[] = { m_cbvSrvUavDescriptorHeap.Get(),m_samplerDescriptorHeap.Get() };
		m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

		//シーン描画用
		// バックバッファを使うためのバリア
		m_commandList->ResourceBarrier(
			1,
			&CD3DX12_RESOURCE_BARRIER::Transition(
				m_renderTargets[m_frameIndex].Get(),
				D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET
			)
		);
		//レンダリングターゲットとデプスステンシルのハンドルを作成
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorHandleIncrementSize);
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
		//ハンドルをセット
		m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
		const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		//rtvのクリア
		m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		//dsvのクリア
		m_commandList->ClearDepthStencilView(m_dsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		////ディスクリプタヒープの登録
		//ID3D12DescriptorHeap* ppHeaps[] = { m_cbvSrvUavDescriptorHeap.Get(),m_samplerDescriptorHeap.Get() };
		//m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		// シーンの個別描画
		App::GetSceneBase().OnDrawPath(1);
		// フロントバッファに転送するためのバリア
		m_commandList->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(
				m_renderTargets[m_frameIndex].Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PRESENT
			)
		);
		ThrowIfFailed(m_commandList->Close());
		// 描画用コマンドリスト実行
		ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
		m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
		//GPU処理待ち
		WaitForGpu();

	}

}
//end basedx12
