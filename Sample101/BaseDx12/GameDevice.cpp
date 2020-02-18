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

    //������
    void GameDivece::OnInit()
    {
        LoadPipeline();
        LoadAssets();
    }

    // �p�C�v���C���̏���
    void GameDivece::LoadPipeline()
    {
//�t�@�N�g��
        ComPtr<IDXGIFactory4> factory = Dx12Factory::CreateDirect();
        //�f�o�C�X(�e�N���X�ɂ���)
        SetID3D12Device(D3D12Device::CreateDefault(factory, m_useWarpDevice));
//�R�}���h�L���[
        m_commandQueue = CommandQueue::CreateDefault();
        //�X���b�v�`�F�[��(�e�N���X�ɂ���)
        SetIDXGISwapChain3(SwapChain::CreateDefault(m_commandQueue, FrameCount));

        // This sample does not support fullscreen transitions.
        ThrowIfFailed(factory->MakeWindowAssociation(App::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));

        m_frameIndex = GetIDXGISwapChain3()->GetCurrentBackBufferIndex();

        // �f�X�N�v���^�q�[�v
        {
            // �����_�����O�^�[�Q�b�g�r���[
            m_rtvHeap = DescriptorHeap::CreateRtvHeap(FrameCount);
            m_rtvDescriptorSize = GetID3D12Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        }

        // �t���[�����[�\�[�X�i�t���[�����Ƃɍ쐬����j
        {
            CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

            // Create a RTV and a command allocator for each frame.
            for (UINT n = 0; n < FrameCount; n++)
            {
                ThrowIfFailed(GetIDXGISwapChain3()->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
                GetID3D12Device()->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
                rtvHandle.Offset(1, m_rtvDescriptorSize);
                //�R�}���h�A���P�[�^
                m_commandAllocators[n] = CommandAllocator::CreateDefault();
            }
        }
    }

    // �ʃA�Z�b�g�̍\�z
    void GameDivece::LoadAssets()
    {
        // ���[�g�V�O�l�`���[
        {
            //��ԃV���v��
            m_rootSignature = RootSignature::CreateSimple();
        }
        // �Q�c�̊�{�I�ȃp�C�v���C��
        {
            D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
            m_pipelineState
                = PipelineState::CreateDefault2D<VertexPositionColor, VSPCSprite, PSPCSprite>(m_rootSignature, PipeLineDesc);
        }
        // ���_�Ȃǂ̃��\�[�X�\�z�p�̃R�}���h���X�g
        m_commandList = CommandList::CreateSimple(m_commandAllocators[m_frameIndex]);
        // ���b�V��
        {
            vector<VertexPositionColor> vertex =
            {
                { Vec3( 0.0f, 0.25f * m_aspectRatio, 0.0f ), Vec4(1.0f, 0.0f, 0.0f, 1.0f ) },
                { Vec3(0.25f, -0.25f * m_aspectRatio, 0.0f ), Vec4(0.0f, 1.0f, 0.0f, 1.0f ) },
                { Vec3(-0.25f, -0.25f * m_aspectRatio, 0.0f ), Vec4(0.0f, 0.0f, 1.0f, 1.0f ) }
            };
            //���b�V���쐬
            m_Dx12Mesh = Dx12Mesh::CreateDx12Mesh<VertexPositionColor>(m_commandList, vertex);
        }

        //�R�}���h���C���N���[�Y����уL���[�̎��s
        ThrowIfFailed(m_commandList->Close());
        ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
        m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

        //�����I�u�W�F�N�g����тf�o�t�̏����҂�
        {
            ThrowIfFailed(GetID3D12Device()->CreateFence(m_fenceValues[m_frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
            m_fenceValues[m_frameIndex]++;

            // Create an event handle to use for frame synchronization.
            m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
            if (m_fenceEvent == nullptr)
            {
                ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
            }

            //GPU�̏����҂�
            WaitForGpu();
        }
    }

    // Update frame-based values.
    void GameDivece::OnUpdate()
    {
    }

    // �`�揈��
    void GameDivece::OnRender()
    {
        // �`��̂��߂̃R�}���h���X�g���W�߂�
        PopulateCommandList();

        // �`��p�R�}���h���X�g���s
        ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
        m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

        // �t�����g�o�b�t�@�ɕ\��
        ThrowIfFailed(GetIDXGISwapChain3()->Present(1, 0));

        //���̃t���[���Ɉړ�
        MoveToNextFrame();
    }
    //��n��
    void GameDivece::OnDestroy()
    {
        //GPU�̏����҂�
        WaitForGpu();

        CloseHandle(m_fenceEvent);
    }

    // �`��̂��߂̃R�}���h���X�g���W�߂�
    void GameDivece::PopulateCommandList()
    {
        ThrowIfFailed(m_commandAllocators[m_frameIndex]->Reset());

//        ThrowIfFailed(m_commandList->Reset(m_commandAllocators[m_frameIndex].Get(), m_pipelineState.Get()));
        //�R�}���h���X�g�̃��Z�b�g
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
