#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

	void TransSquare::OnInit() {
		auto texFile = App::GetRelativeAssetsPath() + L"purple.png";
		//メッシュ、テクスチャ、コンスタントバッファの作成
		InitDrawResources(texFile);
		//Collision関連
		{
			m_isFixed = true;
			auto scene = App::GetTypedSceneBase<Scene>();
			scene->RegisterCollsionObject(this);
		}
	}

}
//end basedx12
