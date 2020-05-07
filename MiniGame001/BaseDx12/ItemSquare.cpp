#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

	void ItemSquare::OnInit() {
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		//メッシュ
		{
			m_ptSquareMesh = CreateSquareMesh(m_initData.m_scale.x, m_initData.m_scale.y);
		}
		//テクスチャ
		{
			auto texFile = App::GetRelativeAssetsPath() + L"Brown.png";
			m_texture = CreateTextureFromFile(texFile, m_srvIndex);
		}
		//コンスタントバッファ
		{
			m_constantBuffer = CreateConstantBuffer(m_spriteConstData, m_constBuffIndex);
		}
		//DrawData関連
		{
			SetInitDrawData();
			if (UpdateDrawMatrix()) {
				UpdateConstdata();
			}
		}
		//Collision関連
		{
			m_isFixed = true;
			auto scene = App::GetTypedSceneBase<Scene>();
			scene->RegisterCollsionObject(this);
		}
	}

	void ItemSquare::UpdateConstdata() {
		m_spriteConstData = {};
		m_spriteConstData.diffuse = m_drawData.m_diffuse;
		m_spriteConstData.emissive = m_drawData.m_emissive;
		m_spriteConstData.worldProj = m_drawData.m_world;
		m_spriteConstData.worldProj *= m_drawData.m_proj;
		m_constantBuffer->Copy(m_spriteConstData);
	}

	void ItemSquare::OnCollisionEnter(BaseSquare* other) {
		auto scene = App::GetTypedSceneBase<Scene>();
		scene->GetCollisionManager()->RemoveObject(this);
		m_drawData.m_emissive = Float4(1.0f);
		m_drawData.m_dirtyflag = true;
	}


	void ItemSquare::OnUpdate() {
		if (UpdateDrawMatrix()) {
			UpdateConstdata();
		}
	}

	void ItemSquare::OnDraw() {
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		auto scene = App::GetTypedSceneBase<Scene>();
		//パイプライステート
		scene->SetGPUPipelineState();
		//Sampler
		scene->SetGPULinearWrapSampler();
		//テクスチャの更新とシェーダリソースビューの作成
		m_texture->UpdateSRAndCreateSRV(commandList);
		//Srvのハンドルの設定
		CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_srvIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		//RootSignature上のt0に設定
		commandList->SetGraphicsRootDescriptorTable(baseDevice->GetGpuSlotID(L"t0"), srvHandle);
		//Cbvのハンドルを設定
		CD3DX12_GPU_DESCRIPTOR_HANDLE cbvHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		//RootSignature上のb0に設定
		commandList->SetGraphicsRootDescriptorTable(baseDevice->GetGpuSlotID(L"b0"), cbvHandle);
		//描画処理
		commandList->IASetVertexBuffers(0, 1, &m_ptSquareMesh->GetVertexBufferView());
		commandList->IASetIndexBuffer(&m_ptSquareMesh->GetIndexBufferView());
		commandList->DrawIndexedInstanced(m_ptSquareMesh->GetNumIndices(), 1, 0, 0, 0);
	}


}
//end basedx12
