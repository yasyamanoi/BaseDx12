#pragma once
#include "GameDevice.h"

namespace basedx12 {

	DECLARE_DX12SHADER(VSPCSprite)
	DECLARE_DX12SHADER(PSPCSprite)


	class Scene :public SceneBase {
		ComPtr<ID3D12PipelineState> m_pipelineState;
		shared_ptr<Dx12Mesh> m_Dx12Mesh;
	public:
		Scene() :SceneBase() {}
		virtual ~Scene() {}
		virtual void OnInit();
		virtual void OnInitAssets();
		virtual void OnUpdate();
		virtual void OnRender();
		virtual void OnDestroy();
	};

}
//end basedx12


