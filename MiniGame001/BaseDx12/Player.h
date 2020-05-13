#pragma once
#include "stdafx.h"

namespace basedx12 {

	class Player : public BaseSquare {

		const float m_speedParam = 250.0f;
		const float m_jumpVelocity = 550.0f;
		const float m_onObjectChkParam = 20.0f;
		const float m_widthMargin = 16.0f;
		const float m_moveSquareHalfWidth = 80.0f;
		//ステートマシーン
		unique_ptr< StateMachine<Player> >  m_stateMachine;
		//PositionTextureの四角形メッシュ
		shared_ptr<BaseMesh> m_ptSquareMesh;
		//テクスチャ
		shared_ptr<BaseTexture> m_texture;
		//テクスチャ（シェーダリソース）のインデックス
		UINT m_srvIndex;
		//コンスタントバッファ
		shared_ptr<ConstantBuffer> m_constantBuffer;
		//コンスタントバッファ構造体の実体
		SpriteConstantBuffer m_spriteConstData;
		//コンスタントバッファのインデックス
		UINT m_constBuffIndex;
		//コンスタントバッファ更新用の仮想関数
		virtual void UpdateConstdata()override;
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
		virtual void OnDraw() override;
		virtual void OnCollisionEnter(BaseSquare* other) override;


		//Aボタン
		void OnPushA();

		void ApplyGravity();
		void ApplyPotision();
		void ApplyControlers();

		void ChkAndChangeOnObject();
		void SetExcludeObject();
		void ResetExcludeObject();


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



}
//end basedx12
