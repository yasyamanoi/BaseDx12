#pragma once
#include "stdafx.h"

namespace basedx12 {

	class Player : public BaseSquare {

		const float m_speedParam = 250.0f;
		const float m_jumpVelocity = 550.0f;
		const float m_onObjectChkParam = 20.0f;
		const float m_widthMargin = 16.0f;
		const float m_moveSquareHalfWidth = 80.0f;
		//�X�e�[�g�}�V�[��
		unique_ptr< StateMachine<Player> >  m_stateMachine;
		//PositionTexture�̎l�p�`���b�V��
		shared_ptr<BaseMesh> m_ptSquareMesh;
		//�e�N�X�`��
		shared_ptr<BaseTexture> m_texture;
		//�e�N�X�`���i�V�F�[�_���\�[�X�j�̃C���f�b�N�X
		UINT m_srvIndex;
		//�R���X�^���g�o�b�t�@
		shared_ptr<ConstantBuffer> m_constantBuffer;
		//�R���X�^���g�o�b�t�@�\���̂̎���
		SpriteConstantBuffer m_spriteConstData;
		//�R���X�^���g�o�b�t�@�̃C���f�b�N�X
		UINT m_constBuffIndex;
		//�R���X�^���g�o�b�t�@�X�V�p�̉��z�֐�
		virtual void UpdateConstdata()override;
		//���̓n���h���[
		InputHandler<Player> m_inputHandler;
		//�v���C���[������Ă���I�u�W�F�N�g
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


		//A�{�^��
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
