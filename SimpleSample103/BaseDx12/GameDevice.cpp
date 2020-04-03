#include "stdafx.h"
#include "Scene.h"
#include "GameDevice.h"

namespace basedx12 {


	GameDivece::GameDivece(UINT frameCount) :
		BaseDevice(frameCount)
	{
		m_Is3DDevice = true;
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
		m_swapChain = SwapChain::CreateDefault(factory, m_commandQueue, m_frameCount);
		//�t���[���C���f�b�N�X�̏����l
		m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
		// �f�X�N�v���^�q�[�v
		{
			// �����_�����O�^�[�Q�b�g�r���[
			m_rtvHeap = DescriptorHeap::CreateRtvHeap(m_frameCount);
			m_rtvDescriptorHandleIncrementSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			//�f�v�X�X�e���V���r���[
			m_dsvHeap = DescriptorHeap::CreateDsvHeap(1);
			m_dsvDescriptorHandleIncrementSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
			//CbvSrvUav�f�X�N�v���^�q�[�v(�R���X�^���g�o�b�t�@�ƃV�F�[�_���\�[�X)
			m_cbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(GetCbvSrvUavMax());
			m_cbvSrvUavDescriptorHandleIncrementSize
				= m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			//�T���v���[�f�X�N���v�^�q�[�v
			m_samplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
		}

		// RTV�ƃR�}���h�A���P�[�^
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
		for (UINT n = 0; n < m_frameCount; n++)
		{
			ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
			m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
			rtvHandle.Offset(1, m_rtvDescriptorHandleIncrementSize);
			//�R�}���h�A���P�[�^
			m_commandAllocators[n] = CommandAllocator::CreateDefault();
		}
		//Dsv���\�[�X
		m_depthStencil = DepthStencil::CreateDefault(m_dsvHeap);
	}

	// �ʃA�Z�b�g�̍\�z
	void GameDivece::LoadAssets()
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
	void GameDivece::OnUpdate()
	{
		//�V�[���ɍX�V��C����
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
		// �o�b�N�o�b�t�@���g�����߂̃o���A
		m_commandList->ResourceBarrier(
			1,
			&CD3DX12_RESOURCE_BARRIER::Transition(
				m_renderTargets[m_frameIndex].Get(),
				D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET
			)
		);
		//�����_�����O�^�[�Q�b�g�ƃf�v�X�X�e���V���̃n���h�����쐬
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorHandleIncrementSize);
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
		//�n���h�����Z�b�g
		m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
		const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		//rtv�̃N���A
		m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		//dsv�̃N���A
		m_commandList->ClearDepthStencilView(m_dsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		//�f�B�X�N���v�^�q�[�v�̓o�^
		ID3D12DescriptorHeap* ppHeaps[] = { m_cbvSrvUavDescriptorHeap.Get(),m_samplerDescriptorHeap.Get() };
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
