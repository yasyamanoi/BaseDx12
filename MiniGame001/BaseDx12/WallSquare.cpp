#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

	void WallSquare::OnInit() {
		auto texFile = App::GetRelativeAssetsPath() + L"Brown2.png";
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
