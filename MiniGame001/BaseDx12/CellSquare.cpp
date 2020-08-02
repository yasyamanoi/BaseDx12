#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

	void CellSquare::OnInit() {
		auto texFile = App::GetRelativeAssetsPath() + L"mapprim.png";
		//メッシュ、テクスチャ、コンスタントバッファの作成
		InitDrawResources(texFile);
	}
}
// end basedx12
