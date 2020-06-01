#pragma once
#include "stdafx.h"

namespace basedx12 {

	class MoveSquare : public BaseSquare {
		const Float3 m_moveVelocity = Float3(-120.0f, 0, 0);
		bool m_sideJumped = false;
	public:
		MoveSquare(const MapData& data) :
			BaseSquare(data)
		{}
		virtual ~MoveSquare() {}
		virtual void OnCollisionEnter(BaseSquare* other) override;
		virtual void OnInit() override;
		virtual void OnUpdate()override;

		bool IsSideJumped() const{
			return m_sideJumped;
		}
		void ClearSideJumped() {
			m_sideJumped = false;
		}
	};


}
//end basedx12
