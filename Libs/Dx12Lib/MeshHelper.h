/*!
@file MeshHelper.h
@brief �v���~�e�B�u�쐬�֐���
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basedx12 {

	//--------------------------------------------------------------------------------------
	///	���_�֘A�̃��[�e�B���e�B�֐��Q(static�Ăяo������)
	//--------------------------------------------------------------------------------------
	class MeshUtill {
		static const float SQRT2;
		static const float SQRT3;
		static const float SQRT6;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Cylinder / Cone�Ŏg�p�����w���p�[(�������ƕ����C���f�b�N�X����A�x�N�g���𓾂�)
		@param[in]	i	�����C���f�b�N�X
		@param[in]	tessellation	������
		@return	�x�N�g��
		*/
		//--------------------------------------------------------------------------------------
		static inline XMVECTOR GetCircleVector(size_t i, size_t tessellation)
		{
			float angle = i * XM_2PI / tessellation;
			float dx, dz;

			XMScalarSinCos(&dx, &dz, angle);

			XMVECTORF32 v = { dx, 0, dz, 0 };
			return v;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Cylinder / Cone�Ŏg�p�����w���p�[(�������ƕ����C���f�b�N�X����ATangent�x�N�g���𓾂�)
		@param[in]	i	�����C���f�b�N�X
		@param[in]	tessellation	������
		@return	Tangent�x�N�g��
		*/
		//--------------------------------------------------------------------------------------
		static inline XMVECTOR GetCircleTangent(size_t i, size_t tessellation)
		{
			float angle = (i * XM_2PI / tessellation) + XM_PIDIV2;
			float dx, dz;

			XMScalarSinCos(&dx, &dz, angle);

			XMVECTORF32 v = { dx, 0, dz, 0 };
			return v;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Cylinder / Cone�Ŏg�p�����w���p�[(CylinderCap���쐬����)
		@param[out]	vertices	���_�̔z��
		@param[out]	indices �C���f�b�N�X�̔z��
		@param[in]	tessellation	������
		@param[in]	height	����
		@param[in]	radius	���a
		@param[in]	isTop	�㕔���ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void CreateCylinderCap(vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices,
			size_t tessellation, float height, float radius, bool isTop);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���_�ƃC���f�b�N�X�̌����𔽓]������iLH��RH�ϊ��j
		@param[out]	indices �C���f�b�N�X�̔z��
		@param[out]	vertices	���_�̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void ReverseWinding(vector<uint32_t>& indices, vector<VertexPositionNormalTexture>& vertices)
		{
			if ((indices.size() % 3) != 0) {
				throw runtime_error("���_�ƃC���f�b�N�X�̌����𔽓]�ł��܂���B");
			}
			for (auto it = indices.begin(); it != indices.end(); it += 3)
			{
				std::swap(*it, *(it + 2));
			}

			for (auto it = vertices.begin(); it != vertices.end(); ++it)
			{
				it->textureCoordinate.x = (1.f - it->textureCoordinate.x);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTex�t�H�[�}�b�g�̕��ʂ��쐬����(static�֐�)
		@param[in]	size 1�̕ӂ̃T�C�Y
		@param[out]	vertices	���_���쐬���邽�߂̔z��
		@param[out]	indices	�C���f�b�N�X���쐬���邽�߂̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void CreateSquare(float size,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTex�t�H�[�}�b�g�̗����̂��쐬����(static�֐�)
		@param[in]	size 1�̕ӂ̃T�C�Y
		@param[out]	vertices	���_���쐬���邽�߂̔z��
		@param[out]	indices	�C���f�b�N�X���쐬���邽�߂̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void CreateCube(float size,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTex�t�H�[�}�b�g�̋��̂��쐬����(static�֐�)
		@param[in]	diameter ���a
		@param[in]	tessellation ������
		@param[out]	vertices	���_���쐬���邽�߂̔z��
		@param[out]	indices	�C���f�b�N�X���쐬���邽�߂̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void CreateSphere(float diameter, size_t tessellation,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTex�t�H�[�}�b�g�̃J�v�Z�����쐬����(static�֐�)
		@param[in]	diameter ���a
		@param[in]	PointA ���ԕ������̊J�n�_�i���j
		@param[in]	PointB ���ԕ������̏I���_�i��j
		@param[in]	tessellation ������
		@param[out]	vertices	���_���쐬���邽�߂̔z��
		@param[out]	indices	�C���f�b�N�X���쐬���邽�߂̔z��
		@param[in]	landscape ���ɂ��邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void CreateCapsule(float diameter,
			const bsm::Float3& PointA, const bsm::Float3& PointB,
			size_t tessellation,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices,bool landscape = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTex�t�H�[�}�b�g�̃V�����_�[���쐬����(static�֐�)
		@param[in]	height ����
		@param[in]	diameter ���a
		@param[in]	tessellation ������
		@param[out]	vertices	���_���쐬���邽�߂̔z��
		@param[out]	indices	�C���f�b�N�X���쐬���邽�߂̔z��
		@param[in]	landscape ���ɂ��邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void CreateCylinder(float height, float diameter, size_t tessellation,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices,
			 bool landscape = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTex�t�H�[�}�b�g�̃R�[�����쐬����(static�֐�)
		@param[in]	diameter ���a
		@param[in]	height ����
		@param[in]	tessellation ������
		@param[out]	vertices	���_���쐬���邽�߂̔z��
		@param[out]	indices	�C���f�b�N�X���쐬���邽�߂̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void CreateCone(float diameter, float height, size_t tessellation,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTex�t�H�[�}�b�g�̃g�[���X���쐬����(static�֐�)
		@param[in]	diameter �h�[�i�b�c���S�܂ł̒��a
		@param[in]	thickness �h�[�i�b�c����
		@param[in]	tessellation ������
		@param[out]	vertices	���_���쐬���邽�߂̔z��
		@param[out]	indices	�C���f�b�N�X���쐬���邽�߂̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void CreateTorus(float diameter, float thickness, size_t tessellation,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTex�t�H�[�}�b�g�̐�4�ʑ̂��쐬����(static�֐�)
		@param[in]	size �T�C�Y
		@param[out]	vertices	���_���쐬���邽�߂̔z��
		@param[out]	indices	�C���f�b�N�X���쐬���邽�߂̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void CreateTetrahedron(float size,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTex�t�H�[�}�b�g�̐�8�ʑ̂��쐬����(static�֐�)
		@param[in]	size �T�C�Y
		@param[out]	vertices	���_���쐬���邽�߂̔z��
		@param[out]	indices	�C���f�b�N�X���쐬���邽�߂̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void CreateOctahedron(float size,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTex�t�H�[�}�b�g�̐�12�ʑ̂��쐬����(static�֐�)
		@param[in]	size �T�C�Y
		@param[out]	vertices	���_���쐬���邽�߂̔z��
		@param[out]	indices	�C���f�b�N�X���쐬���邽�߂̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void CreateDodecahedron(float size,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTex�t�H�[�}�b�g�̐�20�ʑ̂��쐬����(static�֐�)
		@param[in]	size �T�C�Y
		@param[out]	vertices	���_���쐬���邽�߂̔z��
		@param[out]	indices	�C���f�b�N�X���쐬���邽�߂̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void CreateIcosahedron(float size,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPositionNormalTangentTexture�t�H�[�}�b�g�̃��b�V���̃^���W�F���g��ǉ�����
		@param[inout]	vertices	���_�̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void SetNormalTangent(vector<VertexPositionNormalTangentTexture>& vertices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPositionNormalTangentTextureSkinning�t�H�[�}�b�g�̃��b�V���̃^���W�F���g��ǉ�����
		@param[inout]	vertices	���_�̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void SetNormalTangent(vector<VertexPositionNormalTangentTextureSkinning>& vertices);



	};




}
//end basedx12