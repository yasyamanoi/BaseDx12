#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

	void Player::OnInit() {
		auto ptr = GetComponent<Transform>();
		ptr->SetScale(0.25f, 0.25f, 0.25f);	//���a25�Z���`�̋���
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(Float3(0, 0.125f, 0));
		auto compPtr = AddComponent<PNTStaticDraw>();
		//���b�V��
		vector<VertexPositionNormalTexture> vertices;
		vector<uint32_t> indices;
		MeshUtill::CreateSphere(1.0f, 18,vertices, indices);
		auto mesh = BaseMesh::CreateBaseMesh<VertexPositionNormalTexture>(vertices, indices);
		compPtr->SetMesh(mesh);
		//�e�N�X�`��
		auto TexFile = App::GetRelativeAssetsPath() + L"trace.png";
		compPtr->SetTextureFile(TexFile);
		compPtr->SetCameraKey(L"MainCamera");
		compPtr->SetLightSetKey(L"MainLightSet");
		auto camera = GetStage()->GetTypedCamera<MyCamera>(L"MainCamera");
		camera->SetTargetObject(GetThis<GameObject>());
		camera->SetTargetToAt(Float3(0, 0.25f, 0));
	}

	Float3 Player::GetMoveVector() const{
		Float3 angle(0, 0, 0);
		auto camera = GetStage()->GetCamera(L"MainCamera");
		auto cntlStats = App::GetControlers();
		if (cntlStats[0].bConnected) {
			float moveX = cntlStats[0].fThumbLX;
			float moveZ = cntlStats[0].fThumbLY;
			if (moveX != 0 || moveZ != 0) {
//				float moveLength = 0;	//���������̃X�s�[�h
				auto ptrTransform = GetComponent<Transform>();
				//�i�s�����̌������v�Z
				auto front = ptrTransform->GetPosition() - camera->GetEye();
				front.y = 0;
				front.normalize();
				//�i�s������������̊p�x���Z�o
				float frontAngle = atan2(front.z, front.x);
				//�R���g���[���̌����v�Z
				Float2 moveVec(moveX, moveZ);
				float moveSize = moveVec.length();
				//�R���g���[���̌�������p�x���v�Z
				float cntlAngle = atan2(-moveX, moveZ);
				//�g�[�^���̊p�x���Z�o
				float totalAngle = frontAngle + cntlAngle;
				//�p�x����x�N�g�����쐬
				angle = Float3(cos(totalAngle), 0, sin(totalAngle));
				//���K������
				angle.normalize();
				//�ړ��T�C�Y��ݒ�B
				angle *= moveSize;
				//Y���͕ω������Ȃ�
				angle.y = 0;
			}
		}
		return angle;
	}


    void Player::OnUpdate() {
		float elapsedTime = App::GetElapsedTime();
		auto angle = GetMoveVector();
		if (angle.length() > 0.0f) {
			auto trans = GetComponent<Transform>();
			trans->AddPosition(angle * elapsedTime * m_Speed);
		}
		//��]�̌v�Z
		//if (angle.length() > 0.0f) {
		//	auto utilPtr = GetBehavior<UtilBehavior>();
		//	utilPtr->RotToHead(angle, 1.0f);
		//}
	}

}
//end basedx12
