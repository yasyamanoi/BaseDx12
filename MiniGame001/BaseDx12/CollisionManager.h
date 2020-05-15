#pragma once
#include "stdafx.h"

namespace basedx12 {

	class BaseSquare;


	struct CollitionPair {
		BaseSquare* m_left;
		Float3 m_normalLeft;
		Float3 m_hitMomentCenterLeft;
		BaseSquare* m_right;
		Float3 m_normalRight;
		Float3 m_hitMomentCenterRight;
	};

	struct BaseSquarePair {
		BaseSquare* m_left;
		BaseSquare* m_right;
	};

	enum class OnSide {
		noside,
		up,
		down,
		left,
		right
	};

	class CollisionManager {
		const float m_onObjectSpan = 50.0f;
		vector<BaseSquare*> m_objectVec;
		vector<CollitionPair> m_pairVec;
		vector<CollitionPair> m_tempPairVec;
		vector<BaseSquarePair> m_excludeCollisionVec;
		bool IsPairChk(BaseSquare* left, BaseSquare* right);
		bool IsTempPairChk(BaseSquare* left, BaseSquare* right);
		bool IsExcludeChk(BaseSquare* left, BaseSquare* right);
		CollitionPair MakePair(BaseSquare* leftPtr,BaseSquare* rightPtr,float hitTime);
		void TestCollision();
		void EscapeCollision();
		void MessageCollision();
	public:
		CollisionManager():
			m_objectVec(1024), 
			m_pairVec(1024),
			m_tempPairVec(1024),
			m_excludeCollisionVec(1024)
		{
			m_objectVec.clear();
			m_pairVec.clear();
			m_tempPairVec.clear();
			m_excludeCollisionVec.clear();
		}
		~CollisionManager() {}
		void RegisterObject(BaseSquare* pBaseSquare);
		void RemoveObject(BaseSquare* pBaseSquare);

		void TestPreCollision();
		void TestMainCollision();
		void SetExcludeObjects(BaseSquare* left,BaseSquare* right);
		void ResetExcludeObjects(BaseSquare* left, BaseSquare* right);
		bool IsOnObject(BaseSquare* src,BaseSquare* dest);
		OnSide GetOnSide(const OBB& left, const OBB& right);

		vector<CollitionPair>& GetPairVec()  {
			return m_pairVec;
		}
	};

}
//end basedx12
