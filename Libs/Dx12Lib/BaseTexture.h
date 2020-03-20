#pragma once
#include "stdafx.h"

namespace basedx12 {

	//--------------------------------------------------------------------------------------
	///	�e�N�X�`���N���X
	//--------------------------------------------------------------------------------------
	class BaseTexture {
		ComPtr<ID3D12Resource> m_texture;
		ComPtr<ID3D12Resource> m_textureUploadHeap;
		DirectX::ScratchImage m_image;
		std::vector<D3D12_SUBRESOURCE_DATA> m_subresources;
		CD3DX12_CPU_DESCRIPTOR_HANDLE m_maphandle;
		BaseTexture() {}
	public:
		~BaseTexture(){}
		ComPtr<ID3D12Resource> GetTexture() const {
			return m_texture;
		}
		ComPtr<ID3D12Resource> GetTextureUploadHeap() const {
			return m_textureUploadHeap;
		}
		//�T�u���\�[�X�̃A�b�v�f�[�g�ƃV�F�[�_���\�[�X�r���[�̍쐬
		void UpdateSRAndCreateSRV(const ComPtr<ID3D12GraphicsCommandList>& commandList);
		//�e�X�N�`���쐬
		static shared_ptr<BaseTexture>
		CreateBaseTexture(const wstring& FileName,const CD3DX12_CPU_DESCRIPTOR_HANDLE& m_maphandle);

	};



}
//end basedx12
