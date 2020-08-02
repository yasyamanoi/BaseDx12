#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

	void ItemSquare::OnInit() {
		auto texFile = App::GetRelativeAssetsPath() + L"Brown.png";
		//メッシュ、テクスチャ、コンスタントバッファの作成
		InitDrawResources(texFile);
		//Collision関連
		{
			m_isFixed = true;
			auto scene = App::GetTypedSceneBase<Scene>();
			scene->RegisterCollsionObject(this);
		}
	}

	void ItemSquare::OnCollisionEnter(BaseSquare* other) {
		auto scene = App::GetTypedSceneBase<Scene>();
		scene->GetCollisionManager()->RemoveObject(this);
		m_drawData.m_emissive = Float4(1.0f);
		m_drawData.m_dirtyflag = true;
	}

}
//end basedx12
