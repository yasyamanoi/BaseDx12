#include "stdafx.h"
#include "Scene.h"

namespace basedx12 {

	void Scene::OnInit() {
		//�t���[������3�ɐݒ�
		ResetActiveBaseDevice<GameDevice>(3);
	}

	void Scene::OnInitAssets() {
		//�����ŃV�[����̃I�u�W�F�N�g���\�z
	}

	void Scene::OnUpdate() {
		//�����ŃV�[����̍X�V�������L�q
	}

	void Scene::OnDraw() {
		//�����ŃV�[����̕`�揈�����L�q
	}

	void Scene::OnDestroy() {
		//�����ŃV�[����̏I���������L�q
	}

}
//end badedx12


