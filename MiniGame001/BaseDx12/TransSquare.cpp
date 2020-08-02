#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

	void TransSquare::OnInit() {
		auto texFile = App::GetRelativeAssetsPath() + L"purple.png";
		//���b�V���A�e�N�X�`���A�R���X�^���g�o�b�t�@�̍쐬
		InitDrawResources(texFile);
		//Collision�֘A
		{
			m_isFixed = true;
			auto scene = App::GetTypedSceneBase<Scene>();
			scene->RegisterCollsionObject(this);
		}
	}

}
//end basedx12
