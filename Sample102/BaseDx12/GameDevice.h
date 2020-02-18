#pragma once
#include "stdafx.h"

namespace basedx12 {

    DECLARE_DX12SHADER(VSPCSprite)
    DECLARE_DX12SHADER(PSPCSprite)

    DECLARE_DX12SHADER(VSPCConstSprite)
    DECLARE_DX12SHADER(PSPCConstSprite)

    DECLARE_DX12SHADER(VSPTSprite)
    DECLARE_DX12SHADER(PSPTSprite)


    class GameDivece : public Dx12Device {
        static const UINT FrameCount = 2;

        struct Vertex
        {
            XMFLOAT3 position;
            XMFLOAT4 color;
        };


        //パイプライン用
        CD3DX12_VIEWPORT m_viewport;
        CD3DX12_RECT m_scissorRect;

        //レンダリングターゲート
        ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
        //コマンドアロケータ
        ComPtr<ID3D12CommandAllocator> m_commandAllocators[FrameCount];
        //コマンドキュー
        ComPtr<ID3D12CommandQueue> m_commandQueue;

        //デスクプリタヒープ
        //レンダリングターゲートビューのデスクプリタヒープ
        ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
        UINT m_rtvDescriptorSize;
        //コンスタントバッファとシェーダリソース用デスクプリタヒープ
        ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
        //CbvSrvのデスクプリタハンドルのインクリメントサイズ
        UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
        //サンプラー用
        ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;

        //GPU側デスクプリタのハンドルの配列
        vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;

        //コマンドリスト
        ComPtr<ID3D12GraphicsCommandList> m_commandList;

        //個別リソース
        //ルートシグネチャ
        ComPtr<ID3D12RootSignature> m_rootSignature;


        //PositionColorの三角形メッシュ
        shared_ptr<Dx12Mesh> m_pcTriangleMesh;
        shared_ptr<Dx12Mesh> m_pntSquareMesh;
        //テクスチャ
        shared_ptr<Dx12Texture> m_SkyTexture;


        //PositionColor用パイプラインステート（コンスタントバッファなし）
        ComPtr<ID3D12PipelineState> m_pcPipelineState;
        //PositionColor用パイプラインステート（コンスタントバッファあり）
        ComPtr<ID3D12PipelineState> m_pcConstPipelineState;
        //PositionTexture用パイプラインステート（コンスタントバッファあり）
        ComPtr<ID3D12PipelineState> m_ptConstPipelineState;


        struct SceneConstantBuffer
        {
            XMFLOAT4 offset;
            SceneConstantBuffer():
                offset(0.0f,0,0,0)
            {}
        };
        //コンスタントバッファの実体
        SceneConstantBuffer m_constantBufferData;
        //コンスタントバッファ
        shared_ptr<ConstantBuffer> m_ConstantBuffer;
        // Synchronization objects.
        UINT m_frameIndex;
        HANDLE m_fenceEvent;
        ComPtr<ID3D12Fence> m_fence;
        UINT64 m_fenceValues[FrameCount];

        void LoadPipeline();
        void LoadAssets();
        void PopulateCommandList();
        void MoveToNextFrame();
        void WaitForGpu();

    public:
        GameDivece();
        virtual ~GameDivece() {}

        virtual void OnInit();
        virtual void OnUpdate();
        virtual void OnRender();
        virtual void OnDestroy();

    };

}
//end basedx12
