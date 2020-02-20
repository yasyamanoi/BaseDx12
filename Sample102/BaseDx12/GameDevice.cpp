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
            m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
            m_CbvSrvDescriptorHandleIncrementSize
                = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
            //�T���v���[�f�X�N���v�^�q�[�v
            m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);

            //GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
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

        // RTV�ƃR�}���h�A���P�[�^
        CreateRTVandCmdAllocators();
    }

    // �ʃA�Z�b�g�̍\�z
    void GameDivece::LoadAssets()
    {
        // ���[�g�V�O�l�`���[
        {
            //�R���X�^���g�o�b�t�@�t���[�g�V�O�l�`��
            m_rootSignature = RootSignature::CreateCbvSrvSmp();
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
    void GameDivece::OnUpdate()
    {
        //�V�[���ɍX�V��C����̂ŉ������Ȃ�
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

        //�R�}���h���X�g�̃��Z�b�g�i�p�C�v���C���w��Ȃ��j
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

        // �o�b�N�o�b�t�@���g�����߂̃o���A
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
        // �V�[���̌ʕ`��
        App::GetSceneBase().OnRender();
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