#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

	void MoveSquare::OnInit() {
		auto texFile = App::GetRelativeAssetsPath() + L"Blue.png";
		//メッシュ、テクスチャ、コンスタントバッファの作成
		InitDrawResources(texFile);
		//Collision関連
		{
			m_isFixed = true;
			auto scene = App::GetTypedSceneBase<Scene>();
			scene->RegisterCollsionObject(this);
		}
	}

	void MoveSquare::OnUpdate() {
		float elapsedTime = App::GetElapsedTime();
		m_drawData.m_velocity = m_moveVelocity;
		m_drawData.m_pos.x += m_drawData.m_velocity.x * elapsedTime;
		m_drawData.m_dirtyflag = true;
		BaseSquare::OnUpdate();
	}

	void MoveSquare::OnCollisionEnter(BaseSquare* other) {
		auto ptr = dynamic_cast<TransSquare*>(other);
		if (ptr) {
			m_drawData.m_pos.x *= -0.99f;
			m_drawData.m_beforePos = m_drawData.m_pos;
			m_sideJumped = true;
		}
		else {
			m_sideJumped = false;
		}
	}

}
//end basedx12
