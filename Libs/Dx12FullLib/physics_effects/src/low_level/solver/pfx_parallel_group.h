/*
Physics Effects Copyright(C) 2012 Sony Computer Entertainment Inc.
All rights reserved.

Physics Effects is open software; you can redistribute it and/or
modify it under the terms of the BSD License.

Physics Effects is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the BSD License for more details.

A copy of the BSD License is distributed with
Physics Effects under the filename: physics_effects_license.txt
*/

#pragma once
#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
// Parallel Group

#define SCE_PFX_MAX_SOLVER_PHASES 64	// �ő�t�F�[�Y��
#define SCE_PFX_MAX_SOLVER_BATCHES 32	// �P�t�F�[�Y�Ɋ܂܂��ő���񏈗��o�b�`
#define SCE_PFX_MAX_SOLVER_PAIRS  64	// �P�o�b�`�Ɋ܂܂��ő�y�A��
#define SCE_PFX_MIN_SOLVER_PAIRS  16	// �P�o�b�`�Ɋ܂܂��ŏ��y�A��

namespace sce {
namespace PhysicsEffects {

struct SCE_PFX_ALIGNED(128) PfxParallelBatch {
	PfxUInt16 pairIndices[SCE_PFX_MAX_SOLVER_PAIRS];
};

struct SCE_PFX_ALIGNED(128) PfxParallelGroup {
	PfxUInt16 numPhases;
	PfxUInt16 numBatches[SCE_PFX_MAX_SOLVER_PHASES]; // �e�t�F�[�Y�̕ێ����������s�\�ȃo�b�`�̐�
	PfxUInt16 numPairs[SCE_PFX_MAX_SOLVER_PHASES*SCE_PFX_MAX_SOLVER_BATCHES]; // �e�o�b�`�̕ێ�����y�A�̐�
SCE_PFX_PADDING(1,126)
};

} //namespace PhysicsEffects
} //namespace sce

