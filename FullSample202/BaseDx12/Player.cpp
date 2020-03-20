#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

	void Player::OnInit() {
		auto ptr = GetComponent<Transform>();
		ptr->SetScale(0.25f, 0.25f, 0.25f);	//直径25センチの球体
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(Float3(0, 0.125f, 0));
		auto compPtr = AddComponent<PNTStaticDraw>();
		//メッシュ
		vector<VertexPositionNormalTexture> vertices;
		vector<uint32_t> indices;
		MeshUtill::CreateSphere(1.0f, 18,vertices, indices);
		auto mesh = BaseMesh::CreateBaseMesh<VertexPositionNormalTexture>(vertices, indices);
		compPtr->SetMesh(mesh);
		//テクスチャ
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
//				float moveLength = 0;	//動いた時のスピード
				auto ptrTransform = GetComponent<Transform>();
				//進行方向の向きを計算
				auto front = ptrTransform->GetPosition() - camera->GetEye();
				front.y = 0;
				front.normalize();
				//進行方向向きからの角度を算出
				float frontAngle = atan2(front.z, front.x);
				//コントローラの向き計算
				Float2 moveVec(moveX, moveZ);
				float moveSize = moveVec.length();
				//コントローラの向きから角度を計算
				float cntlAngle = atan2(-moveX, moveZ);
				//トータルの角度を算出
				float totalAngle = frontAngle + cntlAngle;
				//角度からベクトルを作成
				angle = Float3(cos(totalAngle), 0, sin(totalAngle));
				//正規化する
				angle.normalize();
				//移動サイズを設定。
				angle *= moveSize;
				//Y軸は変化させない
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
		//回転の計算
		//if (angle.length() > 0.0f) {
		//	auto utilPtr = GetBehavior<UtilBehavior>();
		//	utilPtr->RotToHead(angle, 1.0f);
		//}
	}

}
//end basedx12
