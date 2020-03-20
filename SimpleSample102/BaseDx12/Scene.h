#pragma once
#include "GameDevice.h"

namespace basedx12 {

	DECLARE_DX12SHADER(VSPCSprite)
	DECLARE_DX12SHADER(PSPCSprite)

	DECLARE_DX12SHADER(VSPCConstSprite)
	DECLARE_DX12SHADER(PSPCConstSprite)

	DECLARE_DX12SHADER(VSPTSprite)
	DECLARE_DX12SHADER(PSPTSprite)

	struct SceneConstantBuffer
	{
		XMFLOAT4 offset;
		SceneConstantBuffer() :
			offset(0.0f, 0, 0, 0)
		{}
	};

	class FixedTriangle {
		//PositionColor�̎O�p�`���b�V��
		shared_ptr<BaseMesh> m_pcTriangleMesh;
		//PositionColor�p�p�C�v���C���X�e�[�g�i�R���X�^���g�o�b�t�@�Ȃ��j
		ComPtr<ID3D12PipelineState> m_pcPipelineState;
	public:
		FixedTriangle() {}
		~FixedTriangle() {}
		void OnInit();
		void OnUpdate() {}
		void OnRender();
	};

	class MoveTriangle {
		//PositionColor�̎O�p�`���b�V��
		shared_ptr<BaseMesh> m_pcTriangleMesh;
		//PositionColor�p�p�C�v���C���X�e�[�g�i�R���X�^���g�o�b�t�@����j
		ComPtr<ID3D12PipelineState> m_pcConstPipelineState;
		//�R���X�^���g�o�b�t�@�\���̂̎���
		SceneConstantBuffer m_constantBufferData;
		//�R���X�^���g�o�b�t�@
		shared_ptr<ConstantBuffer> m_ConstantBuffer;
		//�R���X�^���g�o�b�t�@�̃C���f�b�N�X
		UINT m_constBuffIndex;
	public:
		MoveTriangle(){}
		~MoveTriangle() {}
		void OnInit();
		void OnUpdate();
		void OnRender();
	};

	class MoveSquare {
		//PositionTexture�̎l�p�`���b�V��
		shared_ptr<BaseMesh> m_ptSquareMesh;
		//PositionTexture�p�p�C�v���C���X�e�[�g�i�R���X�^���g�o�b�t�@����j
		ComPtr<ID3D12PipelineState> m_ptConstPipelineState;
		//�e�N�X�`��
		shared_ptr<BaseTexture> m_SkyTexture;
		//�e�N�X�`���i�V�F�[�_���\�[�X�j�̃C���f�b�N�X
		UINT m_srvIndex;
		//�R���X�^���g�o�b�t�@�\���̂̎���
		SceneConstantBuffer m_constantBufferData;
		//�R���X�^���g�o�b�t�@
		shared_ptr<ConstantBuffer> m_ConstantBuffer;
		//�R���X�^���g�o�b�t�@�̃C���f�b�N�X
		UINT m_constBuffIndex;
	public:
		MoveSquare() {}
		~MoveSquare() {}
		void OnInit();
		void OnUpdate();
		void OnRender();
	};

	class Scene :public SceneBase {
		FixedTriangle m_FixedTriangle;
		MoveTriangle m_MoveTriangle;
		MoveSquare m_MoveSquare;
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


