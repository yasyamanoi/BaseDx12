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
		auto left = src->GetOBB();
		auto right = dest->GetOBB();
		Float3 ret;
		HitTest::ClosestPtPointOBB(left.m_Center, right, ret);
		float len = bsm::length(ret - left.m_Center);
		if (len > m_onObjectSpan) {
			return false;
		}
		Float3 normal = ret - left.m_Center;
		normal.normalize();
		auto f = bsm::angleBetweenNormals(normal, Float3(0, -1, 0));
		if (abs(f) < XM_PIDIV4) {
			return true;
		}
		return false;
	}

	void CollisionManager::CollisionPre() {
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

	void CollisionManager::CollisionTest() {
		float elapsedTime = App::GetElapsedTime();
		//衝突判定
		m_tempPairVec.clear();
		for (int i = 0; i < m_objectVec.size(); i++) {
			for (int j = 0; j < m_objectVec.size(); j++) {
				if (i != j) {
					auto* leftPtr = dynamic_cast<WallSquare*>(m_objectVec[i]);
					auto* rightPtr = dynamic_cast<WallSquare*>(m_objectVec[j]);
					if (leftPtr && rightPtr) {
						continue;
					}
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

					auto& leftdata = m_objectVec[i]->GetDrawData();
					auto& rightdata = m_objectVec[j]->GetDrawData();

					Float3 spanVelocity = leftdata.m_velocity - rightdata.m_velocity;
					float hitTime = 0;
					if (HitTest::CollisionTestObbObb(leftBefore, spanVelocity, rightBefore, 0, elapsedTime, hitTime)) {

						CollitionPair pair;
						pair.m_left = m_objectVec[i];
						pair.m_right = m_objectVec[j];
						OBB leftChkObb = leftBefore;
						leftChkObb.m_Center += leftdata.m_velocity * hitTime;

						OBB rightChkObb = rightBefore;
						rightChkObb.m_Center += rightdata.m_velocity * hitTime;

						hitTime = elapsedTime - hitTime;

						Float3 ret;
						HitTest::ClosestPtPointOBB(leftChkObb.m_Center, rightChkObb, ret);
						Float3 normal = leftChkObb.m_Center - ret;
						normal.z = 0.0f;
						normal.normalize();
						pair.m_normalLeft = normal;
						pair.m_hitMomentCenterLeft = leftChkObb.m_Center;

						HitTest::ClosestPtPointOBB(rightChkObb.m_Center, leftChkObb, ret);
						normal = rightChkObb.m_Center - ret;
						normal.z = 0.0f;
						normal.normalize();
						pair.m_normalRight = normal;
						pair.m_hitMomentCenterRight = rightChkObb.m_Center;

						//まずはテンポラリに追加
						m_tempPairVec.push_back(pair);
					}
				}
			}
		}
		//ペア配列に追加
		for (auto& v : m_tempPairVec) {
			m_pairVec.push_back(v);
		}
		//エスケープ処理
		for (auto& v : m_pairVec) {
			if (!v.m_left->IsFixed()) {
				//拘束の解消
				Float3 srcCenter = v.m_left->GetDrawData().m_pos;
				Float3 destCenter = v.m_right->GetDrawData().m_pos;
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
					srcCenter.floor(3);
					v.m_left->GetDrawData().m_pos = srcCenter;
				}
				v.m_left->GetDrawData().m_dirtyflag = true;
				//拘束の解消後の速度は0
				v.m_left->GetDrawData().m_velocity = Float3(0);
			}
			if (!v.m_right->IsFixed()) {
				//拘束の解消
				Float3 srcCenter = v.m_right->GetDrawData().m_pos;
				Float3 destCenter = v.m_left->GetDrawData().m_pos;
				Float3 destMoveVec = destCenter - v.m_hitMomentCenterLeft;
				Float3 srcLocalVec = srcCenter - v.m_hitMomentCenterRight -destMoveVec;
				float srcV = bsm::dot(srcLocalVec, v.m_normalRight);
				if (srcV < 0.0f) {
					//まだ衝突していたら
					float escapeLen = abs(srcV);
					if (!v.m_left->IsFixed()) {
						escapeLen *= 0.5f;
					}
					//Srcのエスケープ
					srcCenter += v.m_normalRight * escapeLen;
					srcCenter.floor(3);
					v.m_right->GetDrawData().m_pos = srcCenter;
				}
				v.m_right->GetDrawData().m_dirtyflag = true;
				//拘束の解消後の速度は0
				v.m_right->GetDrawData().m_velocity = Float3(0);
			}
		}

		//各オブジェクトに衝突があったことをメッセージする
		for (auto& v : m_tempPairVec) {
			v.m_left->OnCollisionEnter(v.m_right);
			v.m_right->OnCollisionEnter(v.m_left);
		}

	}

}
//end basedx12
