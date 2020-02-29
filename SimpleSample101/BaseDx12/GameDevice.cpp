#include "stdafx.h"
#include "Scene.h"
#include "GameDevice.h"

namespace basedx12 {


    GameDivece::GameDivece(UINT frameCount) :
        Dx12Device(frameCount)
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
        m_device = D3D12Device::CreateDefault(factory, m_useWarpDevice);
//�R�}���h�L���[
        m_commandQueue = CommandQueue::CreateDefault();
        //�X���b�v�`�F�[��(�e�N���X�ɂ���)
        m_swapChain = SwapChain::CreateDefault(m_commandQueue, m_FrameCount);

        // This sample does not support fullscreen transitions.
        ThrowIfFailed(factory->MakeWindowAssociation(App::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));

        m_frameIndex = GetIDXGISwapChain3()->GetCurrentBackBufferIndex();

        // �f�X�N�v���^�q�[�v
        {
            // �����_�����O�^�[�Q�b�g�r���[
            m_rtvHeap = DescriptorHeap::CreateRtvHeap(m_FrameCount);
            m_rtvDescriptorSize = GetID3D12Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        }
        // RTV�ƃR�}���h�A���P�[�^
        CreateRTVandCmdAllocators();
    }

    // �ʃA�Z�b�g�̍\�z
    void GameDivece::LoadAssets()
    {
        // ���[�g�V�O�l�`���[
        {
            //��ԃV���v��
            m_rootSignature = RootSignature::CreateSimple();
        }
        // ���_�Ȃǂ̃��\�[�X�\�z�p�̃R�}���h���X�g
        m_commandList = CommandList::CreateSimple(m_commandAllocators[m_frameIndex]);
        //�V�[���Ɋe�I�u�W�F�N�g�̍\�z��C����
        App::GetSceneBase().OnInitAssets();
        //�R�}���h���C���N���[�Y����уL���[�̎��s
        ThrowIfFailed(m_commandList->Close());
        ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
        m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

        //�����I�u�W�F�N�g����тf�o�t�̏����҂�
        SyncAndWaitForGpu();
    }

    // Update frame-based values.
    void GameDivece::OnUpdate()
    {
        App::GetSceneBase().OnUpdate();
    }

    // �`�揈��
    void GameDivece::OnDraw()
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

        //�R�}���h���X�g�̃��Z�b�g�i�p�C�v���C���w��Ȃ��j
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
        // �V�[���Ɍʕ`���C����
        App::GetSceneBase().OnDraw();
        // Indicate that the back buffer will now be used to present.
        m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

        ThrowIfFailed(m_commandList->Close());
    }


}
//end basedx12
