#pragma once
#include "GameDevice.h"

namespace basedx12 {

	DECLARE_DX12SHADER(BcVSPNTStaticPLShadow)
	DECLARE_DX12SHADER(BcPSPNTPLShadow)

	DECLARE_DX12SHADER(VSShadowmap)

	struct ShadowmapConstants {
		Mat4x4 world;
		Mat4x4 view;
		Mat4x4 proj;
	};

	struct BasicConstants
	{
		Float4 diffuseColor;
		Float4 emissiveColor;
		Float4 specularColorAndPower;

		Float4 lightDirection[3];
		Float4 lightDiffuseColor[3];
		Float4 lightSpecularColor[3];

		Float4 eyePosition;

		Float4 fogColor;
		Float4 fogVector;

		Mat4x4 world;
		Float4 worldInverseTranspose[3];
		Mat4x4 worldViewProj;
		//�ȉ��e
		Float4 lightPos;
		Float4 eyePos;
		Mat4x4 lightView;
		Mat4x4 lightProjection;
		Float4 bones[3 * 72];
	};


	class FixedBox {
		shared_ptr<BaseMesh> m_mesh;
		//�R���X�^���g�o�b�t�@
		shared_ptr<ConstantBuffer> m_bcConstantBuffer;
		//�R���X�^���g�o�b�t�@�̃C���f�b�N�X
		UINT m_bcConstBuffIndex;
		//�R���X�^���g�o�b�t�@�̎���
		BasicConstants m_bcShadowSceneConstantsData;
		//�e�N�X�`��
		shared_ptr<BaseTexture> m_skyTexture;
		//�e�N�X�`���i�V�F�[�_���\�[�X�j�̃C���f�b�N�X
		UINT m_srvIndex;
		//�X�P�[��
		Float3 m_scale;
		//��]
		Quat m_qt;
		//�ʒu
		Float3 m_pos;
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
		shared_ptr<ConstantBuffer> m_bcConstantBuffer;
		//�R���X�^���g�o�b�t�@�̃C���f�b�N�X
		UINT m_bcConstBuffIndex;
		//�R���X�^���g�o�b�t�@�̎���
		BasicConstants m_bcShadowSceneConstantsData;
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
		void SetShadowmapConstants();
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
		void OnDrawShadowmap();
		void OnDraw();
	};


	class Scene :public SceneBase {
		FixedBox m_FixedBox;
		MoveBox m_MoveBox;
		//�V���h�E�}�b�v�p�C�v���C��
		ComPtr<ID3D12PipelineState> m_shadowmapPipelineState;
		//Basic�p�C�v���C���i�e����j
		ComPtr<ID3D12PipelineState> m_bcSceneShadowPipelineState;
		//�J����
		shared_ptr<Camera> m_camera;
		//���C�g
		Float3 m_lightPos[3];
		//GPU�̃X���b�g�}�b�v
		map<wstring, UINT> m_gpuSlotMap;
		//�t�H�O���g�����ǂ���
		bool m_fogEnabled;
	public:
		Scene() :SceneBase(), m_fogEnabled(true){}
		virtual ~Scene() {}
		ComPtr<ID3D12PipelineState> GetShadowmapPipelineState() const{
			return m_shadowmapPipelineState;
		}
		ComPtr<ID3D12PipelineState> GetBcPipelineState() const {
			return m_bcSceneShadowPipelineState;
		}
		shared_ptr<Camera> GetCamera() const{
			return m_camera;
		}
		const Float3& GetLightPos(UINT num) const {
			return m_lightPos[num];
		}
		void SetLightPos(UINT num,const Float3& pos) {
			m_lightPos[num] = pos;
		}
		void SetBcConstants(
			const Float3& scale,
			const Quat& qt,
			const Float3& pos,
			BasicConstants& ret
		);
		virtual void OnInit()override;
		virtual void OnInitAssets()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override {}
		virtual void OnDrawPath(UINT index)override;
		virtual void OnDestroy()override;

	};

}
//end basedx12


