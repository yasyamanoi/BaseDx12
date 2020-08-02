#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

	void Player::OnInit() {
		auto texFile = App::GetRelativeAssetsPath() + L"white.png";
		//メッシュ、テクスチャ、コンスタントバッファの作成
		InitDrawResources(texFile);
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

	void Player::ApplyGravity() {
		float elapsedTime = App::GetElapsedTime();
		auto scene = App::GetTypedSceneBase<Scene>();
		m_drawData.m_velocity.y += scene->GetSceneGravity() * elapsedTime;
		m_drawData.m_dirtyflag = true;
	}

	void Player::ApplyPotision() {
		float elapsedTime = App::GetElapsedTime();
		if (m_onObject) {
			m_drawData.m_velocity += m_onObject->GetWorldVelocity();
		}
		m_drawData.m_pos += m_drawData.m_velocity * elapsedTime;
		m_drawData.m_dirtyflag = true;
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
			if (m_stateMachine->GetCurrentState() == PlayerJumpState::Instance()) {
				//空中では制御は抑制する
				m_drawData.m_velocity.x = moveX * m_speedJumpParam;
			}
			else {
				m_drawData.m_velocity.x = moveX * m_speedParam;
			}
			m_drawData.m_dirtyflag = true;
		}
	}

	void Player::ApplySideSlide() {
		if (m_slideObject) {
			m_drawData.m_pos = m_slideObject->GetWorldPosition() + m_moveRelativePos;
			m_drawData.m_beforePos = m_drawData.m_pos;
			m_drawData.m_dirtyflag = true;
		}
	}

	void Player::ChkAndChangeOnObject() {
		if (m_onObject) {
			auto scene = App::GetTypedSceneBase<Scene>();
			auto manager = scene->GetCollisionManager();
			auto ptr = dynamic_cast<MoveSquare*>(m_onObject);
			if (manager->IsOnObject(this, m_onObject)) {
				if (ptr) {
					m_moveRelativePos = m_drawData.m_pos - m_onObject->GetWorldPosition();
					m_slideObject = m_onObject;
				}
			}
			else {
				if (ptr) {
					if (ptr->IsSideJumped()) {
						ptr->ClearSideJumped();
						m_stateMachine->ChangeState(PlayerOutSideState::Instance());
					}
					else {
						m_stateMachine->ChangeState(PlayerJumpState::Instance());
					}
				}
				else {
					m_stateMachine->ChangeState(PlayerJumpState::Instance());
				}
			}
		}
	}

	void Player::ChangeJumpState() {
		m_stateMachine->ChangeState(PlayerJumpState::Instance());
	}


	void Player::OnUpdate() {
		//コントローラチェックして入力があればコマンド呼び出し
		m_inputHandler.PushHandle(this);
		m_stateMachine->Update();
		BaseSquare::OnUpdate();
	}

	void Player::OnPushA() {
		if (m_stateMachine->GetCurrentState() == PlayerOnObjState::Instance()) {
			m_drawData.m_velocity.y += m_jumpVelocity;
			m_stateMachine->ChangeState(PlayerJumpState::Instance());
		}
	}

	void Player::OnCollisionEnter(BaseSquare* other) {
		auto ptrItem = dynamic_cast<ItemSquare*>(other);
		if (ptrItem) {
			return;
		}
		auto scene = App::GetTypedSceneBase<Scene>();
		auto manager = scene->GetCollisionManager();
		if (manager->IsOnObject(this, other)) {
			m_onObject = other;
			m_stateMachine->ChangeState(PlayerOnObjState::Instance());
		}
		else {
			auto ptrTrans = dynamic_cast<TransSquare*>(other);
			if (ptrTrans) {
				m_drawData.m_pos.x *= -0.99f;
				m_drawData.m_beforePos = m_drawData.m_pos;
				m_drawData.m_dirtyflag = true;
			}
		}
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
		owner->ChkAndChangeOnObject();
	}

	void PlayerOnObjState::Exit(Player* owner) {
		owner->ResetExcludeObject();
	}

	//PlayerOutSideState
	shared_ptr<PlayerOutSideState> PlayerOutSideState::Instance() {
		static shared_ptr<PlayerOutSideState> instance(new PlayerOutSideState);
		return instance;
	}


	void PlayerOutSideState::Enter(Player* owner) {
		owner->ApplySideSlide();
	}
	void PlayerOutSideState::Execute(Player* owner) {
		owner->ApplyPotision();
		owner->ChangeJumpState();
	}

	void PlayerOutSideState::Exit(Player* owner) {
	}


}
//end basedx12
