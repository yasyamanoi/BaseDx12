#pragma once
#include "stdafx.h"

namespace basedx12 {


	//--------------------------------------------------------------------------------------
	/// ファクトリーユーティリティ
	//--------------------------------------------------------------------------------------
	namespace Dx12Factory {
		static inline ComPtr<IDXGIFactory4> CreateDirect() {
			UINT dxgiFactoryFlags = 0;
#if defined(_DEBUG)
			// Enable the debug layer (requires the Graphics Tools "optional feature").
			// NOTE: Enabling the debug layer after device creation will invalidate the active device.
			{
				ComPtr<ID3D12Debug> debugController;
				if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
				{
					debugController->EnableDebugLayer();

					// Enable additional debug layers.
					dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
				}
			}
#endif
			ComPtr<IDXGIFactory4> factory;
			ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));
			return factory;
		}
	}

	namespace D3D12Device {
		void GetHardwareAdapter(_In_ IDXGIFactory2* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter);
		static inline ComPtr<ID3D12Device> CreateDefault(ComPtr<IDXGIFactory4> factory, bool useWarpDevice) {
			ComPtr<ID3D12Device> device;
			if (useWarpDevice)
			{
				ComPtr<IDXGIAdapter> warpAdapter;
				ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

				ThrowIfFailed(D3D12CreateDevice(
					warpAdapter.Get(),
					D3D_FEATURE_LEVEL_11_0,
					IID_PPV_ARGS(&device)
				));
			}
			else
			{
				ComPtr<IDXGIAdapter1> hardwareAdapter;
				GetHardwareAdapter(factory.Get(), &hardwareAdapter);

				ThrowIfFailed(D3D12CreateDevice(
					hardwareAdapter.Get(),
					D3D_FEATURE_LEVEL_11_0,
					IID_PPV_ARGS(&device)
				));
			}
			return device;
		}
	}

	//--------------------------------------------------------------------------------------
	/// コマンドキューユーティリティ
	//--------------------------------------------------------------------------------------
	namespace CommandQueue {
		static inline ComPtr<ID3D12CommandQueue> CreateDirect(const D3D12_COMMAND_QUEUE_DESC& desc) {
			auto Dev = App::GetID3D12Device();
			ComPtr<ID3D12CommandQueue> queue;
			ThrowIfFailed(Dev->CreateCommandQueue(&desc, IID_PPV_ARGS(&queue)));
			return queue;
		}
		static inline ComPtr<ID3D12CommandQueue> CreateDefault() {
			D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			return CreateDirect(queueDesc);
		}
	}

	//--------------------------------------------------------------------------------------
	/// スワップチェーンユーティリティ
	//--------------------------------------------------------------------------------------
	namespace SwapChain {
		static inline ComPtr<IDXGISwapChain3> 
		CreateDirect(
			ComPtr<IDXGIFactory4> factory,
			const DXGI_SWAP_CHAIN_DESC1& desc,
			ComPtr<ID3D12CommandQueue> queue
		) {
			ComPtr<IDXGISwapChain1> swapChain;
			ThrowIfFailed(factory->CreateSwapChainForHwnd(
				queue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
				App::GetHwnd(),
				&desc,
				nullptr,
				nullptr,
				&swapChain
			));
			//Alt+Enterでフルスクリーンにならない
			ThrowIfFailed(factory->MakeWindowAssociation(App::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));
			ComPtr<IDXGISwapChain3> swapChain3;
			ThrowIfFailed(swapChain.As(&swapChain3));
			return swapChain3;
		}
		static inline ComPtr<IDXGISwapChain3>
		CreateDefault(ComPtr<IDXGIFactory4> factory,ComPtr<ID3D12CommandQueue> queue,UINT framecount) {
			// Describe and create the swap chain.
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
			swapChainDesc.BufferCount = framecount;
			swapChainDesc.Width = App::GetGameWidth();
			swapChainDesc.Height = App::GetGameHeight();
			swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDesc.SampleDesc.Count = 1;
			return CreateDirect(factory,swapChainDesc, queue);
		}
	}

	//--------------------------------------------------------------------------------------
	/// デスクプリタヒープユーティリティ
	//--------------------------------------------------------------------------------------
	namespace DescriptorHeap {
		static inline ComPtr<ID3D12DescriptorHeap> CreateDirect(const D3D12_DESCRIPTOR_HEAP_DESC& desc) {
			auto Dev = App::GetID3D12Device();
			ComPtr<ID3D12DescriptorHeap> Ret;
			ThrowIfFailed(Dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&Ret)),
				L"デスクプリタヒープの作成に失敗しました",
				L"Dev->GetDevice()->CreateDescriptorHeap()",
				L"DescriptorHeap::CreateDirect()"
			);
			return Ret;
		}
		static inline ComPtr<ID3D12DescriptorHeap> CreateRtvHeap(UINT FrameCount) {
			//Rtvデスクプリタヒープ
			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
			rtvHeapDesc.NumDescriptors = FrameCount;
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			return CreateDirect(rtvHeapDesc);
		}
		//デプスステンシル
		static inline ComPtr<ID3D12DescriptorHeap> CreateDsvHeap(UINT NumDescriptorHeap) {
			//Dsvデスクプリタヒープ
			D3D12_DESCRIPTOR_HEAP_DESC DsvHeapDesc = {};
			DsvHeapDesc.NumDescriptors = NumDescriptorHeap;
			DsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			DsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			return CreateDirect(DsvHeapDesc);
		}


		static inline ComPtr<ID3D12DescriptorHeap> CreateCbvSrvUavHeap(UINT NumDescriptorHeap) {
			//CbvSrvデスクプリタヒープ
			D3D12_DESCRIPTOR_HEAP_DESC CbvSrvHeapDesc = {};
			CbvSrvHeapDesc.NumDescriptors = NumDescriptorHeap;
			CbvSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			CbvSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			return CreateDirect(CbvSrvHeapDesc);
		}
		static inline ComPtr<ID3D12DescriptorHeap> CreateSamplerHeap(UINT NumDescriptorHeap) {
			//サンプラーデスクプリタヒープ
			D3D12_DESCRIPTOR_HEAP_DESC SamplerHeapDesc = {};
			SamplerHeapDesc.NumDescriptors = NumDescriptorHeap;
			SamplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
			SamplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			return CreateDirect(SamplerHeapDesc);
		}

	}

	//--------------------------------------------------------------------------------------
	/// デプスステンシルユーティリティ
	//--------------------------------------------------------------------------------------
	namespace DepthStencil {
		static inline ComPtr<ID3D12Resource> CreateDefault(const ComPtr<ID3D12DescriptorHeap>& dsvHeap) {
			auto Dev = App::GetID3D12Device();
			ComPtr<ID3D12Resource> ret;

			D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
			depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
			depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

			D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
			depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
			depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
			depthOptimizedClearValue.DepthStencil.Stencil = 0;

			ThrowIfFailed(Dev->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, 
					App::GetGameWidth(), App::GetGameHeight(),
					1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
				D3D12_RESOURCE_STATE_DEPTH_WRITE,
				&depthOptimizedClearValue,
				IID_PPV_ARGS(&ret)
			),
				L"デプスステンシルリソース作成に失敗しました",
				L"Dev->CreateCommittedResource)",
				L"DepthStencil::CreateDefault()"
			);
			Dev->CreateDepthStencilView(ret.Get(), &depthStencilDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart());
			return ret;
		}
	}


	//--------------------------------------------------------------------------------------
	/// ルートシグネチャユーティリティ
	//--------------------------------------------------------------------------------------
	namespace RootSignature {
		static inline ComPtr<ID3D12RootSignature> CreateDirect(const D3D12_ROOT_SIGNATURE_DESC& desc) {
			auto Dev = App::GetID3D12Device();
			D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};
			featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
			if (FAILED(Dev->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
			{
				featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
			}
			ComPtr<ID3DBlob> signature;
			ComPtr<ID3DBlob> error;
			ComPtr<ID3D12RootSignature> Ret;
			ThrowIfFailed(D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error),
				L"ルートシグネチャのシリアライズに失敗しました",
				L"D3D12SerializeRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error)",
				L"RootSignature::CreateDirect()"
			);
			ThrowIfFailed(
				Dev->CreateRootSignature(0, signature->GetBufferPointer(),
					signature->GetBufferSize(), IID_PPV_ARGS(&Ret)),
				L"ルートシグネチャの作成に失敗しました",
				L"Dev->CreateRootSignature()",
				L"RootSignature::CreateDirect()"
			);
			return Ret;
		}
		//一番シンプルなルートシグネチャ
		static inline ComPtr<ID3D12RootSignature> CreateSimple() {
			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
			return CreateDirect(rootSignatureDesc);
		}
		//コンスタントバッファのみ
		static inline ComPtr<ID3D12RootSignature> CreateCbv() {
			CD3DX12_DESCRIPTOR_RANGE ranges[1];
			ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
			CD3DX12_ROOT_PARAMETER rootParameters[1];
			rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_ALL);

			D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
				D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, rootSignatureFlags);
			return CreateDirect(rootSignatureDesc);
		}
		//シェーダリソースとサンプラー
		static inline ComPtr<ID3D12RootSignature> CreateSrvSmp() {
			CD3DX12_DESCRIPTOR_RANGE ranges[2];
			ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
			ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);

			CD3DX12_ROOT_PARAMETER rootParameters[2];
			rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
			return CreateDirect(rootSignatureDesc);
		}

		//シェーダリソースとサンプラーとコンスタントバッファ
		static inline ComPtr<ID3D12RootSignature> CreateSrvSmpCbv() {

			CD3DX12_DESCRIPTOR_RANGE ranges[3];
			ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
			ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);
			ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

			CD3DX12_ROOT_PARAMETER rootParameters[3];
			rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[2].InitAsDescriptorTable(1, &ranges[2], D3D12_SHADER_VISIBILITY_ALL);

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
			return CreateDirect(rootSignatureDesc);
		}
		//シェーダリソース2つとサンプラー2つとコンスタントバッファ1つ
		static inline ComPtr<ID3D12RootSignature> CreateSrv2Smp2Cbv() {
			CD3DX12_DESCRIPTOR_RANGE ranges[5];
			ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
			ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);
			ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);
			ranges[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 1);
			ranges[4].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

			CD3DX12_ROOT_PARAMETER rootParameters[5];
			rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[2].InitAsDescriptorTable(1, &ranges[2], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[3].InitAsDescriptorTable(1, &ranges[3], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[4].InitAsDescriptorTable(1, &ranges[4], D3D12_SHADER_VISIBILITY_ALL);

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
			return CreateDirect(rootSignatureDesc);
		}


	}

	//--------------------------------------------------------------------------------------
	///	サンプラーステート
	//--------------------------------------------------------------------------------------
	enum class SamplerState {
		SamplerNone,
		PointWrap,
		PointClamp,
		LinearWrap,
		LinearClamp,
		AnisotropicWrap,
		AnisotropicClamp,
		ComparisonLinear,
	};

	//--------------------------------------------------------------------------------------
	/// サンプラーユーティリティ（変更可能）
	//--------------------------------------------------------------------------------------
	namespace Sampler {
		static inline void CreateSampler(const SamplerState State, D3D12_CPU_DESCRIPTOR_HANDLE& Handle) {
			D3D12_SAMPLER_DESC samplerDesc = {};
			//デフォルトを入れておく
			samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
			samplerDesc.MipLODBias = 0.0f;
			samplerDesc.MaxAnisotropy = 1;
			samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;

			switch (State) {
			case SamplerState::PointWrap:
				samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				break;
			case SamplerState::PointClamp:
				samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				break;
			case SamplerState::LinearWrap:
				samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				break;
			case SamplerState::LinearClamp:
				samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				break;
			case SamplerState::AnisotropicWrap:
				samplerDesc.Filter = D3D12_FILTER_ANISOTROPIC;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				break;
			case SamplerState::AnisotropicClamp:
				samplerDesc.Filter = D3D12_FILTER_ANISOTROPIC;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				break;
			case SamplerState::ComparisonLinear:
				samplerDesc.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
				samplerDesc.BorderColor[0] = 1.0f;
				samplerDesc.BorderColor[1] = 1.0f;
				samplerDesc.BorderColor[2] = 1.0f;
				samplerDesc.BorderColor[3] = 1.0f;
				samplerDesc.MinLOD = 0;
				samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
				samplerDesc.MipLODBias = 0.0f;
				samplerDesc.MaxAnisotropy = 0;
				samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

				break;
			}

			//デバイスの取得
			auto Dev = App::GetID3D12Device();
			Dev->CreateSampler(&samplerDesc, Handle);
		}
	}

	//--------------------------------------------------------------------------------------
	/// コンスタントバッファユーティリティ
	//--------------------------------------------------------------------------------------
	class ConstantBuffer {
		ComPtr<ID3D12Resource> m_constantBuffer;
		void* m_pCbvDataBegin;
		ConstantBuffer() :m_pCbvDataBegin(nullptr){}
	public:
		~ConstantBuffer() {
			m_constantBuffer->Unmap(0, nullptr);
		}
		template<typename T>
		static inline shared_ptr<ConstantBuffer> CreateDirect(
			const CD3DX12_CPU_DESCRIPTOR_HANDLE& descHandle,
			const T& src
		) {
			//デバイスの取得
			auto Dev = App::GetID3D12Device();
			shared_ptr<ConstantBuffer> Ptr = shared_ptr<ConstantBuffer>(new ConstantBuffer());
			// CB size is required to be 256-byte aligned.
			UINT constsize = (sizeof(T) + 255) & ~255;
			ThrowIfFailed(Dev->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(constsize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&Ptr->m_constantBuffer)));

			// Describe and create a constant buffer view.
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
			cbvDesc.BufferLocation = Ptr->m_constantBuffer->GetGPUVirtualAddress();
			cbvDesc.SizeInBytes = constsize;    
			Dev->CreateConstantBufferView(&cbvDesc, descHandle);

			// Map and initialize the constant buffer. We don't unmap this until the
			// app closes. Keeping things mapped for the lifetime of the resource is okay.
			CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
			ThrowIfFailed(Ptr->m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&Ptr->m_pCbvDataBegin)));
			memcpy(Ptr->m_pCbvDataBegin, &src, sizeof(src));

			Ptr->m_constantBuffer->Unmap(0, nullptr);

			return Ptr;
		}
		template<typename T>
		void Copy(const T& src) {
			CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
			ThrowIfFailed(m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_pCbvDataBegin)));
			memcpy(m_pCbvDataBegin, &src, sizeof(src));
			m_constantBuffer->Unmap(0, nullptr);
		}
	};

	//--------------------------------------------------------------------------------------
	/// パイプラインステートユーティリティ
	//--------------------------------------------------------------------------------------
	namespace PipelineState {
		static inline ComPtr<ID3D12PipelineState> CreateDirect(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc) {
			//デバイスの取得
			auto Dev = App::GetID3D12Device();
			ComPtr<ID3D12PipelineState> Ret;
			ThrowIfFailed(Dev->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&Ret)),
				L"パイプラインステートの作成に失敗しました",
				L"Dev->GetDevice()->CreateGraphicsPipelineState()",
				L"PipelineState::CreateDirect()"
			);
			return Ret;
		}
		template<typename Vertex, typename VS, typename PS>
		static inline ComPtr<ID3D12PipelineState> CreateDefault2D(const ComPtr<ID3D12RootSignature>& rootSignature, D3D12_GRAPHICS_PIPELINE_STATE_DESC& RetDesc) {

			ZeroMemory(&RetDesc, sizeof(RetDesc));
			RetDesc.InputLayout = { Vertex::GetVertexElement(), Vertex::GetNumElements() };
			RetDesc.pRootSignature = rootSignature.Get();
			RetDesc.VS = CD3DX12_SHADER_BYTECODE(VS::GetPtr()->GetShaderComPtr()->GetBufferPointer(), VS::GetPtr()->GetShaderComPtr()->GetBufferSize());
			RetDesc.PS = CD3DX12_SHADER_BYTECODE(PS::GetPtr()->GetShaderComPtr()->GetBufferPointer(), PS::GetPtr()->GetShaderComPtr()->GetBufferSize());
			RetDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			RetDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			RetDesc.DepthStencilState.DepthEnable = FALSE;
			RetDesc.DepthStencilState.StencilEnable = FALSE;
			RetDesc.SampleMask = UINT_MAX;
			RetDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			RetDesc.NumRenderTargets = 1;
			RetDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			RetDesc.SampleDesc.Count = 1;
			return CreateDirect(RetDesc);
		}
		template<typename Vertex, typename VS, typename PS>
		static inline ComPtr<ID3D12PipelineState> CreateDefault3D(const ComPtr<ID3D12RootSignature>& rootSignature, D3D12_GRAPHICS_PIPELINE_STATE_DESC& RetDesc) {

			CD3DX12_RASTERIZER_DESC rasterizerStateDesc(D3D12_DEFAULT);
			//裏面カリング
			rasterizerStateDesc.CullMode = D3D12_CULL_MODE_NONE;

			ZeroMemory(&RetDesc, sizeof(RetDesc));
			RetDesc.InputLayout = { Vertex::GetVertexElement(), Vertex::GetNumElements() };
			RetDesc.pRootSignature = rootSignature.Get();
			RetDesc.VS =
			{
				reinterpret_cast<UINT8*>(VS::GetPtr()->GetShaderComPtr()->GetBufferPointer()),
				VS::GetPtr()->GetShaderComPtr()->GetBufferSize()
			};
			RetDesc.PS =
			{
				reinterpret_cast<UINT8*>(PS::GetPtr()->GetShaderComPtr()->GetBufferPointer()),
				PS::GetPtr()->GetShaderComPtr()->GetBufferSize()
			};
			RetDesc.RasterizerState = rasterizerStateDesc;
			RetDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			RetDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
			RetDesc.SampleMask = UINT_MAX;
			RetDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			RetDesc.NumRenderTargets = 1;
			RetDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			RetDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
			RetDesc.SampleDesc.Count = 1;
			return CreateDirect(RetDesc);
		}

		template<typename Vertex, typename VS>
		static inline ComPtr<ID3D12PipelineState> CreateShadowmap3D(const ComPtr<ID3D12RootSignature>& rootSignature, D3D12_GRAPHICS_PIPELINE_STATE_DESC& RetDesc) {

			CD3DX12_RASTERIZER_DESC rasterizerStateDesc(D3D12_DEFAULT);
			//表面カリング
			rasterizerStateDesc.CullMode = D3D12_CULL_MODE_FRONT;
			rasterizerStateDesc.FillMode = D3D12_FILL_MODE_SOLID;
			rasterizerStateDesc.DepthClipEnable = TRUE;

			ZeroMemory(&RetDesc, sizeof(RetDesc));
			RetDesc.InputLayout = { Vertex::GetVertexElement(), Vertex::GetNumElements() };
			RetDesc.pRootSignature = rootSignature.Get();
			RetDesc.VS =
			{
				reinterpret_cast<UINT8*>(VS::GetPtr()->GetShaderComPtr()->GetBufferPointer()),
				VS::GetPtr()->GetShaderComPtr()->GetBufferSize()
			};

			RetDesc.PS = CD3DX12_SHADER_BYTECODE(0, 0);
			RetDesc.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
			RetDesc.NumRenderTargets = 0;

			RetDesc.RasterizerState = rasterizerStateDesc;
			RetDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			RetDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
			RetDesc.SampleMask = UINT_MAX;
			RetDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			RetDesc.NumRenderTargets = 0;
			RetDesc.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
			RetDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
			RetDesc.SampleDesc.Count = 1;
			return CreateDirect(RetDesc);
		}

	}


	//--------------------------------------------------------------------------------------
	/// コマンドアロケータユーティリティ
	//--------------------------------------------------------------------------------------
	namespace CommandAllocator {
		static inline  ComPtr<ID3D12CommandAllocator> CreateDefault() {
			//デバイスの取得
			auto Dev = App::GetID3D12Device();
			ComPtr<ID3D12CommandAllocator> allocator;
			ThrowIfFailed(
				Dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,IID_PPV_ARGS(&allocator)),
				L"コマンドアロケータの作成に失敗しました",
				L"Dev->CreateCommandAllocator()",
				L"CommandAllocator::CreateDefault()"
			);
			return allocator;
		}
	}

	//--------------------------------------------------------------------------------------
	/// コマンドリストユーティリティ
	//--------------------------------------------------------------------------------------
	namespace CommandList {
		static inline  ComPtr<ID3D12GraphicsCommandList> CreateSimple(const ComPtr<ID3D12CommandAllocator>& allocator) {
			//デバイスの取得
			auto Dev = App::GetID3D12Device();
			ComPtr<ID3D12GraphicsCommandList> Ret;
			ThrowIfFailed(Dev->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				allocator.Get(),
				nullptr,
				IID_PPV_ARGS(&Ret)),
				L"コマンドリストの作成に失敗しました",
				L"Dev->GetDevice()->CreateCommandList()",
				L"CommandList::CreateSimple()"
			);
			return Ret;
		}

		static inline  ComPtr<ID3D12GraphicsCommandList> CreateDefault(
			const ComPtr<ID3D12CommandAllocator>& allocator,
			const ComPtr<ID3D12PipelineState>& pipelineState) {
			//デバイスの取得
			auto Dev = App::GetID3D12Device();
			ComPtr<ID3D12GraphicsCommandList> Ret;
			ThrowIfFailed(Dev->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				allocator.Get(),
				pipelineState.Get(),
				IID_PPV_ARGS(&Ret)),
				L"コマンドリストの作成に失敗しました",
				L"Dev->GetDevice()->CreateCommandList()",
				L"CommandList::CreateDefault()"
			);
			return Ret;
		}
		static inline  ComPtr<ID3D12GraphicsCommandList> CreateCopy(
			const ComPtr<ID3D12CommandAllocator>& allocator,
			const ComPtr<ID3D12PipelineState>& pipelineState) {
			//デバイスの取得
			auto Dev = App::GetID3D12Device();
			ComPtr<ID3D12GraphicsCommandList> Ret;
			ThrowIfFailed(Dev->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_COPY,
				allocator.Get(),
				pipelineState.Get(),
				IID_PPV_ARGS(&Ret)),
				L"コマンドリストの作成に失敗しました\n",
				L"Dev->CreateCommandList()\n",
				L"CommandList::CreateCopy()"
			);
			return Ret;
		}

		static inline  void Reset(
			const ComPtr<ID3D12CommandAllocator>& allocator,
			const ComPtr<ID3D12GraphicsCommandList>& commandList
		) {
			//デバイスの取得
			auto Dev = App::GetID3D12Device();
			ThrowIfFailed(commandList->Reset(allocator.Get(), nullptr),
				L"コマンドリストのリセットに失敗しました\n",
				L"commandList->Reset(allocator..Get(),nullptr)\n",
				L"CommandList::Reset()"
			);

		}
		static inline  void Reset(
			const ComPtr<ID3D12CommandAllocator>& allocator,
			const ComPtr<ID3D12GraphicsCommandList>& commandList,
			const ComPtr<ID3D12PipelineState>& pipelineState
			) {
			//デバイスの取得
			auto Dev = App::GetID3D12Device();
			ThrowIfFailed(commandList->Reset(allocator.Get(), pipelineState.Get()),
				L"コマンドリストのリセットに失敗しました\n",
				L"commandList->Reset(allocator.Get(), pipelineState.Get()\n",
				L"CommandList::Reset()"
			);

		}
		static inline  void Close(const ComPtr<ID3D12GraphicsCommandList>& commandList) {
			ThrowIfFailed(commandList->Close(),
				L"コマンドリストのクローズに失敗しました\n",
				L"commandList->Close()\n",
				L"CommandList::Close()"
			);

		}
		static inline void Excute(const ComPtr<ID3D12CommandQueue>& queue,const ComPtr<ID3D12GraphicsCommandList>& commandList) {
			ID3D12CommandList* ppCommandLists[] = { commandList.Get() };
			queue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
		}


	}



}

//end basedx12
