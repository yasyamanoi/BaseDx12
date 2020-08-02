#pragma once
#include "stdafx.h"

namespace basedx12 {

	//--------------------------------------------------------------------------------------
	///	物理計算する固定のボックス
	//--------------------------------------------------------------------------------------
	class FixedPsBox : public GameObject {
		Float3 m_Scale;
		Quat m_Qt;
		Float3 m_Position;
	public:
		//構築と破棄
		FixedPsBox(const shared_ptr<Stage>& StagePtr,
			const Float3& Scale,
			const Quat& Qt,
			const Float3& Position
		);
		virtual ~FixedPsBox();
		//初期化
		virtual void OnInit() override;
		virtual void OnUpdate()override {}
	};

	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブなボックス
	//--------------------------------------------------------------------------------------
	class ActivePsBox : public GameObject {
		Float3 m_Scale;
		Quat m_Qt;
		Float3 m_Position;
	public:
		//構築と破棄
		ActivePsBox(const shared_ptr<Stage>& StagePtr,
			const Float3& Scale,
			const Quat& Qt,
			const Float3& Position
		);
		virtual ~ActivePsBox();
		//初期化
		virtual void OnInit() override;
		virtual void OnUpdate()override {}
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
