#include "stdafx.h"

namespace basedx12 {

    Default2DDivece::Default2DDivece(UINT frameCount) :
        Dx12Device(frameCount)
    {
    }


    //������
    void Default2DDivece::OnInit()
    {
        LoadPipeline();
        LoadAssets();
    }

    // �p�C�v���C���̏���
    void Default2DDivece::LoadPipeline()
    {
        //�t�@�N�g��
        ComPtr<IDXGIFactory4> factory = Dx12Factory::CreateDirect();
        //�f�o�C�X
        m_device = D3D12Device::CreateDefault(factory, m_useWarpDevice);
        //�R�}���h�L���[
        m_commandQueue = CommandQueue::CreateDefault();
        //�X���b�v�`�F�[��
        m_swapChain = SwapChain::CreateDefault(m_commandQueue, m_FrameCount);
        // This sample does not support fullscreen transitions.
        ThrowIfFailed(factory->MakeWindowAssociation(App::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));
        m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
        // �f�X�N�v���^�q�[�v
        {
            // �����_�����O�^�[�Q�b�g�r���[
            m_rtvHeap = DescriptorHeap::CreateRtvHeap(m_FrameCount);
            m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

            //CbvSrv�f�X�N�v���^�q�[�v(�R���X�^���g�o�b�t�@�ƃV�F�[�_���\�[�X)
            m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + m_constBuffMax);
            m_CbvSrvDescriptorHandleIncrementSize
                = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
            //�T���v���[�f�X�N���v�^�q�[�v
            m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
        }

        // RTV�ƃR�}���h�A���P�[�^
        CreateRTVandCmdAllocators();
    }

    // �ʃA�Z�b�g�̍\�z
    void Default2DDivece::LoadAssets()
    {
        // ���[�g�V�O�l�`���[
        {
            //�R���X�^���g�o�b�t�@�t���[�g�V�O�l�`��
            m_rootSignature = RootSignature::CreateSrvSmpCbv();
        }
        // ���_�Ȃǂ̃��\�[�X�\�z�p�̃R�}���h���X�g
        m_commandList = CommandList::CreateSimple(m_commandAllocators[m_frameIndex]);
        //�V�[���Ɋe�I�u�W�F�N�g�̍\�z��C����
        App::GetSceneBase().OnInitAssets();
        //�R�}���h���C���N���[�Y����уL���[�̎��s
        CommandList::Close(m_commandList);
        CommandList::Excute(m_commandQueue, m_commandList);
        //�����I�u�W�F�N�g����тf�o�t�̏����҂�
        SyncAndWaitForGpu();
    }

    //�X�V
    void Default2DDivece::OnUpdate()
    {
        // ���_�Ȃǂ̃��\�[�X�\�z�p�̃R�}���h���X�g
        m_commandList = CommandList::CreateSimple(m_commandAllocators[m_frameIndex]);
        //�V�[���̍X�V�i�������̊ԂɃ��b�V���̒ǉ�������΃R�}���h���s�j
        App::GetSceneBase().OnUpdate();
        //�R�}���h���C���N���[�Y����уL���[�̎��s
        CommandList::Close(m_commandList);
        CommandList::Excute(m_commandQueue, m_commandList);
        //�����I�u�W�F�N�g����тf�o�t�̏����҂�
        SyncAndWaitForGpu();
    }

    // �`�揈��
    void Default2DDivece::OnDraw()
    {
        // �`��̂��߂̃R�}���h���X�g���W�߂�
        PopulateCommandList();

        // �`��p�R�}���h���X�g���s
        ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
        m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

//        m_commandQueue->ExecuteCommandLists((UINT)m_drawCommandLists.size(), &m_drawCommandLists[0]);


        // �t�����g�o�b�t�@�ɕ\��
        ThrowIfFailed(GetIDXGISwapChain3()->Present(1, 0));

        //���̃t���[���Ɉړ�
        MoveToNextFrame();
    }
    //��n��
    void Default2DDivece::OnDestroy()
    {
        //GPU�̏����҂�
        WaitForGpu();
        CloseHandle(m_fenceEvent);
    }

    // �`��̂��߂̃R�}���h���X�g���W�߂�
    void Default2DDivece::PopulateCommandList()
    {
        ThrowIfFailed(m_commandAllocators[m_frameIndex]->Reset());

        //�R�}���h���X�g�̃��Z�b�g�i�p�C�v���C���w��Ȃ��j
        CommandList::Reset(m_commandAllocators[m_frameIndex], m_commandList);

        // Set necessary state.
        m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
        m_commandList->RSSetViewports(1, &m_viewport);
        m_commandList->RSSetScissorRects(1, &m_scissorRect);

        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
        m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
        const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
        m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

        // �o�b�N�o�b�t�@���g�����߂̃o���A
        m_commandList->ResourceBarrier(
            1,
            &CD3DX12_RESOURCE_BARRIER::Transition(
                m_renderTargets[m_frameIndex].Get(),
                D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET
            )
        );

        ID3D12DescriptorHeap* ppHeaps[] = { m_CbvSrvUavDescriptorHeap.Get(),m_SamplerDescriptorHeap.Get()};
        m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
        // �V�[���̌ʕ`��
        App::GetSceneBase().OnDraw();
        // �t�����g�o�b�t�@�ɓ]�����邽�߂̃o���A
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
