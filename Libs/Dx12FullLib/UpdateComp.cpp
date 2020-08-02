#include "stdafx.h"

namespace basedx12 {

	shared_ptr<Stage> Component::GetStage() const {
		return GetGameObject()->GetStage();
	}

	void Component::AttachGameObject(const shared_ptr<GameObject>& GameObjectPtr) {
		m_gameObject = GameObjectPtr;
	}


	Transform::Transform(const shared_ptr<GameObject>& gameObjectPtr):
		Component(gameObjectPtr),
		m_beforeScale(1.0f),
		m_beforePivot(),
		m_beforeQuaternion(),
		m_beforePosition(),
		m_scale(1.0f),
		m_pivot(),
		m_quaternion(),
		m_position(),
		m_worldMatrix(),
		m_dirtyFlg(true)
	{}
	Transform::~Transform() {}

	//アクセサ
	//BeforeGetter
	Float3 Transform::GetBeforeScale() const {
		return m_beforeScale;
	}

	Float3 Transform::GetBeforePivot() const {
		return m_beforePivot;
	}

	Quat Transform::GetBeforeQuaternion() const {
		return m_beforeQuaternion;
	}

	Float3 Transform::GetBeforeRotation() const {
		return m_beforeQuaternion.toRotVec();
	}

	Float3 Transform::GetBeforePosition() const {
		return m_beforePosition;
	}

	Float3 Transform::GetBeforeWorldPosition() const {
		return GetBeforeWorldMatrix().transInMatrix();
	}

	bool Transform::IsSameBeforeWorldMatrix(const bsm::Mat4x4& mat) const {
		return mat.equalInt(GetBeforeWorldMatrix());
	}


	const Mat4x4 Transform::GetBeforeWorldMatrix() const {
		auto ParPtr = GetParent();
		Mat4x4 BefWorld;
		BefWorld.affineTransformation(
			m_beforeScale,
			m_beforePivot,
			m_beforeQuaternion,
			m_beforePosition
		);
		if (ParPtr) {
			auto ParBeforeWorld = ParPtr->GetComponent<Transform>()->GetBeforeWorldMatrix();
			ParBeforeWorld.scaleIdentity();
			BefWorld *= ParBeforeWorld;
		}
		return BefWorld;
	}


	//Getter&Setter

	Float3 Transform::GetScale() const {
		return m_scale;
	}

	void Transform::SetScale(const Float3& scale) {
		if (!scale.isNaN() && !scale.isInfinite()) {
			m_scale = scale;
			m_dirtyFlg = true;
		}
	}
	void Transform::SetScale(float x, float y, float z) {
		SetScale(Float3(x, y, z));
	}

	Float3 Transform::GetPivot() const {
		return m_pivot;
	}
	void Transform::SetPivot(const Float3& pivot) {
		if (!pivot.isNaN() && !pivot.isInfinite()) {
			m_pivot = pivot;
			m_dirtyFlg = true;
		}
	}
	void Transform::SetPivot(float x, float y, float z) {
		SetPivot(Float3(x, y, z));
	}

	Quat Transform::GetQuaternion() const {
		return m_quaternion;
	}
	void Transform::SetQuaternion(const bsm::Quat& quaternion) {
		if (!quaternion.isNaN() && !quaternion.isInfinite()) {
			m_quaternion = quaternion;
			m_quaternion.normalize();
			m_dirtyFlg = true;
		}
	}
	Float3 Transform::GetRotation() const {
		Float3 r = m_quaternion.toRotVec();
		r.normalize();
		return r;
	}

	void Transform::SetRotation(const Float3& rot) {
		if (!rot.isNaN() && !rot.isInfinite()) {
			Quat qt;
			qt.rotationRollPitchYawFromVector(rot);
			SetQuaternion(qt);
		}
	}
	void Transform::SetRotation(float x, float y, float z) {
		SetRotation(Float3(x, y, z));
	}

	void Transform::AddPosition(const Float3& addpos) {
		if (!addpos.isNaN() && !addpos.isInfinite()) {
			m_position += addpos;
			m_dirtyFlg = true;
		}
	}


	Float3 Transform::GetPosition() const {
		return m_position;
	}

	void Transform::SetPosition(const Float3& position) {
		if (!position.isNaN() && !position.isInfinite()) {
			m_position = position;
			m_dirtyFlg = true;
		}
	}
	void Transform::SetPosition(float x, float y, float z) {
		SetPosition(Float3(x, y, z));
	}

	void Transform::ResetPosition(const Float3& position) {
		if (!position.isNaN() && !position.isInfinite()) {
			m_beforePosition = position;
			m_position = position;
			m_dirtyFlg = true;
		}
	}

	Float3 Transform::GetWorldPosition(){
		return GetWorldMatrix().transInMatrix();
	}
	void Transform::SetWorldPosition(const Float3& position) {
		auto SetPos = position;
		auto parPtr = GetParent();
		if (parPtr) {
			auto parWorldPos = parPtr->GetComponent<Transform>()->GetWorldMatrix().transInMatrix();
			SetPos -= parWorldPos;
			auto parQt = parPtr->GetComponent<Transform>()->GetWorldMatrix().quatInMatrix();
			parQt = bsm::inverse(parQt);
			Mat4x4 parQtMat(parQt);
			SetPos *= parQtMat;
		}
		SetPosition(SetPos);
	}
	void Transform::ResetWorldPosition(const Float3& position) {
		auto setPos = position;
		auto parPtr = GetParent();
		if (parPtr) {
			auto parWorldPos = parPtr->GetComponent<Transform>()->GetWorldMatrix().transInMatrix();
			setPos -= parWorldPos;
			auto parQt = parPtr->GetComponent<Transform>()->GetWorldMatrix().quatInMatrix();
			parQt = bsm::inverse(parQt);
			Mat4x4 parQtMat(parQt);
			setPos *= parQtMat;
		}
		ResetPosition(setPos);
	}

	bool Transform::IsSameWorldMatrix(const bsm::Mat4x4& mat) {
		return mat.equalInt(GetWorldMatrix());
	}


	const Mat4x4& Transform::GetWorldMatrix() {
		if (m_dirtyFlg) {
			auto parPtr = GetParent();
			//Mat4x4 WorldMat;
			m_worldMatrix.affineTransformation(
				m_scale,
				m_pivot,
				m_quaternion,
				m_position
			);
			if (parPtr) {
				auto parWorld = parPtr->GetComponent<Transform>()->GetWorldMatrix();
				parWorld.scaleIdentity();
				m_worldMatrix *= parWorld;
			}
			m_dirtyFlg = false;
		}
		return m_worldMatrix;
	}

	const Mat4x4& Transform::Get2DWorldMatrix() {
		if (m_dirtyFlg) {
			auto parPtr = GetParent();
			m_scale.z = 1.0f;
			Float4 temp_z(m_position.z);
			temp_z = XMVector4ClampLength(temp_z, 0.0f, 1.0f);
			m_position.z = temp_z.z;
			m_pivot.z = 0;
			//Mat4x4 WorldMat;
			m_worldMatrix.affineTransformation(
				m_scale,
				m_pivot,
				m_quaternion,
				m_position
			);
			if (parPtr) {
				auto parWorld = parPtr->GetComponent<Transform>()->Get2DWorldMatrix();
				parWorld.scaleIdentity();
				m_worldMatrix *= parWorld;
			}
			m_dirtyFlg = false;
		}
		return m_worldMatrix;
	}


	shared_ptr<GameObject> Transform::GetParent()const {
		auto shPtr = m_parent.lock();
		if (shPtr) {
			return shPtr;
		}
		return nullptr;
	}
	void Transform::SetParent(const shared_ptr<GameObject>& Obj) {
		if (GetParent() == Obj) {
			return;
		}
		if (Obj) {
			ClearParent();
			m_parent = Obj;
			auto parWorld = Obj->GetComponent<Transform>()->GetWorldMatrix();
			parWorld.scaleIdentity();
			auto posSpan = GetPosition() - parWorld.transInMatrix();
			auto qtSpan = parWorld.quatInMatrix();
			qtSpan = bsm::inverse(qtSpan);
			Mat4x4 qarQtMat(qtSpan);
			posSpan *= qarQtMat;

			Mat4x4 Mat = GetWorldMatrix() * parWorld;
			Float3 scale, pos;
			bsm::Quat qt;
			Mat.decompose(scale, qt, pos);
			SetScale(scale);
			SetQuaternion(qt);
			SetPosition(posSpan);
			SetToBefore();

		}
		else {
			//nullptrが渡された
			ClearParent();
		}
		m_dirtyFlg = true;
	}

	void Transform::ClearParent() {
		if (auto parPtr = GetParent()) {
			auto pos = GetWorldPosition();
			SetPosition(pos);
			SetToBefore();
		}
		m_parent.reset();
		m_dirtyFlg = true;
	}

	Float3 Transform::GetVelocity() const {
		//前回のターンからの時間
		float ElapsedTime = App::GetElapsedTime();
		Float3 Velocity = m_position - m_beforePosition;
		Velocity /= ElapsedTime;
		return Velocity;
	}

	float Transform::GetMoveSize() const {
		Float3 Move = m_position - m_beforePosition;
		return bsm::length(Move);
	}

	void Transform::SetToBefore() {
		m_beforeScale = m_scale;
		m_beforePivot = m_pivot;
		m_beforeQuaternion = m_quaternion;
		m_beforePosition = m_position;
	}

	Float3 Transform::GetForword() {
		Float3 ret = GetWorldMatrix().rotZInMatrix();
		ret.normalize();
		return ret;
	}

	Float3 Transform::GetUp() {
		Float3 ret = GetWorldMatrix().rotYInMatrix();
		ret.normalize();
		return ret;
	}
	Float3 Transform::GetRight() {
		Float3 ret = GetWorldMatrix().rotXInMatrix();
		ret.normalize();
		return ret;
	}


	//操作
	void Transform::OnUpdate() {
	}


}
//end basedx12
