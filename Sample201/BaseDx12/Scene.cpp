#include "stdafx.h"
#include "GameStage.h"
#include "Scene.h"


namespace basedx12 {

	void Scene::OnInit() {
		//�����ł͎g�p����f�o�C�X�����߂�
		ResetActiveDx12Device<Default2DDivece>(3);
	}
	void Scene::OnInitAssets() {
		//�����ł͍ŏ��ɐݒu����X�e�[�W�����߂�
        auto stagePtr = ResetActiveStage<GameStage>();
    }


}
//end badedx12


