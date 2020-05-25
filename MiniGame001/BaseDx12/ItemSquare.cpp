#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

	void ItemSquare::OnInit() {
		auto texFile = App::GetRelativeAssetsPath() + L"Brown.png";
		//���b�V���A�e�N�X�`���A�R���X�^���g�o�b�t�@�̍쐬
		InitDrawResources(texFile);
		//Collision�֘A
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
