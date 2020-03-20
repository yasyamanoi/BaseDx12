#include "stdafx.h"

namespace basedx12 {

	shared_ptr<BaseTexture>  BaseTexture::CreateBaseTexture(const wstring& FileName, const CD3DX12_CPU_DESCRIPTOR_HANDLE& maphandle) {
		if (FileName == L"") {
			throw BaseException(
				L"�t�@�C�����w�肳��Ă��܂���\n",
				L"BaseTexture::CreateBaseTexture()"
			);
		}
		DWORD RetCode;
		RetCode = GetFileAttributes(FileName.c_str());
		if (RetCode == -1) {
			throw BaseException(
				L"�t�@�C�������݂��܂���\n",
				FileName,
				L"\nBaseTexture::CreateBaseTexture()"
			);
		}
		//�e�N�X�`���쐬
		//�t�@�C���g���q�̒���
		wchar_t Drivebuff[_MAX_DRIVE];
		wchar_t Dirbuff[_MAX_DIR];
		wchar_t FileNamebuff[_MAX_FNAME];
		wchar_t Extbuff[_MAX_EXT];

		::ZeroMemory(Drivebuff, sizeof(Drivebuff));
		::ZeroMemory(Dirbuff, sizeof(Dirbuff));
		::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
		::ZeroMemory(Extbuff, sizeof(Extbuff));

		_wsplitpath_s(FileName.c_str(),
			Drivebuff, _MAX_DRIVE,
			Dirbuff, _MAX_DIR,
			FileNamebuff, _MAX_FNAME,
			Extbuff, _MAX_EXT);

		wstring ExtStr = Extbuff;
		shared_ptr<BaseTexture> Ptr = shared_ptr<BaseTexture>(new BaseTexture());
		TexMetadata matadata;

		if (ExtStr == L".dds" || ExtStr == L".DDS") {
			ThrowIfFailed(
				DirectX::LoadFromDDSFile(FileName.c_str(), DDS_FLAGS_NONE, &matadata, Ptr->m_image),
				L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���\n",
				FileName,
				L"\nBaseTexture::CreateBaseTexture()"
			);
		}
		else if (ExtStr == L".tga" || ExtStr == L".TGA") {
			ThrowIfFailed(
				DirectX::LoadFromTGAFile(FileName.c_str(), &matadata, Ptr->m_image),
				L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���\n",
				FileName,
				L"\nBaseTexture::CreateBaseTexture()"
			);
		}
		else if (ExtStr == L".hdr" || ExtStr == L".HDR")
		{
			ThrowIfFailed(
				DirectX::LoadFromHDRFile(FileName.c_str(), &matadata, Ptr->m_image),
				L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���\n",
				FileName,
				L"\nBaseTexture::CreateBaseTexture()"
			);
		}
		else {
			ThrowIfFailed(
				DirectX::LoadFromWICFile(FileName.c_str(), WIC_FLAGS_NONE, &matadata, Ptr->m_image),
				L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���\n",
				FileName,
				L"\nBaseTexture::CreateBaseTexture()"
			);
		}
		//�f�o�C�X�̎擾
		auto Dev = App::GetID3D12Device();
		ThrowIfFailed(
			DirectX::CreateTexture(Dev.Get(), matadata, &Ptr->m_texture),
			L"�e�N�X�`�����\�[�X�̍쐬�Ɏ��s���܂���\n",
			FileName,
			L"\nBaseTexture::CreateBaseTexture()"
		);

		ThrowIfFailed(
			DirectX::PrepareUpload(Dev.Get(), Ptr->m_image.GetImages(),
				Ptr->m_image.GetImageCount(), matadata, Ptr->m_subresources),
			L"DirectX::PrepareUpload()�Ɏ��s���܂���\n",
			FileName,
			L"\nBaseTexture::CreateBaseTexture()"
		);
		const UINT64 uploadBufferSize = GetRequiredIntermediateSize(
			Ptr->m_texture.Get(),
			0, 
			static_cast<unsigned int>(Ptr->m_subresources.size())
		);
		ThrowIfFailed(
			Dev->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(Ptr->m_textureUploadHeap.GetAddressOf())),
			L"�e�N�X�`���̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���\n",
			FileName,
			L"\nBaseTexture::CreateBaseTexture()"
		);
		Ptr->m_maphandle = maphandle;
		return Ptr;
	}
	void BaseTexture::UpdateSRAndCreateSRV(const ComPtr<ID3D12GraphicsCommandList>& commandList) {
		UpdateSubresources(commandList.Get(),
			m_texture.Get(), m_textureUploadHeap.Get(),
			0, 0, static_cast<unsigned int>(m_subresources.size()),
			m_subresources.data());
		commandList->ResourceBarrier(
			1,
			&CD3DX12_RESOURCE_BARRIER::Transition(m_texture.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE)
		);
		//�e�N�X�`���̃V�F�[�_���\�[�X�r���[���쐬
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//�t�H�[�}�b�g
		srvDesc.Format = m_texture->GetDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = m_texture->GetDesc().MipLevels;
		//�V�F�[�_���\�[�X�r���[
		auto Dev = App::GetID3D12Device();
		Dev->CreateShaderResourceView(
			m_texture.Get(),
			&srvDesc,
			m_maphandle);

	}


}
// end basedx12