#pragma once
#include "stdafx.h"

namespace basedx12 {

	//--------------------------------------------------------------------------------------
	///	テクスチャクラス
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
		//サブリソースのアップデートとシェーダリソースビューの作成
		void UpdateSRAndCreateSRV(const ComPtr<ID3D12GraphicsCommandList>& commandList);
		//テスクチャ作成
		static shared_ptr<BaseTexture>
		CreateBaseTexture(const wstring& FileName,const CD3DX12_CPU_DESCRIPTOR_HANDLE& m_maphandle);

	};



}
//end basedx12
