#pragma once
#include "GameDevice.h"

namespace basedx12 {

	DECLARE_DX12SHADER(VSPNT)
	DECLARE_DX12SHADER(PSPNT)

	DECLARE_DX12SHADER(VSPNTShadow)
	DECLARE_DX12SHADER(PSPNTShadow)


	DECLARE_DX12SHADER(VSShadowmap)

	struct ShadowmapConstants {
		Mat4x4 world;
		Mat4x4 view;
		Mat4x4 proj;
	};

	struct SimpleConstants {
		Mat4x4 worldViewProj;
	};

	struct ShadowSceneConstants {
		/// ���[���h�s��
		Mat4x4 World;
		/// �r���[�s��
		Mat4x4 View;
		/// �ˉe�s��
		Mat4x4 Projection;
		/// ���C�g�ʒu
		Float4 LightPos;
		/// Eye�̈ʒu
		Float4 EyePos;
		/// ���C�g�r���[�s��
		Mat4x4 LightView;
		/// ���C�g�ˉe�s��
		Mat4x4 LightProjection;
	};




	class FixedBox {
		shared_ptr<BaseMesh> m_mesh;
		//�R���X�^���g�o�b�t�@
		shared_ptr<ConstantBuffer> m_constantBuffer;
		//�R���X�^���g�o�b�t�@�̃C���f�b�N�X
		UINT m_constBuffIndex;
		//�R���X�^���g�o�b�t�@�̎���
		ShadowSceneConstants m_shadowSceneConstantsData;
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
		void SetShadowSceneConstants();
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
		//���b�V��
		shared_ptr<BaseMesh> m_mesh;
		//�V���h�E�}�b�v�R���X�^���g�o�b�t�@
		shared_ptr<ConstantBuffer> m_shadowmapConstantBuffer;
		///�V���h�E�}�b�v�R���X�^���g�o�b�t�@�̃C���f�b�N�X
		UINT m_shadowmapConstBuffIndex;
		//�V���h�E�}�b�v�R���X�^���g�o�b�t�@�̎���
		ShadowmapConstants m_shadowmapConstantsData;
		//�V�[���p�R���X�^���g�o�b�t�@
		shared_ptr<ConstantBuffer> m_constantBuffer;
		//�V�[���p�R���X�^���g�o�b�t�@�̃C���f�b�N�X
		UINT m_constBuffIndex;
		//�V�[���p�R���X�^���g�o�b�t�@�̎���
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
		void SetShadowmapConstants();
	public:
		MoveBox():
			m_posSpan(0.02f),
			m_scale(1.0f),
			m_qt(),
			m_pos(0.0f,1.0f,0.0f)
		{}
		~MoveBox() {}
		void OnInit();
		void OnUpdate();
		void OnDrawShadowmap();
		void OnDraw();
	};


	class Scene :public SceneBase {
		FixedBox m_FixedBox;
		MoveBox m_MoveBox;
		//�V���h�E�}�b�v�p�C�v���C��
		ComPtr<ID3D12PipelineState> m_shadowmapPipelineState;
		//�V�[���`��p�p�C�v���C��(�e����)
		ComPtr<ID3D12PipelineState> m_scenePipelineState;
		//�V�[���`��p�p�C�v���C���i�e����j
		ComPtr<ID3D12PipelineState> m_sceneShadowPipelineState;
		//�J����
		shared_ptr<Camera> m_camera;
		//���C�g
		Float3 m_lightPos;
	public:
		Scene() :SceneBase(), m_lightPos(0.0f, 10.0f, 0.0) {}
		virtual ~Scene() {}
		ComPtr<ID3D12PipelineState> GetShadowmapPipelineState() const{
			return m_shadowmapPipelineState;
		}
		ComPtr<ID3D12PipelineState> GetScenePipelineState() const{
			return m_scenePipelineState;
		}
		ComPtr<ID3D12PipelineState> GetSceneShadowPipelineState() const {
			return m_sceneShadowPipelineState;
		}
		shared_ptr<Camera> GetCamera() const{
			return m_camera;
		}
		const Float3& GetLightPos() const {
			return m_lightPos;
		}
		void SetLightPos(const Float3& pos) {
			m_lightPos = pos;
		}
		virtual void OnInit()override;
		virtual void OnInitAssets()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override {}
		virtual void OnDrawPath(UINT index)override;
		virtual void OnDestroy()override;

	};

}
//end basedx12


