#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

	void Player::OnInit() {
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		//メッシュ
		{
			m_ptSquareMesh = CreateSquareMesh(1.0f,1.0f);
		}
		//テクスチャ
		{
			auto TexFile = App::GetRelativeAssetsPath() + L"white.png";
			m_texture = CreateTextureFromFile(TexFile, m_srvIndex);
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
			m_isFixed = false;
			auto scene = App::GetTypedSceneBase<Scene>();
			scene->RegisterCollsionObject(this);
		}
		//StateMachine
		{
			//ステートマシンの構築
			m_stateMachine.reset(new StateMachine<Player>(this));
			//最初のステートをPlayerJumpStateに設定
			m_stateMachine->ChangeState(PlayerJumpState::Instance());
		}
	}

	void Player::UpdateConstdata() {
		Mat4x4 world = GetWorldMatrix();
		m_spriteConstData = {};
		m_spriteConstData.diffuse = Float4(1.0f);
		m_spriteConstData.emissive = Float4(0.0f);
		m_spriteConstData.worldProj = world;
		m_spriteConstData.worldProj *= m_drawData.m_proj;
		m_constantBuffer->Copy(m_spriteConstData);
	}

	void Player::ApplyGravity() {
		float elapsedTime = App::GetElapsedTime();
		auto scene = App::GetTypedSceneBase<Scene>();
		m_drawData.m_velocity.y += scene->GetSceneGravity() * elapsedTime;
		m_drawData.m_dirtyflag = true;
	}

	void Player::ApplyPotision() {
		float elapsedTime = App::GetElapsedTime();
		if (m_onObject) {
			m_drawData.m_pos += m_onObject->GetWorldVelocity() * elapsedTime;
		}
		m_drawData.m_pos += m_drawData.m_velocity * elapsedTime;
		m_drawData.m_dirtyflag = true;

	}

	void Player::ApplyOutStage() {
		//ステージ外に出たときの処理
		float halfW = static_cast<float>(App::GetGameWidth()) / 2.0f;
		float halfWEx = halfW + m_widthMargin;
		if (abs(m_drawData.m_pos.x) >= halfWEx) {
			auto ptr = dynamic_cast<MoveSquare*>(m_onObject);
			if (ptr) {
				static Float3 relativePos(0.0f);
				OBB myOBB = GetOBB();
				OBB onOBB = m_onObject->GetOBB();
				myOBB.m_Center.y -= m_onObjectChkParam;
				if (!HitTest::OBB_OBB(myOBB, onOBB)) {
					m_drawData.m_pos = m_onObject->GetWorldPosition() + relativePos;
				}
				else {
					relativePos = m_drawData.m_pos - m_onObject->GetWorldPosition();
				}
			}
			else {
				m_drawData.m_pos.x *= -1.0f;
			}
		}
	}


	void Player::SetExcludeObject() {
		if (m_onObject) {
			auto scene = App::GetTypedSceneBase<Scene>();
			auto manager = scene->GetCollisionManager();
			manager->SetExcludeObjects(this, m_onObject);
		}
	}
	void Player::ResetExcludeObject() {
		if (m_onObject) {
			auto scene = App::GetTypedSceneBase<Scene>();
			auto manager = scene->GetCollisionManager();
			manager->ResetExcludeObjects(this, m_onObject);
			m_onObject = nullptr;
		}
	}


	void Player::ApplyControlers() {
		float elapsedTime = App::GetElapsedTime();
		auto cntlStats = App::GetControlers();
		if (cntlStats[0].bConnected) {
			float moveX = cntlStats[0].fThumbLX;
			m_drawData.m_velocity.x = moveX * m_speedParam;
			m_drawData.m_dirtyflag = true;
		}
	}

	void Player::ChkAndChangeOnObject() {
		if (m_onObject) {
			auto scene = App::GetTypedSceneBase<Scene>();
			auto manager = scene->GetCollisionManager();
			if (!manager->IsOnObject(this, m_onObject)) {
				m_stateMachine->ChangeState(PlayerJumpState::Instance());
			}
		}
	}


	void Player::OnUpdate() {
		//コントローラチェックして入力があればコマンド呼び出し
		m_inputHandler.PushHandle(this);
		m_stateMachine->Update();
		if (UpdateDrawMatrix()) {
			UpdateConstdata();
		}
	}

	void Player::OnPushA() {
		if (m_stateMachine->GetCurrentState() == PlayerOnObjState::Instance()) {
			m_drawData.m_velocity.y += m_jumpVelocity;
			m_stateMachine->ChangeState(PlayerJumpState::Instance());
		}
	}

	void Player::OnCollisionEnter(BaseSquare* other) {
		auto scene = App::GetTypedSceneBase<Scene>();
		auto manager = scene->GetCollisionManager();
		if (manager->IsOnObject(this, other)) {
			m_onObject = other;
			m_stateMachine->ChangeState(PlayerOnObjState::Instance());
		}
	}


	void Player::OnDraw() {
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		auto scene = App::GetTypedSceneBase<Scene>();
		//パイプライステート
		scene->SetGPUPipelineState();
		//Sampler
		scene->SetGPULinearClampSampler();
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

	//PlayerJumpState
	shared_ptr<PlayerJumpState> PlayerJumpState::Instance() {
		static shared_ptr<PlayerJumpState> instance(new PlayerJumpState);
		return instance;
	}
	void PlayerJumpState::Enter(Player* owner) {
	}

	void PlayerJumpState::Execute(Player* owner) {
		owner->ApplyGravity();
		owner->ApplyControlers();
		owner->ApplyPotision();
		owner->ApplyOutStage();
	}

	void PlayerJumpState::Exit(Player* owner) {
	}


	//PlayerOnObjState
	shared_ptr<PlayerOnObjState> PlayerOnObjState::Instance() {
		static shared_ptr<PlayerOnObjState> instance(new PlayerOnObjState);
		return instance;
	}
	void PlayerOnObjState::Enter(Player* owner) {
		owner->SetExcludeObject();
	}
	void PlayerOnObjState::Execute(Player* owner) {
		owner->ApplyControlers();
		owner->ApplyPotision();
		owner->ApplyOutStage();
		owner->ChkAndChangeOnObject();
	}

	void PlayerOnObjState::Exit(Player* owner) {
		owner->ResetExcludeObject();
	}

}
//end basedx12
