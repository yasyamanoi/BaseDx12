#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

	void CellSquare::OnInit() {
		auto texFile = App::GetRelativeAssetsPath() + L"mapprim.png";
		//���b�V���A�e�N�X�`���A�R���X�^���g�o�b�t�@�̍쐬
		InitDrawResources(texFile);
	}
}
// end basedx12
