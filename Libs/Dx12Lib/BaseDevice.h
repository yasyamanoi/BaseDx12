#pragma once
#include "stdafx.h"

namespace basedx12 {

	//--------------------------------------------------------------------------------------
	///	BaseDevice�N���X
	//--------------------------------------------------------------------------------------
	class BaseDevice : public ObjectInterface {
		//�R���X�^���g�o�b�t�@�ƃV�F�[�_�[���\�[�X��MAX�T�C�Y
		const UINT m_cbvSrvUavMax;
		UINT m_cbvSrvUavSendIndex;
	protected:
		BaseDevice(UINT frameCount, UINT cbvSrvUavMax = 1024);
		virtual ~BaseDevice();
	public:
		UINT GetCbvSrvUavNextIndex();
		UINT GetCbvSrvUavMax() const {
			return m_cbvSrvUavMax;
		}
		UINT GetFrameCount()const {
			return m_frameCount;
		}
		UINT GetFrameIndex() const {
			return m_frameIndex;
		}
		bool IsUseWarpDevice() const {
			return m_useWarpDevice;
		}
		ComPtr<ID3D12Device> GetID3D12Device() const {
			return m_device;
		}
		ComPtr<IDXGISwapChain3> GetIDXGISwapChain3() const {
			return m_swapChain;
		}
		const CD3DX12_VIEWPORT& GetViewport() const {
			return m_viewport;
		}
		void SetViewport(const CD3DX12_VIEWPORT& viewport) {
			m_viewport = viewport;
		}
		const CD3DX12_RECT& GetScissorRect() const {
			return m_scissorRect;
		}
		void SetScissorRect(const CD3DX12_RECT& scissorRect) {
			m_scissorRect = scissorRect;
		}
		float GetAspectRatio() const {
			return m_aspectRatio;
		}
		ComPtr<ID3D12CommandQueue> GetCommandQueue() const {
			return m_commandQueue;
		}
		ComPtr<ID3D12Fence> GetFence() const {
			return m_fence;
		}
		const vector<UINT64>& GetFenceValues() const {
			return m_fenceValues;
		}
		const HANDLE& GetFenceEvent() const {
			return m_fenceEvent;
		}
		const vector<ComPtr<ID3D12Resource>>& GetRenderTargets() const {
			return m_renderTargets;
		}
		const vector<ComPtr<ID3D12CommandAllocator>> GetCommandAllocators() const {
			return m_commandAllocators;
		}
		ComPtr<ID3D12DescriptorHeap> GetRtvHeap() {
			return m_rtvHeap;
		}
		UINT GetRtvDescriptorHandleIncrementSize() const {
			return m_rtvDescriptorHandleIncrementSize;
		}
		ComPtr<ID3D12DescriptorHeap> GetDsvHeap() {
			return m_dsvHeap;
		}
		UINT GetDsvDescriptorHandleIncrementSize() const {
			return m_dsvDescriptorHandleIncrementSize;
		}
		ComPtr<ID3D12Resource> GetDepthStencil() const {
			return m_depthStencil;
		}
		ComPtr<ID3D12DescriptorHeap> GetShadowmapDsvHeap() {
			return m_shadowDsvHeap;
		}
		ComPtr<ID3D12Resource> GetShadowmapDepthStencil() const {
			return m_shadowDepthStencil;
		}
		UINT GetShadowSRVIndex() const{
			return m_ShadowSRVIndex;
		}
		ComPtr<ID3D12DescriptorHeap> GetCbvSrvUavDescriptorHeap() const {
			return m_cbvSrvUavDescriptorHeap;
		}
		UINT GetCbvSrvUavDescriptorHandleIncrementSize() const {
			return m_cbvSrvUavDescriptorHandleIncrementSize;
		}
		ComPtr<ID3D12DescriptorHeap> GetSamplerDescriptorHeap() const {
			return m_samplerDescriptorHeap;
		}
		UINT GetSamplerDescriptorHandleIncrementSize() const {
			return m_samplerDescriptorHandleIncrementSize;
		}
		ComPtr<ID3D12GraphicsCommandList> GetCommandList() const {
			return m_commandList;
		}
		ComPtr<ID3D12RootSignature> GetRootSignature() const {
			return m_rootSignature;
		}
		bool Is3DDevice() const {
			return m_Is3DDevice;
		}
	protected:
		//3D�f�o�C�X���ǂ���
		bool m_Is3DDevice;
		//�t���[���֘A
		const UINT m_frameCount;
		UINT m_frameIndex = 0;
		//���b�v�A�_�v�^�[���g�����ǂ����ifalse�j
		bool m_useWarpDevice = false;
		//�f�o�C�X�ƃX���b�v�`�F�[��
		ComPtr<ID3D12Device> m_device;
		ComPtr<IDXGISwapChain3> m_swapChain;
		//�r���[�֘A
		CD3DX12_VIEWPORT m_viewport;
		CD3DX12_RECT m_scissorRect;
		float m_aspectRatio;
		//�R�}���h�L���[
		ComPtr<ID3D12CommandQueue> m_commandQueue;
		//�t�F���X�֘A
		ComPtr<ID3D12Fence> m_fence;
		vector<UINT64> m_fenceValues;
		HANDLE m_fenceEvent;
		//�����_�����O�^�[�Q�[�g
		vector<ComPtr<ID3D12Resource>> m_renderTargets;
		//�R�}���h�A���P�[�^
		vector<ComPtr<ID3D12CommandAllocator>> m_commandAllocators;
		vector<ComPtr<ID3D12CommandAllocator>> m_shadowmapCommandAllocators;
		//�f�X�N�v���^�q�[�v
		//�����_�����O�^�[�Q�[�g�r���[�̃f�X�N�v���^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
		UINT m_rtvDescriptorHandleIncrementSize = 0;
		//�f�v�X�X�e���V�b��
		ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
		UINT m_dsvDescriptorHandleIncrementSize = 0;
		ComPtr<ID3D12Resource> m_depthStencil;
		//�V���h�E�}�b�v�p�f�v�X�X�e���V��
		ComPtr<ID3D12DescriptorHeap> m_shadowDsvHeap;
		ComPtr<ID3D12Resource> m_shadowDepthStencil;
		//�V���h�E�`��p�V�F�[�_���\�[�X�̃C���f�b�N�X
		UINT m_ShadowSRVIndex = 0;
		//�R���X�^���g�o�b�t�@�ƃV�F�[�_���\�[�X�p�f�X�N�v���^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_cbvSrvUavDescriptorHeap;
		//CbvSrvUav�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
		UINT m_cbvSrvUavDescriptorHandleIncrementSize{ 0 };
		//�T���v���[�p
		ComPtr<ID3D12DescriptorHeap> m_samplerDescriptorHeap;
		//�T���v���[�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
		UINT m_samplerDescriptorHandleIncrementSize{ 0 };
		//�R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_commandList;
		//���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> m_rootSignature;
		//�����o�֐�
		void GetHardwareAdapter(_In_ IDXGIFactory2* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter);
		void MoveToNextFrame();
		void WaitForGpu();
		void SyncAndWaitForGpu();
	};

}
// end namespace basedx12
