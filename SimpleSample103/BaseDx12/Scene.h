#pragma once
#include "GameDevice.h"

namespace basedx12 {




	DECLARE_DX12SHADER(VSPNT)
	DECLARE_DX12SHADER(PSPNT)

	struct SimpleConstants {
		Mat4x4 worldViewProj;
	};

	class FixedBox {
		shared_ptr<Camera> m_camera;
		shared_ptr<LightSet> m_lightSet;
		shared_ptr<BaseMesh> m_mesh;
		ComPtr<ID3D12PipelineState> m_pipelineState;
		//�R���X�^���g�o�b�t�@
		shared_ptr<ConstantBuffer> m_constantBuffer;
		//�R���X�^���g�o�b�t�@�̃C���f�b�N�X
		UINT m_constBuffIndex;
		//�R���X�^���g�o�b�t�@�̎���
		SimpleConstants m_simpleConstantsData;
		//�e�N�X�`��
		shared_ptr<BaseTexture> m_SkyTexture;
		//�e�N�X�`���i�V�F�[�_���\�[�X�j�̃C���f�b�N�X
		UINT m_srvIndex;
		//�X�P�[��
		Float3 m_scale;
		//��]
		Quat m_qt;
		//�ʒu
		Float3 m_pos;

		void SetSimpleConstants();

	public:
		FixedBox() :
			m_scale(10.0f,1.0f,10.0f),
			m_qt(),
			m_pos(0.0f, -0.5f, 0.0f)
		{}
		~FixedBox() {}
		void OnInit();
		void OnUpdate() {}
		void OnDraw();
	};



	class MoveBox{
		shared_ptr<Camera> m_camera;
		shared_ptr<LightSet> m_lightSet;
		shared_ptr<BaseMesh> m_mesh;
		ComPtr<ID3D12PipelineState> m_pipelineState;
		//�R���X�^���g�o�b�t�@
		shared_ptr<ConstantBuffer> m_constantBuffer;
		//�R���X�^���g�o�b�t�@�̃C���f�b�N�X
		UINT m_constBuffIndex;
		//�R���X�^���g�o�b�t�@�̎���
		SimpleConstants m_simpleConstantsData;
		//�e�N�X�`��
		shared_ptr<BaseTexture> m_wallTexture;
		//�e�N�X�`���i�V�F�[�_���\�[�X�j�̃C���f�b�N�X
		UINT m_srvIndex;
		//�X�P�[��
		Float3 m_scale;
		//��]
		Quat m_qt;
		//�ʒu
		Float3 m_pos;
		float m_posSpan;
		void SetSimpleConstants();
	public:
		MoveBox():
			m_posSpan(0.02f),
			m_scale(1.0f),
			m_qt(),
			m_pos(0.0f,0.5f,0.0f)
		{}
		~MoveBox() {}
		void OnInit();
		void OnUpdate();
		void OnDraw();
	};


	class Scene :public SceneBase {
		FixedBox m_FixedBox;
		MoveBox m_MoveBox;
	public:
		Scene() :SceneBase() {}
		virtual ~Scene() {}
		virtual void OnInit()override;
		virtual void OnInitAssets()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override;
		virtual void OnDestroy()override;
	};

}
//end basedx12


