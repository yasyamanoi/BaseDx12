#pragma once
#include "stdafx.h"

namespace basedx12 {

	class ItemSquare : public BaseSquare {
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
		virtual void UpdateConstdata()override;
	public:
		ItemSquare(const MapData& data) :
			BaseSquare(data)
		{}
		virtual ~ItemSquare() {}
		virtual void OnCollisionEnter(BaseSquare* other) override;
		virtual void OnInit() override;
		virtual void OnUpdate()override;
		virtual void OnDraw() override;
	};

}
//end basedx12
