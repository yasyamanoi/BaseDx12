#pragma once
#include "stdafx.h"

namespace basedx12 {

	class Dx12Texture {
	public:
		Dx12Texture(){}
		~Dx12Texture(){}
		static ComPtr<ID3D12Resource> CreateDx12Texture(ID3D12Device* pDevice,
			const wstring& FileName, DirectX::ScratchImage& image);
	};

}
//end basedx12
