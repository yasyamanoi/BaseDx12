#include "stdafx.h"

namespace basedx12 {

	shared_ptr<BaseTexture>  BaseTexture::CreateBaseTexture(const wstring& fileName, const CD3DX12_CPU_DESCRIPTOR_HANDLE& mapHandle) {
		if (fileName == L"") {
			throw BaseException(
				L"ファイルが指定されていません\n",
				L"BaseTexture::CreateBaseTexture()"
			);
		}
		DWORD retCode;
		retCode = GetFileAttributes(fileName.c_str());
		if (retCode == -1) {
			throw BaseException(
				L"ファイルが存在しません\n",
				fileName,
				L"\nBaseTexture::CreateBaseTexture()"
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

		_wsplitpath_s(fileName.c_str(),
			Drivebuff, _MAX_DRIVE,
			Dirbuff, _MAX_DIR,
			FileNamebuff, _MAX_FNAME,
			Extbuff, _MAX_EXT);

		wstring extStr = Extbuff;
		shared_ptr<BaseTexture> ptrTexture = shared_ptr<BaseTexture>(new BaseTexture());
		TexMetadata matadata;

		if (extStr == L".dds" || extStr == L".DDS") {
			ThrowIfFailed(
				DirectX::LoadFromDDSFile(fileName.c_str(), DDS_FLAGS_NONE, &matadata, ptrTexture->m_image),
				L"テクスチャの読み込みに失敗しました\n",
				fileName,
				L"\nBaseTexture::CreateBaseTexture()"
			);
		}
		else if (extStr == L".tga" || extStr == L".TGA") {
			ThrowIfFailed(
				DirectX::LoadFromTGAFile(fileName.c_str(), &matadata, ptrTexture->m_image),
				L"テクスチャの読み込みに失敗しました\n",
				fileName,
				L"\nBaseTexture::CreateBaseTexture()"
			);
		}
		else if (extStr == L".hdr" || extStr == L".HDR")
		{
			ThrowIfFailed(
				DirectX::LoadFromHDRFile(fileName.c_str(), &matadata, ptrTexture->m_image),
				L"テクスチャの読み込みに失敗しました\n",
				fileName,
				L"\nBaseTexture::CreateBaseTexture()"
			);
		}
		else {
			ThrowIfFailed(
				DirectX::LoadFromWICFile(fileName.c_str(), WIC_FLAGS_NONE, &matadata, ptrTexture->m_image),
				L"テクスチャの読み込みに失敗しました\n",
				fileName,
				L"\nBaseTexture::CreateBaseTexture()"
			);
		}
		//デバイスの取得
		auto device = App::GetID3D12Device();
		ThrowIfFailed(
			DirectX::CreateTexture(device.Get(), matadata, &ptrTexture->m_texture),
			L"テクスチャリソースの作成に失敗しました\n",
			fileName,
			L"\nBaseTexture::CreateBaseTexture()"
		);

		ThrowIfFailed(
			DirectX::PrepareUpload(device.Get(), ptrTexture->m_image.GetImages(),
				ptrTexture->m_image.GetImageCount(), matadata, ptrTexture->m_subresources),
			L"DirectX::PrepareUpload()に失敗しました\n",
			fileName,
			L"\nBaseTexture::CreateBaseTexture()"
		);
		const UINT64 uploadBufferSize = GetRequiredIntermediateSize(
			ptrTexture->m_texture.Get(),
			0, 
			static_cast<unsigned int>(ptrTexture->m_subresources.size())
		);
		ThrowIfFailed(
			device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(ptrTexture->m_textureUploadHeap.GetAddressOf())),
			L"テクスチャのアップロードヒープ作成に失敗しました\n",
			fileName,
			L"\nBaseTexture::CreateBaseTexture()"
		);
		ptrTexture->m_maphandle = mapHandle;
		return ptrTexture;
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
		//テクスチャのシェーダリソースビューを作成
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//フォーマット
		srvDesc.Format = m_texture->GetDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = m_texture->GetDesc().MipLevels;
		//シェーダリソースビュー
		auto device = App::GetID3D12Device();
		device->CreateShaderResourceView(
			m_texture.Get(),
			&srvDesc,
			m_maphandle);

	}


}
// end basedx12