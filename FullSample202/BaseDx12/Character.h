#pragma once
#include "stdafx.h"

namespace basedx12 {

	//--------------------------------------------------------------------------------------
	///	�����v�Z����Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	class FixedPsBox : public GameObject {
		Float3 m_Scale;
		Quat m_Qt;
		Float3 m_Position;
	public:
		//�\�z�Ɣj��
		FixedPsBox(const shared_ptr<Stage>& StagePtr,
			const Float3& Scale,
			const Quat& Qt,
			const Float3& Position
		);
		virtual ~FixedPsBox();
		//������
		virtual void OnInit() override;
		virtual void OnUpdate()override {}
	};

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȃ{�b�N�X
	//--------------------------------------------------------------------------------------
	class ActivePsBox : public GameObject {
		Float3 m_Scale;
		Quat m_Qt;
		Float3 m_Position;
	public:
		//�\�z�Ɣj��
		ActivePsBox(const shared_ptr<Stage>& StagePtr,
			const Float3& Scale,
			const Quat& Qt,
			const Float3& Position
		);
		virtual ~ActivePsBox();
		//������
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
