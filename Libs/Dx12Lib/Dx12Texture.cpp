#include "stdafx.h"

namespace basedx12 {
/*
		//テクスチャ用のデスクプリタ
		D3D12_RESOURCE_DESC m_TextureResDesc;
		//リソース
		ComPtr<ID3D12Resource> m_Texture;
		//GPUアップロードのためのリソース
		ComPtr<ID3D12Resource> m_TextureUploadHeap;
		//テクスチャファイルへのパス
		wstring m_FileName{ L"" };
		//テクスチャデータ
		vector<uint8_t> m_Data;
		UINT m_TexturePixelSize;
		bool m_DataRefresh;
*/
	ComPtr<ID3D12Resource> Dx12Texture::CreateDx12Texture(ID3D12Device* pDevice,
		const wstring& FileName, 
		DirectX::ScratchImage& image) {
		if (FileName == L"") {
			throw BaseException(
				L"ファイルが指定されていません\n",
				L"Dx12Texture::CreateDx12Texture()"
			);
		}
		DWORD RetCode;
		RetCode = GetFileAttributes(FileName.c_str());
		if (RetCode == -1) {
			throw BaseException(
				L"ファイルが存在しません\n",
				FileName,
				L"Dx12Texture::CreateDx12Texture()"
			);
		}
		//テクスチャ作成
		//ファイル拡張子の調査
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
				L"テクスチャの読み込みに失敗しました",
				FileName,
				L"Dx12Texture::CreateDx12Texture()"
			);
		}
		else if (ExtStr == L".tga" || ExtStr == L".TGA") {
			ThrowIfFailed(
				DirectX::LoadFromTGAFile(FileName.c_str(), nullptr, image),
				L"テクスチャの読み込みに失敗しました",
				FileName,
				L"Dx12Texture::CreateDx12Texture()"
			);
		}
		else if (ExtStr == L".hdr" || ExtStr == L".HDR")
		{
			ThrowIfFailed(
				DirectX::LoadFromHDRFile(FileName.c_str(), nullptr, image),
				L"テクスチャの読み込みに失敗しました",
				FileName,
				L"Dx12Texture::CreateDx12Texture()"
			);
		}
		else {
			ThrowIfFailed(
				DirectX::LoadFromWICFile(FileName.c_str(), WIC_FLAGS_NONE, nullptr, image),
				L"テクスチャの読み込みに失敗しました",
				FileName,
				L"Dx12Texture::CreateDx12Texture()"
			);
		}
		ComPtr<ID3D12Resource> texture;
		ThrowIfFailed(
			DirectX::CreateTexture(pDevice, image.GetMetadata(), &texture),
			L"テクスチャリソースの作成に失敗しました",
			FileName,
			L"Dx12Texture::CreateDx12Texture()"
		);
		return texture;
	}

}
// end basedx12