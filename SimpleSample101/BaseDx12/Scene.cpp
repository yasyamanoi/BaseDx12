#include "stdafx.h"
#include "Scene.h"

namespace basedx12 {
	IMPLEMENT_DX12SHADER(VSPCSprite, App::GetShadersPath() + L"vshader.cso")
	IMPLEMENT_DX12SHADER(PSPCSprite, App::GetShadersPath() + L"pshader.cso")


	void Scene::OnInit() {
		ResetActiveBaseDevice<GameDivece>(2);
	}

	void Scene::OnInitAssets() {
		//ここでシーン上のオブジェクトを構築
		//デバイスの取得
		auto Device = App::GetBaseDevice();
		// ２Ｄの基本的なパイプライン
		{
			D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
			m_pipelineState
				= PipelineState::CreateDefault2D<VertexPositionColor, VSPCSprite, PSPCSprite>(Device->GetRootSignature(), PipeLineDesc);
		}
		// メッシュ
		{
			auto aspectRatio = Device->GetAspectRatio();
			vector<VertexPositionColor> vertex =
			{
				{ Float3(0.0f, 0.25f * aspectRatio, 0.0f), Float4(1.0f, 0.0f, 0.0f, 1.0f) },
				{ Float3(0.25f, -0.25f * aspectRatio, 0.0f), Float4(0.0f, 1.0f, 0.0f, 1.0f) },
				{ Float3(-0.25f, -0.25f * aspectRatio, 0.0f), Float4(0.0f, 0.0f, 1.0f, 1.0f) }
			};
			//メッシュ作成
			m_baseMesh = BaseMesh::CreateBaseMesh<VertexPositionColor>(vertex);
		}
	}

	void Scene::OnUpdate() {
	}

	void Scene::OnDraw() {
		auto Device = App::GetBaseDevice();
		auto commandList = Device->GetCommandList();
		commandList->SetPipelineState(m_pipelineState.Get());
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commandList->IASetVertexBuffers(0, 1, &m_baseMesh->GetVertexBufferView());
		commandList->DrawInstanced(3, 1, 0, 0);
	}

	void Scene::OnDestroy() {
	}

}
//end badedx12


