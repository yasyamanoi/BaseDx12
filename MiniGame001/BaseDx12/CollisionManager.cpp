#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

	void CollisionManager::RegisterObject(BaseSquare* pBaseSquare) {
		m_objectVec.push_back(pBaseSquare);
	}

	void CollisionManager::RemoveObject(BaseSquare* pBaseSquare) {
		auto it = m_objectVec.begin();
		while (it != m_objectVec.end()) {
			if ((*it) == pBaseSquare) {
				m_objectVec.erase(it);
				return;
			}
			it++;
		}

	}

	bool CollisionManager::IsPairChk(BaseSquare* left, BaseSquare* right) {
		for (auto& v : m_pairVec) {
			if (v.m_left == left && v.m_right == right) {
				return true;
			}
			if (v.m_left == right && v.m_right == left) {
				return true;
			}
		}
		return false;
	}

	bool CollisionManager::IsTempPairChk(BaseSquare* left, BaseSquare* right) {
		for (auto& v : m_tempPairVec) {
			if (v.m_left == left && v.m_right == right) {
				return true;
			}
			if (v.m_left == right && v.m_right == left) {
				return true;
			}
		}
		return false;
	}


	bool CollisionManager::IsExcludeChk(BaseSquare* left, BaseSquare* right) {
		for (auto v : m_excludeCollisionVec) {
			if (v.m_left == left && v.m_right == right) {
				return true;
			}
			if (v.m_left == right && v.m_right == left) {
				return true;
			}
		}
		return false;
	}

	void CollisionManager::SetExcludeObjects(BaseSquare* left,BaseSquare* right) {
		for (auto v : m_excludeCollisionVec) {
			if (v.m_left == left && v.m_right == right) {
				return;
			}
			if (v.m_left == right && v.m_right == left) {
				return;
			}
		}
		BaseSquarePair pair;
		pair.m_left = left;
		pair.m_right = right;
		m_excludeCollisionVec.push_back(pair);
	}

	void CollisionManager::ResetExcludeObjects(BaseSquare* left, BaseSquare* right) {
		auto it = m_excludeCollisionVec.begin();
		while (it != m_excludeCollisionVec.end()) {
			if ((*it).m_left == left && (*it).m_right == right) {
				m_excludeCollisionVec.erase(it);
				return;
			}
			if ((*it).m_right == left && (*it).m_left == right) {
				m_excludeCollisionVec.erase(it);
				return;
			}
			it++;
		}
	}


	bool CollisionManager::IsOnObject(BaseSquare* src,BaseSquare* dest) {
		float span = 7.0f;
		auto left = src->GetOBB();
		auto right = dest->GetOBB();
		OBB testOBB = left;
		testOBB.m_Center.y -= span;
		if (HitTest::OBB_OBB(testOBB, right)) {
			testOBB = left;
			testOBB.m_Center.y += span;
			if (HitTest::OBB_OBB(testOBB, right)) {
				return false;
			}
			testOBB = left;
			testOBB.m_Center.x -= span;
			if (HitTest::OBB_OBB(testOBB, right)) {
				return false;
			}
			testOBB = left;
			testOBB.m_Center.x += span;
			if (HitTest::OBB_OBB(testOBB, right)) {
				return false;
			}
			return true;
		}
		return false;
	}

	OnSide CollisionManager::GetOnSide(const OBB& left, const OBB& right) {
		float span = 10.0f;
		OBB testOBB = left;
		testOBB.m_Center.y -= span;
		if (HitTest::OBB_OBB(testOBB, right)) {
			return OnSide::up;
		}
		testOBB = left;
		testOBB.m_Center.y += span;
		if (HitTest::OBB_OBB(testOBB, right)) {
			return OnSide::down;
		}
		testOBB = left;
		testOBB.m_Center.x -= span;
		if (HitTest::OBB_OBB(testOBB, right)) {
			return OnSide::left;
		}
		testOBB = left;
		testOBB.m_Center.x += span;
		if (HitTest::OBB_OBB(testOBB, right)) {
			return OnSide::right;
		}
		return OnSide::up;
	}

	void CollisionManager::TestPreCollision() {
		//コリジョンの前処理
		auto it = m_pairVec.begin();
		while (it != m_pairVec.end()) {
			auto left = (*it).m_left->GetOBB();
			auto right = (*it).m_right->GetOBB();
			if (!HitTest::OBB_OBB(left, right)) {
				it = m_pairVec.erase(it);
			}
			else {
				it++;
			}
		}
	}

	CollitionPair CollisionManager::MakePair(BaseSquare* leftPtr,BaseSquare* rightPtr, float hitTime) {
		auto leftBefore = leftPtr->GetBeforeOBB();
		auto rightBefore = rightPtr->GetBeforeOBB();

		CollitionPair pair;
		pair.m_left = leftPtr;
		pair.m_right = rightPtr;
		OBB leftChkObb = leftBefore;
		leftChkObb.m_Center += leftPtr->GetWorldVelocity() * hitTime;

		OBB rightChkObb = rightBefore;
		rightChkObb.m_Center += rightPtr->GetWorldVelocity() * hitTime;

		switch (GetOnSide(leftChkObb, rightChkObb)) {
		case OnSide::up:
			pair.m_normalLeft = Float3(0, 1, 0);
			pair.m_hitMomentCenterLeft = leftChkObb.m_Center;
			pair.m_normalRight = Float3(0, -1, 0);
			pair.m_hitMomentCenterRight = rightChkObb.m_Center;
			break;
		case OnSide::down:
			pair.m_normalLeft = Float3(0, -1, 0);
			pair.m_hitMomentCenterLeft = leftChkObb.m_Center;
			pair.m_normalRight = Float3(0, 1, 0);
			pair.m_hitMomentCenterRight = rightChkObb.m_Center;
			break;
		case OnSide::left:
			pair.m_normalLeft = Float3(1, 0, 0);
			pair.m_hitMomentCenterLeft = leftChkObb.m_Center;
			pair.m_normalRight = Float3(-1, 0, 0);
			pair.m_hitMomentCenterRight = rightChkObb.m_Center;
			break;
		case OnSide::right:
			pair.m_normalLeft = Float3(-1, 0, 0);
			pair.m_hitMomentCenterLeft = leftChkObb.m_Center;
			pair.m_normalRight = Float3(1, 0, 0);
			pair.m_hitMomentCenterRight = rightChkObb.m_Center;
			break;
		}
		//まずはテンポラリに追加
		m_tempPairVec.push_back(pair);
		return pair;
	}


	void CollisionManager::TestCollision() {
		float elapsedTime = App::GetElapsedTime();
		//衝突判定
		m_tempPairVec.clear();
		for (int i = 0; i < m_objectVec.size(); i++) {
			auto playerPtr = dynamic_cast<Player*>(m_objectVec[i]);
			//プレイヤー以外は判定なし
			if (!playerPtr) {
				continue;
			}
			for (int j = 0; j < m_objectVec.size(); j++) {
				if (i != j) {
					if (IsExcludeChk(m_objectVec[i], m_objectVec[j])) {
						continue;
					}
					if (IsPairChk(m_objectVec[i], m_objectVec[j])) {
						continue;
					}
					if (IsTempPairChk(m_objectVec[i], m_objectVec[j])) {
						continue;
					}
					auto leftBefore = m_objectVec[i]->GetBeforeOBB();
					auto rightBefore = m_objectVec[j]->GetBeforeOBB();
					auto left = m_objectVec[i]->GetOBB();
					auto right = m_objectVec[j]->GetOBB();
					Float3 spanVelocity = m_objectVec[i]->GetWorldVelocity() - m_objectVec[j]->GetWorldVelocity();
					float hitTime = 0;
					if (HitTest::CollisionTestObbObbWithEpsilon(leftBefore, spanVelocity, rightBefore,0.01f, 0, elapsedTime, hitTime)) {
						auto pair = MakePair(m_objectVec[i], m_objectVec[j], hitTime);
						Float3 velo = m_objectVec[i]->GetWorldVelocity();
						velo.slide(pair.m_normalLeft);
						m_objectVec[i]->SetWorldVelocity(velo);
					}
				}
			}
		}
		//ペア配列に追加
		for (auto& v : m_tempPairVec) {
			m_pairVec.push_back(v);
		}
	}
	void CollisionManager::EscapeCollision() {
		//エスケープ処理
		//left側のみエスケープ
		for (auto& v : m_pairVec) {
			if (!v.m_left->IsFixed()) {
				//拘束の解消
				Float3 srcCenter = v.m_left->GetWorldPosition();
				Float3 destCenter = v.m_right->GetWorldPosition();
				Float3 destMoveVec = destCenter - v.m_hitMomentCenterRight;
				Float3 srcLocalVec = srcCenter - v.m_hitMomentCenterLeft - destMoveVec;
				float srcV = bsm::dot(srcLocalVec, v.m_normalLeft);
				if (srcV < 0.0f) {
					//まだ衝突していたら
					float escapeLen = abs(srcV);
					if (!v.m_right->IsFixed()) {
						escapeLen *= 0.5f;
					}
					//Srcのエスケープ
					srcCenter += v.m_normalLeft * escapeLen;
					v.m_left->SetWorldPosition(srcCenter);
				}
				v.m_left->SetDirtyflag(true);
			}
		}
	}

	void CollisionManager::MessageCollision() {
		//各オブジェクトに衝突があったことをメッセージする
		for (auto& v : m_tempPairVec) {
			v.m_left->OnCollisionEnter(v.m_right);
			v.m_right->OnCollisionEnter(v.m_left);
		}
	}

	void CollisionManager::TestMainCollision() {
		TestCollision();
		EscapeCollision();
		MessageCollision();
	}

}
//end basedx12
