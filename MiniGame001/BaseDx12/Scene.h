#pragma once
#include "stdafx.h"

namespace basedx12 {

	struct ObjRect;
	struct ObjData;
	class Scene :public SceneBase {
		const UINT m_halfWidth = 48 / 2;
		const UINT m_cellSize = 32;
		const float m_sceneGravity = -980.0f;
		float m_lowSpan;
		//�T���v���[�̃C���f�b�N�X(���L����)
		UINT m_linearClampSamplerIndex;
		UINT m_linearWrapSamplerIndex;
		//PositionTexture�p�p�C�v���C���X�e�[�g�i�R���X�^���g�o�b�t�@���苤�L����j
		ComPtr<ID3D12PipelineState> m_ptConstPipelineState;
		//�z�u�����I�u�W�F�N�g�̐e�N���X�|�C���^�̔z��
		vector<shared_ptr<BaseSquare>> m_baseSquareVec;
		//�Փ˔���}�l�[�W��
		CollisionManager m_collisionManager;
		//�X�e�[�W�f�[�^�ǂݍ���
		//1�̃I�u�W�F�N�g�f�[�^�̃N���A
		void ClearTgtCelldata(const ObjRect& rect);
		//1�̃I�u�W�F�N�g�f�[�^�̓ǂݍ���
		ObjRect ReadSimgleObject();
		//������W����Z���^�[���W�ɕϊ�
		void ConvertLeftTopDataToCenter(vector<ObjRect>& rectVec, vector<ObjData>& objVec);
	public:
		Scene() :SceneBase(), m_lowSpan(0.0f) {
		}
		virtual ~Scene() {}
		UINT GetHalfWidth() const {
			return m_halfWidth;
		}
		UINT GetCellSize() const {
			return m_cellSize;
		}

		float GetLowSpan() const {
			return m_lowSpan;
		}
		void RegisterCollsionObject(BaseSquare* pBaseSquare);

		CollisionManager* GetCollisionManager() {
			return &m_collisionManager;
		}
		float GetSceneGravity() const {
			return m_sceneGravity;
		}
		void SetGPULinearClampSampler();
		void SetGPULinearWrapSampler();
		void SetGPUPipelineState();
		virtual void OnInit()override;
		virtual void OnInitAssets()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override;
		virtual void OnDestroy()override;
	};

}
//end basedx12


