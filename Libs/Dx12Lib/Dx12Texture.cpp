#include "stdafx.h"

namespace basedx12 {
/*
		//�e�N�X�`���p�̃f�X�N�v���^
		D3D12_RESOURCE_DESC m_TextureResDesc;
		//���\�[�X
		ComPtr<ID3D12Resource> m_Texture;
		//GPU�A�b�v���[�h�̂��߂̃��\�[�X
		ComPtr<ID3D12Resource> m_TextureUploadHeap;
		//�e�N�X�`���t�@�C���ւ̃p�X
		wstring m_FileName{ L"" };
		//�e�N�X�`���f�[�^
		vector<uint8_t> m_Data;
		UINT m_TexturePixelSize;
		bool m_DataRefresh;
*/
	ComPtr<ID3D12Resource> Dx12Texture::CreateDx12Texture(ID3D12Device* pDevice,
		const wstring& FileName, 
		DirectX::ScratchImage& image) {
		if (FileName == L"") {
			throw BaseException(
				L"�t�@�C�����w�肳��Ă��܂���\n",
				L"Dx12Texture::CreateDx12Texture()"
			);
		}
		DWORD RetCode;
		RetCode = GetFileAttributes(FileName.c_str());
		if (RetCode == -1) {
			throw BaseException(
				L"�t�@�C�������݂��܂���\n",
				FileName,
				L"Dx12Texture::CreateDx12Texture()"
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

		if (ExtStr == L".dds" || ExtStr == L".DDS") {
			ThrowIfFailed(
				DirectX::LoadFromDDSFile(FileName.c_str(), DDS_FLAGS_NONE, nullptr, image),
				L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
				FileName,
				L"Dx12Texture::CreateDx12Texture()"
			);
		}
		else if (ExtStr == L".tga" || ExtStr == L".TGA") {
			ThrowIfFailed(
				DirectX::LoadFromTGAFile(FileName.c_str(), nullptr, image),
				L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
				FileName,
				L"Dx12Texture::CreateDx12Texture()"
			);
		}
		else if (ExtStr == L".hdr" || ExtStr == L".HDR")
		{
			ThrowIfFailed(
				DirectX::LoadFromHDRFile(FileName.c_str(), nullptr, image),
				L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
				FileName,
				L"Dx12Texture::CreateDx12Texture()"
			);
		}
		else {
			ThrowIfFailed(
				DirectX::LoadFromWICFile(FileName.c_str(), WIC_FLAGS_NONE, nullptr, image),
				L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
				FileName,
				L"Dx12Texture::CreateDx12Texture()"
			);
		}
		ComPtr<ID3D12Resource> texture;
		ThrowIfFailed(
			DirectX::CreateTexture(pDevice, image.GetMetadata(), &texture),
			L"�e�N�X�`�����\�[�X�̍쐬�Ɏ��s���܂���",
			FileName,
			L"Dx12Texture::CreateDx12Texture()"
		);
		return texture;
	}

}
// end basedx12