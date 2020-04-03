#pragma once
#include "GameDevice.h"

namespace basedx12 {


	DECLARE_DX12SHADER(VSPTSprite)
	DECLARE_DX12SHADER(PSPTSprite)

	DECLARE_DX12SHADER(VSPNTStatic)
	DECLARE_DX12SHADER(PSPNTStatic)


	struct SceneConstantBuffer
	{
		XMFLOAT4 offset;
		SceneConstantBuffer() :
			offset(0.0f, 0, 0, 0)
		{}
	};

	//2D�R���X�^���g�o�b�t�@
	struct SpriteConstantBuffer
	{
		Mat4x4 World;
		Float4 Emissive;
		Float4 Diffuse;
		SpriteConstantBuffer() :
			World(),
			Emissive(0.0f),
			Diffuse(1.0f)
		{
		};
	};



	class MoveSquare {
		float m_posSpan;
		//�X�P�[��
		Float3 m_scale;
		//��]
		Quat m_qt;
		//�ʒu
		Float3 m_pos;
		//PositionTexture�̎l�p�`���b�V��
		shared_ptr<BaseMesh> m_ptSquareMesh;
		//PositionTexture�p�p�C�v���C���X�e�[�g�i�R���X�^���g�o�b�t�@����j
		ComPtr<ID3D12PipelineState> m_ptConstPipelineState;
		//�e�N�X�`��
		shared_ptr<BaseTexture> m_SkyTexture;
		//�e�N�X�`���i�V�F�[�_���\�[�X�j�̃C���f�b�N�X
		UINT m_srvIndex;
		//�R���X�^���g�o�b�t�@�\���̂̎���
		SpriteConstantBuffer m_constantBufferData;
		//�R���X�^���g�o�b�t�@
		shared_ptr<ConstantBuffer> m_ConstantBuffer;
		//�R���X�^���g�o�b�t�@�̃C���f�b�N�X
		UINT m_constBuffIndex;
	public:
		MoveSquare():
			m_posSpan(1.0f),
			m_scale(1.0f),
			m_qt(),
			m_pos(0.0f)
		{}
		~MoveSquare() {}
		void OnInit();
		void OnUpdate();
		void OnDraw();
	};

	//--------------------------------------------------------------------------------------
	///	Basic�V�F�[�_�[�p�R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
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
		//�ėp�t���O
		XMUINT4 activeFlg;
		//�ȉ��e
		Float4 lightPos;
		Float4 eyePos;
		Mat4x4 lightView;
		Mat4x4 lightProjection;

		Float4 bones[3 * 72];

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
		BasicConstants m_constantBufferData;
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



		float m_posSpan;
		//�G�~�b�V�u�F
		Float4 m_emissive;
		//�f�t���[�Y�F
		Float4 m_diffuse;
		//�����x
		float m_alpha;
		//�X�y�L�����[�ƃp���[�F
		Float4 m_specularColorAndPower;

		void SetConstants();

	public:
		MoveBox():
			m_posSpan(0.02f),
			m_scale(1.0f),
			m_qt(),
			m_pos(0.0f)
		{}
		~MoveBox() {}
		void OnInit();
		void OnUpdate();
		void OnDraw();
	};


	class Scene :public SceneBase {
		MoveSquare m_MoveSquare;
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


