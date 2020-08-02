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

#include "../../../include/physics_effects/base_level/collision/pfx_contact_manifold.h"

namespace sce {
namespace PhysicsEffects {

#define SCE_PFX_CONTACT_SAME_POINT			0.01f
#define SCE_PFX_CONTACT_THRESHOLD_NORMAL	0.01f	// �Փ˓_��臒l�i�@�������j
#define SCE_PFX_CONTACT_THRESHOLD_TANGENT	0.002f	// �Փ˓_��臒l�i���ʏ�j

int PfxContactManifold::findNearestContactPoint(const PfxPoint3 &newPointA,const PfxPoint3 &newPointB,const PfxVector3 &newNormal,PfxSubData subData)
{
	int nearestIdx = -1;
	PfxFloat minDiff = SCE_PFX_CONTACT_SAME_POINT;
	for(int i=0;i<m_numContacts;i++) {
		PfxFloat diffA = lengthSqr(pfxReadVector3(m_contactPoints[i].m_localPointA) - PfxVector3(newPointA));
		PfxFloat diffB = lengthSqr(pfxReadVector3(m_contactPoints[i].m_localPointB) - PfxVector3(newPointB));
		
		if(m_contactPoints[i].m_subData.m_type == PfxSubData::MESH_INFO && 
			m_contactPoints[i].m_subData.m_facetLocal.islandId == subData.m_facetLocal.islandId &&
			m_contactPoints[i].m_subData.m_facetLocal.facetId == subData.m_facetLocal.facetId &&
			diffA < minDiff && diffB < minDiff ) {
			return i;
		}
		
		if(diffA < minDiff && diffB < minDiff && dot(newNormal,pfxReadVector3(m_contactPoints[i].m_constraintRow[0].m_normal)) > 0.99f) {
			minDiff = SCE_PFX_MAX(diffA,diffB);
			nearestIdx = i;
		}
	}
	return nearestIdx;
}

static inline PfxFloat calcArea4Points(const PfxVector3 &p0,const PfxVector3 &p1,const PfxVector3 &p2,const PfxVector3 &p3)
{
	PfxVector3 a[3],b[3];
	a[0] = p0 - p1;
	a[1] = p0 - p2;
	a[2] = p0 - p3;
	b[0] = p2 - p3;
	b[1] = p1 - p3;
	b[2] = p1 - p2;
	PfxVector3 Ax(a[0].getX(),a[1].getX(),a[2].getX());
	PfxVector3 Ay(a[0].getY(),a[1].getY(),a[2].getY());
	PfxVector3 Az(a[0].getZ(),a[1].getZ(),a[2].getZ());
	PfxVector3 Bx(b[0].getX(),b[1].getX(),b[2].getX());
	PfxVector3 By(b[0].getY(),b[1].getY(),b[2].getY());
	PfxVector3 Bz(b[0].getZ(),b[1].getZ(),b[2].getZ());
	PfxVector3 tmpX = mulPerElem(Ay,Bz)-mulPerElem(Az,By);
	PfxVector3 tmpY = mulPerElem(Az,Bx)-mulPerElem(Ax,Bz);
	PfxVector3 tmpZ = mulPerElem(Ax,By)-mulPerElem(Ay,Bx);
	PfxVector3 area = mulPerElem(tmpX,tmpX) + mulPerElem(tmpY,tmpY) + mulPerElem(tmpZ,tmpZ);
	return maxElem(area);
}

int PfxContactManifold::sort4ContactPoints(const PfxPoint3 &newCP,PfxFloat newDistance)
{
	int maxPenetrationIndex = -1;
	PfxFloat maxPenetration = newDistance;

	// �ł��[���Փ˓_�͔r���Ώۂ���͂���
	for(int i=0;i<m_numContacts;i++) {
		if(m_contactPoints[i].m_distance < maxPenetration) {
			maxPenetrationIndex = i;
			maxPenetration = m_contactPoints[i].m_distance;
		}
	}
	
	PfxFloat res[4] = {0.0f};
	
	// �e�_���������Ƃ��̏Փ˓_�����ʐς̂����A�ł��傫���Ȃ���̂�I��
	PfxVector3 newp(newCP);
	PfxVector3 p[4];
	p[0] = pfxReadVector3(m_contactPoints[0].m_localPointA);
	p[1] = pfxReadVector3(m_contactPoints[1].m_localPointA);
	p[2] = pfxReadVector3(m_contactPoints[2].m_localPointA);
	p[3] = pfxReadVector3(m_contactPoints[3].m_localPointA);

#if 0
	if(maxPenetrationIndex != 0) {
		PfxVector3 a0 = newp-p[1];
		PfxVector3 b0 = p[3]-p[2];
		res[0] = lengthSqr(cross(a0,b0));
	}
 
	if(maxPenetrationIndex != 1) {
		PfxVector3 a1 = newp-p[0];
		PfxVector3 b1 = p[3]-p[2];
		res[1] = lengthSqr(cross(a1,b1));
	}

	if(maxPenetrationIndex != 2) {
		PfxVector3 a2 = newp-p[0];
		PfxVector3 b2 = p[3]-p[1];
		res[2] = lengthSqr(cross(a2,b2));
	}

	if(maxPenetrationIndex != 3) {
		PfxVector3 a3 = newp-p[0];
		PfxVector3 b3 = p[2]-p[1];
		res[3] = lengthSqr(cross(a3,b3));
	}
#else
	if(maxPenetrationIndex != 0) {
		res[0] = calcArea4Points(newp,p[1],p[2],p[3]);
	}
 
	if(maxPenetrationIndex != 1) {
		res[1] = calcArea4Points(newp,p[0],p[2],p[3]);
	}

	if(maxPenetrationIndex != 2) {
		res[2] = calcArea4Points(newp,p[0],p[1],p[3]);
	}

	if(maxPenetrationIndex != 3) {
		res[3] = calcArea4Points(newp,p[0],p[1],p[2]);
	}
#endif

	int maxIndex = 0;
	PfxFloat maxVal = res[0];

	if (res[1] > maxVal) {
		maxIndex = 1;
		maxVal = res[1];
	}

	if (res[2] > maxVal) {
		maxIndex = 2;
		maxVal = res[2];
	}

	if (res[3] > maxVal) {
		maxIndex = 3;
		maxVal = res[3];
	}

	return maxIndex;
}

void PfxContactManifold::addContactPoint(
		PfxFloat newDistance,
		const PfxVector3 &newNormal, // world coords
		const PfxPoint3 &newPointA, // local to the objectA
		const PfxPoint3 &newPointB, // local to the objectB
		PfxSubData subData)
{
	int id = findNearestContactPoint(newPointA,newPointB,newNormal,subData);

	if(id < 0 && m_numContacts < SCE_PFX_NUMCONTACTS_PER_BODIES) {
		// �Փ˓_��V�K�ǉ�
		id = m_numContacts++;
		m_contactPoints[id].reset();
	}
	else if(id < 0){
		// �\�[�g
		id = sort4ContactPoints(newPointA,newDistance);
		m_contactPoints[id].reset();
	}

	m_contactPoints[id].m_distance = newDistance;
	m_contactPoints[id].m_subData = subData;
	pfxStorePoint3(newPointA,m_contactPoints[id].m_localPointA);
	pfxStorePoint3(newPointB,m_contactPoints[id].m_localPointB);
	pfxStoreVector3(newNormal,m_contactPoints[id].m_constraintRow[0].m_normal);
}

void PfxContactManifold::addContactPoint(const PfxContactPoint &cp)
{
	PfxPoint3 pA = pfxReadPoint3(cp.m_localPointA);
	PfxPoint3 pB = pfxReadPoint3(cp.m_localPointB);
	
	int id = findNearestContactPoint(pA,pB,pfxReadVector3(cp.m_constraintRow[0].m_normal),cp.m_subData);
	
	if(id >= 0) {
#if 1
		PfxVector3 nml1(pfxReadVector3(m_contactPoints[id].m_constraintRow[0].m_normal));
		PfxVector3 nml2(pfxReadVector3(cp.m_constraintRow[0].m_normal));
		if(pfxAbsf(dot(nml1,nml2)) > 0.99f ) {
			// ����_�𔭌��A�~�ς��ꂽ�����p��
			m_contactPoints[id].m_distance = cp.m_distance;
			m_contactPoints[id].m_localPointA[0] = cp.m_localPointA[0];
			m_contactPoints[id].m_localPointA[1] = cp.m_localPointA[1];
			m_contactPoints[id].m_localPointA[2] = cp.m_localPointA[2];
			m_contactPoints[id].m_localPointB[0] = cp.m_localPointB[0];
			m_contactPoints[id].m_localPointB[1] = cp.m_localPointB[1];
			m_contactPoints[id].m_localPointB[2] = cp.m_localPointB[2];
			m_contactPoints[id].m_constraintRow[0].m_normal[0] = cp.m_constraintRow[0].m_normal[0];
			m_contactPoints[id].m_constraintRow[0].m_normal[1] = cp.m_constraintRow[0].m_normal[1];
			m_contactPoints[id].m_constraintRow[0].m_normal[2] = cp.m_constraintRow[0].m_normal[2];
		}
		else {
			// ����_�ł͂��邪�@�����Ⴄ���ߍX�V
			m_contactPoints[id] = cp;
		}
#else
		if(m_contactPoints[id].m_distance > cp.m_distance) {
			// ����_�𔭌��A�Փ˓_�����X�V
			m_contactPoints[id].m_distance = cp.m_distance;
			m_contactPoints[id].m_localPointA[0] = cp.m_localPointA[0];
			m_contactPoints[id].m_localPointA[1] = cp.m_localPointA[1];
			m_contactPoints[id].m_localPointA[2] = cp.m_localPointA[2];
			m_contactPoints[id].m_localPointB[0] = cp.m_localPointB[0];
			m_contactPoints[id].m_localPointB[1] = cp.m_localPointB[1];
			m_contactPoints[id].m_localPointB[2] = cp.m_localPointB[2];
			m_contactPoints[id].m_constraintRow[0].m_normal[0] = cp.m_constraintRow[0].m_normal[0];
			m_contactPoints[id].m_constraintRow[0].m_normal[1] = cp.m_constraintRow[0].m_normal[1];
			m_contactPoints[id].m_constraintRow[0].m_normal[2] = cp.m_constraintRow[0].m_normal[2];
		}
#endif
	}
	else if(m_numContacts < SCE_PFX_NUMCONTACTS_PER_BODIES) {
		// �Փ˓_��V�K�ǉ�
		m_contactPoints[m_numContacts++] = cp;
	}
	else {
		// �\�[�g
		id = sort4ContactPoints(pA,cp.m_distance);
		
		// �R���^�N�g�|�C���g����ւ�
		m_contactPoints[id] = cp;
	}
}

void PfxContactManifold::merge(const PfxContactManifold &contact)
{
	SCE_PFX_ALWAYS_ASSERT(m_rigidBodyIdA == contact.getRigidBodyIdA());
	SCE_PFX_ALWAYS_ASSERT(m_rigidBodyIdB == contact.getRigidBodyIdB());

	for(int i=0;i<contact.getNumContacts();i++) {
		addContactPoint(contact.getContactPoint(i));
	}
}

void PfxContactManifold::refresh(const PfxVector3 &pA,const PfxQuat &qA,const PfxVector3 &pB,const PfxQuat &qB)
{
	// �Փ˓_�̍X�V
	// ���Փ˓_�Ԃ̋�����臒l�iCONTACT_THRESHOLD�j�𒴂��������
	for(int i=0;i<(int)m_numContacts;i++) {
		PfxVector3 normal = pfxReadVector3(m_contactPoints[i].m_constraintRow[0].m_normal);
		PfxVector3 cpA = pA + rotate(qA,pfxReadVector3(m_contactPoints[i].m_localPointA));
		PfxVector3 cpB = pB + rotate(qB,pfxReadVector3(m_contactPoints[i].m_localPointB));

		// �ђʐ[�x���v���X�ɓ]�������ǂ������`�F�b�N
		PfxFloat distance = dot(normal,cpA - cpB);
		if(distance > SCE_PFX_CONTACT_THRESHOLD_NORMAL) {
			removeContactPoint(i);
			i--;
			continue;
		}
		m_contactPoints[i].m_distance = distance;

		// �[�x�������������ė��_�̋������`�F�b�N
		cpA = cpA - m_contactPoints[i].m_distance * normal;
		PfxFloat distanceAB = lengthSqr(cpA - cpB);
		if(distanceAB > SCE_PFX_CONTACT_THRESHOLD_TANGENT) {
			removeContactPoint(i);
			i--;
			continue;
		}
		if(m_contactPoints[i].m_duration < 255) m_contactPoints[i].m_duration++;
	}
	if(m_numContacts > 0 && m_duration < 65535) m_duration++;
}
} //namespace PhysicsEffects
} //namespace sce
