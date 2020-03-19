#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

	//--------------------------------------------------------------------------------------
	///	物理計算する固定のボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	FixedPsBox::FixedPsBox(const shared_ptr<Stage>& StagePtr,
		const Float3& Scale,
		const Quat& Qt,
		const Float3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	FixedPsBox::~FixedPsBox() {}
	//初期化
	void FixedPsBox::OnInit() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(m_Scale);
		ptrTrans->SetQuaternion(m_Qt);
		ptrTrans->SetPosition(m_Position);

		auto compPtr = AddComponent<PNTStaticDraw>();
		//メッシュ
		vector<VertexPositionNormalTexture> vertices;
		vector<uint32_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		auto mesh = Dx12Mesh::CreateDx12Mesh<VertexPositionNormalTexture>(vertices, indices);
		compPtr->SetMesh(mesh);
		//テクスチャ
		auto TexFile = App::GetRelativeAssetsPath() + L"sky.jpg";
		compPtr->SetTextureFile(TexFile);
		compPtr->SetCameraKey(L"MainCamera");
		compPtr->SetLightSetKey(L"MainLightSet");
		//物理計算ボックス
		PsBoxParam param(ptrTrans->GetWorldMatrix(), 0.0f, true, PsMotionType::MotionTypeFixed);
		auto PsPtr = AddComponent<RigidbodyBox>(param);
	}

	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブなボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	ActivePsBox::ActivePsBox(const shared_ptr<Stage>& StagePtr,
		const Float3& Scale,
		const Quat& Qt,
		const Float3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	ActivePsBox::~ActivePsBox() {}
	//初期化
	void ActivePsBox::OnInit() {
		auto ptrTrans = GetComponent<Transform>();

		ptrTrans->SetScale(m_Scale);
		ptrTrans->SetQuaternion(m_Qt);
		ptrTrans->SetPosition(m_Position);


		auto compPtr = AddComponent<PNTStaticDraw>();
		//メッシュ
		vector<VertexPositionNormalTexture> vertices;
		vector<uint32_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		auto mesh = Dx12Mesh::CreateDx12Mesh<VertexPositionNormalTexture>(vertices, indices);
		compPtr->SetMesh(mesh);
		//テクスチャ
		auto TexFile = App::GetRelativeAssetsPath() + L"sky.jpg";
		compPtr->SetTextureFile(TexFile);
		compPtr->SetCameraKey(L"MainCamera");
		compPtr->SetLightSetKey(L"MainLightSet");
		//物理計算ボックス
		PsBoxParam param(ptrTrans->GetWorldMatrix(), 1.0f, true, PsMotionType::MotionTypeActive);
		auto PsPtr = AddComponent<RigidbodyBox>(param);
	}

	void MoveBox::OnInit() {
		auto Device = App::GetDx12Device();
		auto compPtr = AddComponent<PNTStaticDraw>();
		//メッシュ
		vector<VertexPositionNormalTexture> vertices;
		vector<uint32_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		auto mesh = Dx12Mesh::CreateDx12Mesh<VertexPositionNormalTexture>(vertices, indices);
		compPtr->SetMesh(mesh);
		//テクスチャ
		auto TexFile = App::GetRelativeAssetsPath() + L"sky.jpg";
		compPtr->SetTextureFile(TexFile);
		compPtr->SetCameraKey(L"MainCamera");
		compPtr->SetLightSetKey(L"MainLightSet");

		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Float3(1.0f));
		ptrTrans->SetPosition(Float3(0.0f));
	}

	void MoveBox::OnUpdate() {
		auto ptrTrans = GetComponent<Transform>();
		auto qt = ptrTrans->GetQuaternion();
		Quat qtspan(Float3(0, 1, 1), -0.02f);
		qt *= qtspan;
		qt.normalize();
		ptrTrans->SetQuaternion(qt);
		auto pos = ptrTrans->GetPosition();
		pos.x += m_posSpan;
		if (abs(pos.x) >= 5.0f) {
			m_posSpan *= -1.0f;
		}
		ptrTrans->SetPosition(pos);
	}

}
//end basedx12
