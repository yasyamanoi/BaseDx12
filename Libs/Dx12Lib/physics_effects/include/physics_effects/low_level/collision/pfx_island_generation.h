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

#include "../../base_level/solver/pfx_constraint_pair.h"
#include "../task/pfx_task_manager.h"

namespace sce {
namespace PhysicsEffects {

///////////////////////////////////////////////////////////////////////////////
// Island Generation

struct PfxIsland;
struct PfxIslandUnit;

struct PfxGenerateIslandParam {
	void *islandBuff;
	PfxUInt32 islandBytes;
	PfxConstraintPair *pairs;
	PfxUInt32 numPairs;
	PfxUInt32 numObjects;
};

struct PfxGenerateIslandResult {
	PfxIsland *island;
};

//J	�A�C�����h�����g���I���܂�islandBuff��j�����Ȃ��ł�������
//E Keep islandBuff while the island is used
PfxUInt32 pfxGetIslandBytesOfGenerateIsland(PfxUInt32 numObjects);

PfxInt32 pfxGenerateIsland(PfxGenerateIslandParam &param,PfxGenerateIslandResult &result);

//E Access to island information
//J	�A�C�����h���ւ̃A�N�Z�X

//E Get the number of islands
//J �A�C�����h�����擾����
PfxUInt32 pfxGetNumIslands(const PfxIsland *islands);

//E Get a island unit in a island
//J �w�肵���A�C�����h�Ɋ܂܂�郆�j�b�g�i���́j�ɃA�N�Z�X
PfxIslandUnit *pfxGetFirstUnitInIsland(const PfxIsland *islands,PfxUInt32 islandId);
PfxIslandUnit *pfxGetNextUnitInIsland(const PfxIslandUnit *islandUnit);
PfxUInt32 pfxGetUnitId(const PfxIslandUnit *islandUnit);

//E Get an index of an island which includes a specific unit
//J �w�肵�����j�b�g�i���́j�̃A�C�����h�C���f�b�N�X���擾����
PfxUInt32 pfxGetIslandId(const PfxIsland *islands,PfxUInt32 unitId);

//E Add pairs and construct islands
//J �A�C�����h�Ƀy�A��ǉ�����
PfxInt32 pfxAppendPairs(PfxIsland *island,PfxConstraintPair *pairs,PfxUInt32 numPairs);

//E Reset islands
//J	�A�C�����h���Z�b�g
void pfxResetIsland(PfxIsland *island);

} //namespace PhysicsEffects
} //namespace sce

