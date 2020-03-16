#pragma once
#include "GameDevice.h"

namespace basedx12 {

	DECLARE_DX12SHADER(VSPCSprite)
	DECLARE_DX12SHADER(PSPCSprite)


	class Scene :public SceneBase {
		ComPtr<ID3D12PipelineState> m_pipelineState;
		shared_ptr<Dx12Mesh> m_dx12Mesh;
	public:
		Scene() :SceneBase() {}
		virtual ~Scene() {}
		virtual void OnInit()override;
		virtual void OnInitAssets()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override;
		virtual void OnDestroy()override;
	};

}
//end basedx12


