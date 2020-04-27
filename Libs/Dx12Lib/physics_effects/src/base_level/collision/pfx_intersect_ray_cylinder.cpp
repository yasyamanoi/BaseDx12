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
#include "stdafx.h"

#include "pfx_intersect_common.h"
#include "pfx_intersect_ray_cylinder.h"

namespace sce {
namespace PhysicsEffects {

PfxBool pfxIntersectRayCylinder(const PfxRayInput &ray,PfxRayOutput &out,const PfxCylinder &cylinder,const PfxTransform3 &transform)
{
	// ���C���~���̃��[�J�����W�֕ϊ�
	PfxTransform3 transformCapsule = orthoInverse(transform);
	PfxVector3 startPosL = transformCapsule.getUpper3x3() * ray.m_startPosition + transformCapsule.getTranslation();
	PfxVector3 rayDirL = transformCapsule.getUpper3x3() * ray.m_direction;
	
	PfxFloat radSqr = cylinder.m_radius * cylinder.m_radius;

	// �n�_���~���̓����ɂ��邩����
	{
		PfxFloat h = startPosL[0];
		if(-cylinder.m_halfLen <= h && h <= cylinder.m_halfLen) {
			PfxVector3 Px(h,0,0);
			PfxFloat sqrLen = lengthSqr(startPosL-Px);
			if(sqrLen <= radSqr) return false;
		}
	}

	// �~���̓��̂Ƃ̌�������
	do {
		PfxVector3 P(startPosL);
		PfxVector3 D(rayDirL);
		
		P[0] = 0.0f;
		D[0] = 0.0f;
		
		PfxFloat a = dot(D,D);
		PfxFloat b = dot(P,D);
		PfxFloat c = dot(P,P) - radSqr;
		
		PfxFloat d = b * b - a * c;
		
		if(d < 0.0f) return false; // ���C�͈��Ă���
		if(pfxAbsf(a) < 0.00001f) break; // ���C��X���ɕ��s
		
		PfxFloat tt = ( -b - sqrtf(d) ) / a;
		
		if(tt < 0.0f || tt > 1.0f) break;
		
		if(tt < out.m_variable) {
			PfxVector3 cp = startPosL + tt * rayDirL;
			
			if(pfxAbsf(cp[0]) <= cylinder.m_halfLen) {
				out.m_contactFlag = true;
				out.m_variable = tt;
				out.m_contactPoint = PfxVector3(transform * PfxPoint3(cp));
				out.m_contactNormal = transform.getUpper3x3() * normalize(cp);
				out.m_subData.m_type = PfxSubData::NONE;
				return true;
			}
		}
	} while(0);
	
	// �~���̗��[�ɂ��镽�ʂƂ̌�������
	{
		if(pfxAbsf(rayDirL[0]) < 0.00001f) return false;
		
		PfxFloat t1 = ( cylinder.m_halfLen - startPosL[0] ) / rayDirL[0];
		PfxFloat t2 = ( - cylinder.m_halfLen - startPosL[0] ) / rayDirL[0];

		PfxFloat tt = SCE_PFX_MIN(t1,t2);
		
		if(tt < 0.0f || tt > 1.0f) return false;

		PfxVector3 p = startPosL + tt * rayDirL;
		p[0] = 0.0f;

		if(lengthSqr(p) < radSqr && tt < out.m_variable) {
			PfxVector3 cp = startPosL + tt * rayDirL;
			out.m_contactFlag = true;
			out.m_variable = tt;
			out.m_contactPoint = ray.m_startPosition + tt * ray.m_direction;
			out.m_contactNormal = transform.getUpper3x3() * ((cp[0]>0.0f)?PfxVector3(1.0,0.0,0.0):PfxVector3(-1.0,0.0,0.0));
			out.m_subData.m_type = PfxSubData::NONE;
			return true;
		}
	}
	
	return false;
}
} //namespace PhysicsEffects
} //namespace sce
