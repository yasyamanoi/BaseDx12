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

#include "../../../include/physics_effects/base_level/base/pfx_vec_utils.h"
#include "pfx_contact_tri_mesh_box.h"
#include "pfx_intersect_common.h"
#include "pfx_mesh_common.h"
#include "pfx_decoded_tri_mesh.h"

namespace sce {
namespace PhysicsEffects {

#define CHECK_SAT(axis,AMin,AMax,BMin,BMax) \
{\
	PfxFloat d1 = AMin - BMax;\
	PfxFloat d2 = BMin - AMax;\
	if(distMin < d1) {\
		distMin = d1;\
		axisMin = sepAxis;\
	}\
	if(distMin < d2) {\
		distMin = d2;\
		axisMin = -sepAxis;\
	}\
	if(d1 > 0.0f || d2 > 0.0f) {\
		return false;\
	}\
}

static inline
void projection(const PfxVector3 &boxHalf,const PfxVector3 &boxPos,const PfxMatrix3 &boxOri,const PfxVector3 &axis,PfxFloat &boxMin,PfxFloat &boxMax)
{
	PfxFloat boxHalf_ = 
		fabs(dot(boxHalf[0] * boxOri.getCol(0),axis)) + 
		fabs(dot(boxHalf[1] * boxOri.getCol(1),axis)) + 
		fabs(dot(boxHalf[2] * boxOri.getCol(2),axis));
	boxMin = dot(boxPos,axis) - boxHalf_;
	boxMax = dot(boxPos,axis) + boxHalf_;
}

static
bool pfxContactTriangleBox(PfxContactCache &contacts,PfxUInt32 facetId,
							const PfxVector3 &normal,const PfxVector3 &p0,const PfxVector3 &p1,const PfxVector3 &p2,
							const PfxFloat thickness,const PfxFloat angle0,const PfxFloat angle1,const PfxFloat angle2,
							PfxUInt32 edgeChk,
							const PfxVector3 &boxHalf,const PfxTransform3 &transformB)
{
	const PfxFloat epsilon = 0.00001f;
	
	// �ł��󂢊ђʐ[�x�Ƃ��̂Ƃ��̕�����
	PfxFloat distMin = -SCE_PFX_FLT_MAX;
	PfxVector3 axisMin(0.0f);
	
	PfxVector3 boxPos = transformB.getTranslation();
	PfxMatrix3 boxOri = transformB.getUpper3x3();
	
	//-------------------------------------------
	// �P�D����������
	
	{
		PfxVector3 facetPnts[6] = {
			p0,p1,p2,p0-thickness*normal,p1-thickness*normal,p2-thickness*normal
		};

		PfxVector3 sideNml[3] = {
			normalize(cross((facetPnts[1] - facetPnts[0]),normal)),
			normalize(cross((facetPnts[2] - facetPnts[1]),normal)),
			normalize(cross((facetPnts[0] - facetPnts[2]),normal)),
		};


		// Triangles�̖� -> Box
		{
			// ������
			const PfxVector3 sepAxis = normal;

			// ��������
			PfxPlane planeA(sepAxis,p0);
			
			// Box�𕪗����ɓ��e���Ĕ͈͂��擾
			PfxFloat boxMax,boxMin;
			projection(boxHalf,boxPos-p0,boxOri,sepAxis,boxMin,boxMax);
			
			// ����
			if(boxMin > 0.0f || boxMax < -thickness) {
				return false;
			}

			if(distMin < boxMin) {
				distMin = boxMin;
				axisMin = -sepAxis;
			}
		}

		// Box -> Triangles
		for(int bf=0;bf<3;bf++) {
			// ������
			PfxVector3 sepAxis = boxOri.getCol(bf);

			// Triangles�𕪗����ɓ��e���Ĕ͈͂��擾
			PfxFloat triMin,triMax;
			pfxGetProjAxisPnts6(facetPnts,sepAxis,triMin,triMax);

			// Box�𕪗����ɓ��e���Ĕ͈͂��擾
			PfxFloat boxMin = dot(boxPos,sepAxis)-boxHalf[bf];
			PfxFloat boxMax = dot(boxPos,sepAxis)+boxHalf[bf];
			
			CHECK_SAT(sepAxis,triMin,triMax,boxMin,boxMax)
		}

		// �G�b�W Triangles�ʂ̃G�b�W(x3)�~Box�̃G�b�W(x3)
		for(int e=0;e<3;e++) {
			PfxVector3 dir = normalize(facetPnts[(e+1)%3] - facetPnts[e]);

			for(int i=0;i<3;i++) {
				PfxVector3 boxEdge = boxOri.getCol(i);
				
				// �G�b�W�����s�ł���Δ��肵�Ȃ�
				if(pfxIsSameDirection(dir,boxEdge)) continue;

				PfxVector3 sepAxis = normalize(cross(dir,boxEdge));

				// Triangles�𕪗����ɓ��e���Ĕ͈͂��擾
				PfxFloat triMin,triMax;
				pfxGetProjAxisPnts6(facetPnts,sepAxis,triMin,triMax);

				// Box�𕪗����ɓ��e���Ĕ͈͂��擾
				PfxFloat boxMax,boxMin;
				projection(boxHalf,boxPos,boxOri,sepAxis,boxMin,boxMax);

				CHECK_SAT(sepAxis,triMin,triMax,boxMin,boxMax)
			}
		}

		// �ʂɌ��݂�����ꍇ�̕⏕�I�Ȕ���i�������邩���Ȃ��������𔻒�j
		if(thickness > SCE_PFX_THICKNESS_THRESHOLD) {
			// ���ݑ��ʂ̖@��
			for(int i=0;i<3;i++) {
				// ��������
				PfxPlane planeA(sideNml[i],facetPnts[i]);

				// Box�𕪗����ɓ��e���Ĕ͈͂��擾
				PfxFloat r = dot(boxHalf,absPerElem(sideNml[i]));
				PfxFloat boxOffset = planeA.onPlane(boxPos);
				PfxFloat boxMin = boxOffset - r;

				if(boxMin > 0.0f) {
					return false;
				}
			}

			// �Q�̌��ݑ��ʂ̂Ȃ��G�b�W3�~�{�b�N�X�̃G�b�W3
			for(int e=0;e<3;e++) {
				PfxVector3 edgeVec = normalize(cross(sideNml[(e+1)%3],sideNml[e]));

				for(int i=0;i<3;i++) {
					PfxVector3 boxEdge(0.0f);
					boxEdge[i] = 1.0f;
					
					// �G�b�W�����s�ł���Δ��肵�Ȃ�
					if(pfxIsSameDirection(edgeVec,boxEdge)) continue;

					PfxVector3 sepAxis = normalize(cross(edgeVec,boxEdge));

					// Triangles�𕪗����ɓ��e���Ĕ͈͂��擾
					PfxFloat triMin,triMax;
					pfxGetProjAxisPnts3(facetPnts,sepAxis,triMin,triMax);

					// Box�𕪗����ɓ��e���Ĕ͈͂��擾
					PfxFloat boxMax,boxMin;
					projection(boxHalf,boxPos,boxOri,sepAxis,boxMin,boxMax);

					if(triMax < boxMin || boxMax < triMin) {
						return false;
					}
				}
			}
		}
	}
	

	//-------------------------------------------
	// �Q�D�ŋߐږʂ̒T��

	int faceB=0; // X
	int signB=1; // +

	{
		PfxFloat f,maxf;
		
		f = dot(axisMin,boxOri.getCol0());
		maxf=f;faceB=0;signB=1;
		f = dot(axisMin,boxOri.getCol1());
		if(maxf < f) {maxf=f;faceB=1;signB=1;}
		f = dot(axisMin,boxOri.getCol2());
		if(maxf < f) {maxf=f;faceB=2;signB=1;}
		f = dot(axisMin,-boxOri.getCol0());
		if(maxf < f) {maxf=f;faceB=0;signB=-1;}
		f = dot(axisMin,-boxOri.getCol1());
		if(maxf < f) {maxf=f;faceB=1;signB=-1;}
		f = dot(axisMin,-boxOri.getCol2());
		if(maxf < f) {maxf=f;faceB=2;signB=-1;}
	}
	
	//-------------------------------------------
	// �R�D�Փ˓_�̒T��
	
	{
		// �����������Ɉ�������(�ŋߐڂ𔻒肷�邽�߁A�������������)
		PfxVector3 sepAxis = 1.1f * pfxAbsf(distMin) * axisMin;
		
		const PfxVector3 facetPnts[3] = {
			p0 + sepAxis,
			p1 + sepAxis,
			p2 + sepAxis,
		};
		
		const PfxVector3 boxPnts[4] = {
			boxPos + (float)signB * boxHalf[faceB] * boxOri.getCol(faceB) + 
				(float)signB * boxHalf[(faceB+1)%3] * boxOri.getCol((faceB+1)%3) + 
				(float)signB * boxHalf[(faceB+2)%3] * boxOri.getCol((faceB+2)%3),
			boxPos + (float)signB * boxHalf[faceB] * boxOri.getCol(faceB) + 
				(float)signB * -boxHalf[(faceB+1)%3] * boxOri.getCol((faceB+1)%3) + 
				(float)signB * boxHalf[(faceB+2)%3] * boxOri.getCol((faceB+2)%3),
			boxPos + (float)signB * boxHalf[faceB] * boxOri.getCol(faceB) + 
				(float)signB * -boxHalf[(faceB+1)%3] * boxOri.getCol((faceB+1)%3) + 
				(float)signB * -boxHalf[(faceB+2)%3] * boxOri.getCol((faceB+2)%3),
			boxPos + (float)signB * boxHalf[faceB] * boxOri.getCol(faceB) + 
				(float)signB * boxHalf[(faceB+1)%3] * boxOri.getCol((faceB+1)%3) + 
				(float)signB * -boxHalf[(faceB+2)%3] * boxOri.getCol((faceB+2)%3),
		};
		
		//--------------------------------------------------------------------
		// �Փ˓_�̒T��
		
		PfxClosestPoints cp;
		PfxVector3 sA,sB;
		
		// �G�b�W�Ԃ̍ŒZ�����ƍ��W�l���Z�o
		{
			for(int i=0;i<3;i++) {
				for(int j=0;j<4;j++) {
					pfxClosestTwoLines(facetPnts[i],facetPnts[(i+1)%3],boxPnts[j],boxPnts[(j+1)%4],sA,sB);
					cp.add(PfxPoint3(sA),PfxPoint3(sB),lengthSqr(sA-sB));
				}
			}
		}
		
		// Triangle�̒��_ -> Box�̖�
		{
			PfxTransform3 transformBInv = orthoInverse(transformB);
			pfxClosestPointAABB(PfxVector3(transformBInv * PfxPoint3(facetPnts[0])),boxHalf,sB);
			sB = PfxVector3(transformB * PfxPoint3(sB));
			cp.add(PfxPoint3(facetPnts[0]),PfxPoint3(sB),lengthSqr(sB-facetPnts[0]));
			
			pfxClosestPointAABB(PfxVector3(transformBInv * PfxPoint3(facetPnts[1])),boxHalf,sB);
			sB = PfxVector3(transformB * PfxPoint3(sB));
			cp.add(PfxPoint3(facetPnts[1]),PfxPoint3(sB),lengthSqr(sB-facetPnts[1]));
			
			pfxClosestPointAABB(PfxVector3(transformBInv * PfxPoint3(facetPnts[2])),boxHalf,sB);
			sB = PfxVector3(transformB * PfxPoint3(sB));
			cp.add(PfxPoint3(facetPnts[2]),PfxPoint3(sB),lengthSqr(sB-facetPnts[2]));
		}

		// Box�̒��_ -> Triangles�̖�
		PfxTriangle triangleA(facetPnts[0],facetPnts[1],facetPnts[2]);
		for(int i=0;i<4;i++) {
			pfxClosestPointTriangle(boxPnts[i],triangleA,sA);
			cp.add(PfxPoint3(sA),PfxPoint3(boxPnts[i]),lengthSqr(sA-boxPnts[i]));
		}
		
		for(int i=0;i<cp.numPoints;i++) {
			if(cp.distSqr[i] < cp.closestDistSqr + epsilon) {
				cp.pA[i] -= sepAxis;
				
				// �Փ˓_�����R�ȃG�b�W��ł���Ζ@����ς���
				if( (((edgeChk&0x03)==0)&&pfxPointOnLine(PfxVector3(cp.pA[i]),p0,p1)) ||
					(((edgeChk&0x0c)==0)&&pfxPointOnLine(PfxVector3(cp.pA[i]),p1,p2)) ||
					(((edgeChk&0x30)==0)&&pfxPointOnLine(PfxVector3(cp.pA[i]),p2,p0)) ) {
					axisMin=-normal;
				}
				
				PfxSubData subData;
				subData.setFacetId(facetId);
				contacts.addContactPoint(-length(cp.pB[i]-cp.pA[i]),axisMin,cp.pA[i],cp.pB[i],subData);
			}
		}
	}
	
	return true;
}

PfxInt32 pfxContactTriMeshBox(
	PfxContactCache &contacts,
	const PfxLargeTriMesh *largeMeshA,const PfxQuantizedTriMesh *meshA,
	const PfxBox &boxB,
	const PfxTransform3 &transformB,
	PfxFloat distanceThreshold)
{
	(void) distanceThreshold;
	
	//-------------------------------------------
	// ���肷��ʂ��i�荞��

	PfxUInt8 SCE_PFX_ALIGNED(16) selFacets[SCE_PFX_NUMMESHFACETS] = {0};

	PfxVector3 center = transformB.getTranslation();
	PfxVector3 half = absPerElem(transformB.getUpper3x3()) * boxB.m_half;
	PfxQuantize3 quantizedMinHalf = pfxHalve(largeMeshA->quantizePosition(center - half));
	PfxQuantize3 quantizedMaxHalf = pfxHalve(largeMeshA->quantizePosition(center + half));
	PfxUInt32 numSelFacets = pfxGatherFacets(meshA,quantizedMinHalf+quantizedMaxHalf,quantizedMaxHalf-quantizedMinHalf,selFacets);

	if(numSelFacets == 0) {
		return 0;
	}
	
	//-------------------------------------------
	// ����������(SAT)
	// ��Box���W�n (B���[�J��)�Ŕ���
	
	PfxDecodedTriMesh decodedMesh;
	PfxContactCache localContacts;
	
	for(PfxUInt32 f = 0; f < numSelFacets; f++) {
		const PfxQuantizedFacet &facet = meshA->m_facets[selFacets[f]];
		
		// �f�R�[�h
		PfxDecodedFacet decodedFacet;
		const PfxUInt32 vId[3] = {facet.m_vertIds[0],facet.m_vertIds[1],facet.m_vertIds[2]};

		decodedFacet.m_normal = largeMeshA->decodeNormal(facet.m_normal);
		decodedFacet.m_thickness = largeMeshA->decodeFloat(facet.m_thickness);

	decodedFacet.m_normal = largeMeshA->decodeNormal(facet.m_normal);
	decodedFacet.m_thickness = largeMeshA->decodeFloat(facet.m_thickness);

	for(int v=0;v<3;v++) {
		if(!decodedMesh.isDecodedVertex(vId[v])) {
			decodedMesh.m_verts[vId[v]] = largeMeshA->decodePosition(meshA->m_verts[vId[v]]);
			decodedMesh.m_decodedVertex[vId[v]>>5] |= 1 << (vId[v]&0x1f);
		}
	}

		PfxVector3 facetNormal = decodedFacet.m_normal;

		PfxVector3 facetPntsA[3] = {
			decodedMesh.m_verts[vId[0]],
			decodedMesh.m_verts[vId[1]],
			decodedMesh.m_verts[vId[2]],
		};

		const PfxEdge *edge[3] = {
			&meshA->m_edges[facet.m_edgeIds[0]],
			&meshA->m_edges[facet.m_edgeIds[1]],
			&meshA->m_edges[facet.m_edgeIds[2]],
		};
		
		PfxUInt32 edgeChk = edge[0]->m_angleType | (edge[1]->m_angleType << 2) | (edge[2]->m_angleType << 4);
		
		pfxContactTriangleBox(localContacts,selFacets[f],
							facetNormal,facetPntsA[0],facetPntsA[1],facetPntsA[2],
							decodedFacet.m_thickness,
							0.5f*SCE_PFX_PI*(edge[0]->m_tilt/255.0f),
							0.5f*SCE_PFX_PI*(edge[1]->m_tilt/255.0f),
							0.5f*SCE_PFX_PI*(edge[2]->m_tilt/255.0f),
							edgeChk,boxB.m_half,transformB);
	}
	
	for(int i=0;i<localContacts.getNumContacts();i++) {
		PfxSubData subData = localContacts.getSubData(i);
		
		const PfxQuantizedFacet &facet = meshA->m_facets[subData.getFacetId()];

		PfxTriangle triangleA(
			decodedMesh.m_verts[facet.m_vertIds[0]],
			decodedMesh.m_verts[facet.m_vertIds[1]],
			decodedMesh.m_verts[facet.m_vertIds[2]]);
		
		PfxFloat s=0.0f,t=0.0f;
		pfxGetLocalCoords(PfxVector3(localContacts.getLocalPointA(i)),triangleA,s,t);
		subData.m_type = PfxSubData::MESH_INFO;
		subData.setFacetLocalS(s);
		subData.setFacetLocalT(t);
		subData.setUserData(facet.m_userData);
		
		contacts.addContactPoint(
			localContacts.getDistance(i),
			localContacts.getNormal(i),
			localContacts.getLocalPointA(i),
			localContacts.getLocalPointB(i),
			subData);
	}
	
	return contacts.getNumContacts();
}

PfxInt32 pfxContactTriMeshBox(
	PfxContactCache &contacts,
	const PfxLargeTriMesh *largeMeshA,const PfxExpandedTriMesh *meshA,
	const PfxBox &boxB,
	const PfxTransform3 &transformB, // A���[�J����B
	PfxFloat distanceThreshold)
{
	(void) largeMeshA;
	(void) distanceThreshold;

	//-------------------------------------------
	// ���肷��ʂ��i�荞��

	PfxUInt8 SCE_PFX_ALIGNED(16) selFacets[SCE_PFX_NUMMESHFACETS] = {0};
	PfxUInt32 numSelFacets = pfxGatherFacets(meshA,
		transformB.getTranslation(),
		absPerElem(transformB.getUpper3x3()) * boxB.m_half,selFacets);
	
	if(numSelFacets == 0) {
		return 0;
	}
	
	//-------------------------------------------
	// ����������(SAT)
	// ��Box���W�n (B���[�J��)�Ŕ���
	
	PfxContactCache localContacts;
	
	for(PfxUInt32 f = 0; f < numSelFacets; f++) {
		const PfxExpandedFacet &facet = meshA->m_facets[selFacets[f]];

		PfxVector3 facetNormal = facet.m_normal;

		PfxVector3 facetPntsA[3] = {
			meshA->m_verts[facet.m_vertIds[0]],
			meshA->m_verts[facet.m_vertIds[1]],
			meshA->m_verts[facet.m_vertIds[2]],
		};

		const PfxEdge *edge[3] = {
			&meshA->m_edges[facet.m_edgeIds[0]],
			&meshA->m_edges[facet.m_edgeIds[1]],
			&meshA->m_edges[facet.m_edgeIds[2]],
		};
		
		PfxUInt32 edgeChk = edge[0]->m_angleType | (edge[1]->m_angleType << 2) | (edge[2]->m_angleType << 4);
		
		pfxContactTriangleBox(localContacts,selFacets[f],
							facetNormal,facetPntsA[0],facetPntsA[1],facetPntsA[2],
							facet.m_thickness,
							0.5f*SCE_PFX_PI*(edge[0]->m_tilt/255.0f),
							0.5f*SCE_PFX_PI*(edge[1]->m_tilt/255.0f),
							0.5f*SCE_PFX_PI*(edge[2]->m_tilt/255.0f),
							edgeChk,boxB.m_half,transformB);
	}
	
	for(int i=0;i<localContacts.getNumContacts();i++) {
		PfxSubData subData = localContacts.getSubData(i);
		
		const PfxExpandedFacet &facet = meshA->m_facets[subData.getFacetId()];

		PfxTriangle triangleA(
			meshA->m_verts[facet.m_vertIds[0]],
			meshA->m_verts[facet.m_vertIds[1]],
			meshA->m_verts[facet.m_vertIds[2]]);

		PfxFloat s=0.0f,t=0.0f;
		pfxGetLocalCoords(PfxVector3(localContacts.getLocalPointA(i)),triangleA,s,t);
		subData.m_type = PfxSubData::MESH_INFO;
		subData.setFacetLocalS(s);
		subData.setFacetLocalT(t);
		subData.setUserData(facet.m_userData);
		
		contacts.addContactPoint(
			localContacts.getDistance(i),
			localContacts.getNormal(i),
			localContacts.getLocalPointA(i),
			localContacts.getLocalPointB(i),
			subData);
	}
	
	return contacts.getNumContacts();
}


PfxInt32 pfxContactTriMeshBox(
	PfxContactCache &contacts,
	const PfxLargeTriMesh *largeMeshA,const PfxQuantizedTriMeshBvh *meshA,
	const PfxBox &boxB,
	const PfxTransform3 &transformB,
	PfxFloat distanceThreshold)
{
	(void) distanceThreshold;

	//-------------------------------------------
	// Box��AABB

	PfxVecInt3 aabbMinL,aabbMaxL;

	largeMeshA->getLocalPosition(
		(transformB.getTranslation() - absPerElem(transformB.getUpper3x3()) * boxB.m_half),
		(transformB.getTranslation() + absPerElem(transformB.getUpper3x3()) * boxB.m_half),
		aabbMinL,aabbMaxL);

	PfxAabb16 aabbB;
	
	pfxSetXMin(aabbB,aabbMinL.getX());
	pfxSetXMax(aabbB,aabbMaxL.getX());
	pfxSetYMin(aabbB,aabbMinL.getY());
	pfxSetYMax(aabbB,aabbMaxL.getY());
	pfxSetZMin(aabbB,aabbMinL.getZ());
	pfxSetZMax(aabbB,aabbMaxL.getZ());

	//-------------------------------------------
	// BVH�T��

	PfxUInt8 SCE_PFX_ALIGNED(16) selFacets[SCE_PFX_NUMMESHFACETS] = {0};
	PfxUInt32 numSelFacets = pfxGatherFacets(meshA,aabbB,selFacets);
	
	if(numSelFacets == 0) {
		return 0;
	}

	PfxDecodedTriMesh decodedMesh;
	PfxContactCache localContacts;
	
	for(PfxUInt32 f = 0; f < numSelFacets; f++) {
		//-------------------------------------------
		// ���������ʂƂ̕���������(SAT)
		// ��Box���W�n (B���[�J��)�Ŕ���

		const PfxQuantizedFacetBvh &facet = meshA->m_facets[selFacets[f]];
		
		// �f�R�[�h
		PfxDecodedFacet decodedFacet;
		const PfxUInt32 vId[3] = {facet.m_vertIds[0],facet.m_vertIds[1],facet.m_vertIds[2]};

		decodedFacet.m_normal = largeMeshA->decodeNormal(facet.m_normal);
		decodedFacet.m_thickness = largeMeshA->decodeFloat(facet.m_thickness);

		for(int v=0;v<3;v++) {
			if(!decodedMesh.isDecodedVertex(vId[v])) {
				decodedMesh.m_verts[vId[v]] = largeMeshA->decodePosition(meshA->m_verts[vId[v]]);
				decodedMesh.m_decodedVertex[vId[v]>>5] |= 1 << (vId[v]&0x1f);
			}
		}
		
		PfxVector3 facetNormal = decodedFacet.m_normal;
		
		PfxVector3 facetPntsA[3] = {
			decodedMesh.m_verts[vId[0]],
			decodedMesh.m_verts[vId[1]],
			decodedMesh.m_verts[vId[2]],
		};
		
		const PfxEdge *edge[3] = {
			&meshA->m_edges[facet.m_edgeIds[0]],
			&meshA->m_edges[facet.m_edgeIds[1]],
			&meshA->m_edges[facet.m_edgeIds[2]],
		};
		
		PfxUInt32 edgeChk = edge[0]->m_angleType | (edge[1]->m_angleType << 2) | (edge[2]->m_angleType << 4);
		
		pfxContactTriangleBox(localContacts,selFacets[f],
							facetNormal,facetPntsA[0],facetPntsA[1],facetPntsA[2],
							decodedFacet.m_thickness,
							0.5f*SCE_PFX_PI*(edge[0]->m_tilt/255.0f),
							0.5f*SCE_PFX_PI*(edge[1]->m_tilt/255.0f),
							0.5f*SCE_PFX_PI*(edge[2]->m_tilt/255.0f),
							edgeChk,boxB.m_half,transformB);
	}
	
	for(int i=0;i<localContacts.getNumContacts();i++) {
		PfxSubData subData = localContacts.getSubData(i);
		
		const PfxQuantizedFacetBvh &facet = meshA->m_facets[subData.getFacetId()];
		
		PfxTriangle triangleA(
			decodedMesh.m_verts[facet.m_vertIds[0]],
			decodedMesh.m_verts[facet.m_vertIds[1]],
			decodedMesh.m_verts[facet.m_vertIds[2]]);

		PfxFloat s=0.0f,t=0.0f;
		pfxGetLocalCoords(PfxVector3(localContacts.getLocalPointA(i)),triangleA,s,t);
		subData.m_type = PfxSubData::MESH_INFO;
		subData.setFacetLocalS(s);
		subData.setFacetLocalT(t);
		subData.setUserData(facet.m_userData);
		
		contacts.addContactPoint(
			localContacts.getDistance(i),
			localContacts.getNormal(i),
			localContacts.getLocalPointA(i),
			localContacts.getLocalPointB(i),
			subData);
	}

	return contacts.getNumContacts();
}

PfxInt32 pfxContactTriMeshBox(
	PfxContactCache &contacts,
	const PfxLargeTriMesh *largeMeshA,const PfxExpandedTriMeshBvh *meshA,
	const PfxBox &boxB,
	const PfxTransform3 &transformB,
	PfxFloat distanceThreshold)
{
	(void) distanceThreshold;

	//-------------------------------------------
	// Box��AABB

	PfxVecInt3 aabbMinL,aabbMaxL;

	largeMeshA->getLocalPosition(
		(transformB.getTranslation() - absPerElem(transformB.getUpper3x3()) * boxB.m_half),
		(transformB.getTranslation() + absPerElem(transformB.getUpper3x3()) * boxB.m_half),
		aabbMinL,aabbMaxL);

	PfxAabb16 aabbB;
	
	pfxSetXMin(aabbB,aabbMinL.getX());
	pfxSetXMax(aabbB,aabbMaxL.getX());
	pfxSetYMin(aabbB,aabbMinL.getY());
	pfxSetYMax(aabbB,aabbMaxL.getY());
	pfxSetZMin(aabbB,aabbMinL.getZ());
	pfxSetZMax(aabbB,aabbMaxL.getZ());

	//-------------------------------------------
	// BVH�T��

	PfxUInt8 SCE_PFX_ALIGNED(16) selFacets[SCE_PFX_NUMMESHFACETS] = {0};
	PfxUInt32 numSelFacets = pfxGatherFacets(meshA,aabbB,selFacets);
	
	if(numSelFacets == 0) {
		return 0;
	}
	
	PfxContactCache localContacts;
	
	for(PfxUInt32 f = 0; f < numSelFacets; f++) {
		//-------------------------------------------
		// ���������ʂƂ̕���������(SAT)
		// ��Box���W�n (B���[�J��)�Ŕ���

		const PfxExpandedFacetBvh &facet = meshA->m_facets[selFacets[f]];
		
		PfxVector3 facetNormal = facet.m_normal;
		
		PfxVector3 facetPntsA[3] = {
			meshA->m_verts[facet.m_vertIds[0]],
			meshA->m_verts[facet.m_vertIds[1]],
			meshA->m_verts[facet.m_vertIds[2]],
		};
		
		const PfxEdge *edge[3] = {
			&meshA->m_edges[facet.m_edgeIds[0]],
			&meshA->m_edges[facet.m_edgeIds[1]],
			&meshA->m_edges[facet.m_edgeIds[2]],
		};
		
		PfxUInt32 edgeChk = edge[0]->m_angleType | (edge[1]->m_angleType << 2) | (edge[2]->m_angleType << 4);

		
		pfxContactTriangleBox(localContacts,selFacets[f],
							facetNormal,facetPntsA[0],facetPntsA[1],facetPntsA[2],
							facet.m_thickness,
							0.5f*SCE_PFX_PI*(edge[0]->m_tilt/255.0f),
							0.5f*SCE_PFX_PI*(edge[1]->m_tilt/255.0f),
							0.5f*SCE_PFX_PI*(edge[2]->m_tilt/255.0f),
							edgeChk,boxB.m_half,transformB);
	}
	
	for(int i=0;i<localContacts.getNumContacts();i++) {
		PfxSubData subData = localContacts.getSubData(i);
		
		const PfxExpandedFacetBvh &facet = meshA->m_facets[subData.getFacetId()];
		
		PfxTriangle triangleA(
			meshA->m_verts[facet.m_vertIds[0]],
			meshA->m_verts[facet.m_vertIds[1]],
			meshA->m_verts[facet.m_vertIds[2]]);
		
		PfxFloat s=0.0f,t=0.0f;
		pfxGetLocalCoords(PfxVector3(localContacts.getLocalPointA(i)),triangleA,s,t);
		subData.m_type = PfxSubData::MESH_INFO;
		subData.setFacetLocalS(s);
		subData.setFacetLocalT(t);
		subData.setUserData(facet.m_userData);
		
		contacts.addContactPoint(
			localContacts.getDistance(i),
			localContacts.getNormal(i),
			localContacts.getLocalPointA(i),
			localContacts.getLocalPointB(i),
			subData);
	}

	return contacts.getNumContacts();
}


} //namespace PhysicsEffects
} //namespace sce
