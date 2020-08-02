#pragma once
#include "stdafx.h"

namespace basedx12 {

	class Player : public BaseSquare {

		const float m_speedParam = 250.0f;
		const float m_speedJumpParam = 170.0f;

		const float m_jumpVelocity = 550.0f;
		//移動オブジェクトに乗ってる場合のパラメータ
		Float3 m_moveRelativePos = Float3(0.0f);
		BaseSquare* m_slideObject;
		//ステートマシーン
		unique_ptr< StateMachine<Player> >  m_stateMachine;
		//入力ハンドラー
		InputHandler<Player> m_inputHandler;
		//プレイヤーが乗っているオブジェクト
		BaseSquare* m_onObject;
	public:
		Player():
			BaseSquare(),
			m_onObject(nullptr)
		{}
		virtual ~Player() {}
		virtual void OnInit() override;
		virtual void OnUpdate()override;
		virtual void OnCollisionEnter(BaseSquare* other) override;

		//Aボタン
		void OnPushA();

		void ApplyGravity();
		void ApplyPotision();
		void ApplyControlers();
		void ApplySideSlide();

		void ChkAndChangeOnObject();
		void SetExcludeObject();
		void ResetExcludeObject();
		void ChangeJumpState();

	};

	class PlayerJumpState : public ObjState<Player>
	{
		PlayerJumpState() {}
	public:
		static shared_ptr<PlayerJumpState> Instance();
		virtual void Enter(Player* owner)override;
		virtual void Execute(Player* owner)override;
		virtual void Exit(Player* owner)override;
	};

	class PlayerOnObjState : public ObjState<Player>
	{
		PlayerOnObjState() {}
	public:
		static shared_ptr<PlayerOnObjState> Instance();
		virtual void Enter(Player* owner)override;
		virtual void Execute(Player* owner)override;
		virtual void Exit(Player* owner)override;
	};

	class PlayerOutSideState : public ObjState<Player>
	{
		PlayerOutSideState() {}
	public:
		static shared_ptr<PlayerOutSideState> Instance();
		virtual void Enter(Player* owner)override;
		virtual void Execute(Player* owner)override;
		virtual void Exit(Player* owner)override;
	};


}
//end basedx12
