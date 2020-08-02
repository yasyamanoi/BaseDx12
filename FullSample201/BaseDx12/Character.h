#pragma once
#include "stdafx.h"

namespace basedx12 {

	class MoveTriangle :public GameObject {
		float m_posSpan;
	public:
		explicit MoveTriangle(const shared_ptr<Stage>& stage) :
			GameObject(stage),
			m_posSpan(1.0f)
			{}
		virtual ~MoveTriangle() {}
		virtual void OnInit()override;
		virtual void OnUpdate()override;
	};

	class MoveTriangle2 :public GameObject {
		float m_posSpan;
	public:
		explicit MoveTriangle2(const shared_ptr<Stage>& stage) :
			GameObject(stage),
			m_posSpan(2.0f)
			{}
		virtual ~MoveTriangle2() {}
		virtual void OnInit()override;
		virtual void OnUpdate()override;
	};

	class MoveSquare :public GameObject {
		float m_posSpan;
	public:
		explicit MoveSquare(const shared_ptr<Stage>& stage) :
			GameObject(stage),
			m_posSpan(3.0f)
			{}
		virtual ~MoveSquare() {}
		virtual void OnInit()override;
		virtual void OnUpdate()override;
	};

	class MoveBox :public GameObject {
		float m_posSpan;
	public:
		explicit MoveBox(const shared_ptr<Stage>& stage) :
		GameObject(stage),
		m_posSpan(0.02f)
		{}
		virtual ~MoveBox() {}
		virtual void OnInit()override;
		virtual void OnUpdate()override;
	};

}
//end basedx12
