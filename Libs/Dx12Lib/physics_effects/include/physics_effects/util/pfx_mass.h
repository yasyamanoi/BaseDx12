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

///////////////////////////////////////////////////////////////////////////////
//E Mass , Inertia tensor calculation
//J ���ʁE�����e���\���̎Z�o

#pragma once
#include "stdafx.h"

#include "../base_level/base/pfx_common.h"

namespace sce {
namespace PhysicsEffects {

// Box
PfxFloat pfxCalcMassBox(PfxFloat density,const PfxVector3 &halfExtent);
PfxMatrix3 pfxCalcInertiaBox(const PfxVector3 &halfExtent,PfxFloat mass);

// Sphere
PfxFloat pfxCalcMassSphere(PfxFloat density,PfxFloat radius);
PfxMatrix3 pfxCalcInertiaSphere(PfxFloat radius,PfxFloat mass);

// Cylinder
PfxFloat pfxCalcMassCylinder(PfxFloat density,PfxFloat halfLength,PfxFloat radius);
PfxMatrix3 pfxCalcInertiaCylinderX(PfxFloat halfLength,PfxFloat radius,PfxFloat mass);
PfxMatrix3 pfxCalcInertiaCylinderY(PfxFloat halfLength,PfxFloat radius,PfxFloat mass);
PfxMatrix3 pfxCalcInertiaCylinderZ(PfxFloat halfLength,PfxFloat radius,PfxFloat mass);

///////////////////////////////////////////////////////////////////////////////
//E Mass convertion
//J ���ʂ̈ړ��E��]�E����

// translation
//E returns translated inertia tensor
//J �ړ���̊����e���\����Ԃ��܂�
PfxMatrix3 pfxMassTranslate(PfxFloat mass,const PfxMatrix3 &inertia,const PfxVector3 &translation);

// rotation
//E returns rotated inertia tensor
//J ��]��̊����e���\����Ԃ��܂�
PfxMatrix3 pfxMassRotate(const PfxMatrix3 &inertia,const PfxMatrix3 &rotate);

} //namespace PhysicsEffects
} //namespace sce

