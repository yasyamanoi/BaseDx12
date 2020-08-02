
#pragma once
#include "stdafx.h"

namespace basedx12 {

	//--------------------------------------------------------------------------------------
	///	頂点関連のユーティリティ関数群(static呼び出しをす)
	//--------------------------------------------------------------------------------------
	class MeshUtill {
		static const float SQRT2;
		static const float SQRT3;
		static const float SQRT6;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Cylinder / Coneで使用されるヘルパー(分割数と分割インデックスから、ベクトルを得る)
		@param[in]	i	分割インデックス
		@param[in]	tessellation	分割数
		@return	ベクトル
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
		@brief	Cylinder / Coneで使用されるヘルパー(分割数と分割インデックスから、Tangentベクトルを得る)
		@param[in]	i	分割インデックス
		@param[in]	tessellation	分割数
		@return	Tangentベクトル
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
		@brief	Cylinder / Coneで使用されるヘルパー(CylinderCapを作成する)
		@param[out]	vertices	頂点の配列
		@param[out]	indices インデックスの配列
		@param[in]	tessellation	分割数
		@param[in]	height	高さ
		@param[in]	radius	半径
		@param[in]	isTop	上部かどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateCylinderCap(vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices,
			size_t tessellation, float height, float radius, bool isTop);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点とインデックスの向きを反転させる（LHとRH変換）
		@param[out]	indices インデックスの配列
		@param[out]	vertices	頂点の配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void ReverseWinding(vector<uint32_t>& indices, vector<VertexPositionNormalTexture>& vertices)
		{
			if ((indices.size() % 3) != 0) {
				throw runtime_error("頂点とインデックスの向きを反転できません。");
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
		@brief	VertexPosNormalTexフォーマットの平面を作成する(static関数)
		@param[in]	size 1つの辺のサイズ
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateSquare(float size,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTexフォーマットの立方体を作成する(static関数)
		@param[in]	size 1つの辺のサイズ
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateCube(float size,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTexフォーマットの球体を作成する(static関数)
		@param[in]	diameter 直径
		@param[in]	tessellation 分割数
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateSphere(float diameter, size_t tessellation,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTexフォーマットのカプセルを作成する(static関数)
		@param[in]	diameter 直径
		@param[in]	PointA 中間部線分の開始点（下）
		@param[in]	PointB 中間部線分の終了点（上）
		@param[in]	tessellation 分割数
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@param[in]	landscape 横にするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateCapsule(float diameter,
			const bsm::Float3& PointA, const bsm::Float3& PointB,
			size_t tessellation,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices,bool landscape = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTexフォーマットのシリンダーを作成する(static関数)
		@param[in]	height 高さ
		@param[in]	diameter 直径
		@param[in]	tessellation 分割数
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@param[in]	landscape 横にするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateCylinder(float height, float diameter, size_t tessellation,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices,
			 bool landscape = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTexフォーマットのコーンを作成する(static関数)
		@param[in]	diameter 直径
		@param[in]	height 高さ
		@param[in]	tessellation 分割数
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateCone(float diameter, float height, size_t tessellation,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTexフォーマットのトーラスを作成する(static関数)
		@param[in]	diameter ドーナッツ中心までの直径
		@param[in]	thickness ドーナッツ太さ
		@param[in]	tessellation 分割数
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateTorus(float diameter, float thickness, size_t tessellation,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTexフォーマットの正4面体を作成する(static関数)
		@param[in]	size サイズ
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateTetrahedron(float size,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTexフォーマットの正8面体を作成する(static関数)
		@param[in]	size サイズ
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateOctahedron(float size,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTexフォーマットの正12面体を作成する(static関数)
		@param[in]	size サイズ
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateDodecahedron(float size,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTexフォーマットの正20面体を作成する(static関数)
		@param[in]	size サイズ
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateIcosahedron(float size,
			vector<VertexPositionNormalTexture>& vertices, vector<uint32_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPositionNormalTangentTextureフォーマットのメッシュのタンジェントを追加する
		@param[inout]	vertices	頂点の配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void SetNormalTangent(vector<VertexPositionNormalTangentTexture>& vertices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPositionNormalTangentTextureSkinningフォーマットのメッシュのタンジェントを追加する
		@param[inout]	vertices	頂点の配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void SetNormalTangent(vector<VertexPositionNormalTangentTextureSkinning>& vertices);

		static void ConvertVertex(const vector<VertexPositionNormalTexture>& vertices,
			vector<VertexPositionColor>& new_pc_vertices,
			vector<VertexPositionNormal>& new_pn_vertices,
			vector<VertexPositionTexture>& new_pt_vertices,
			vector<VertexPositionNormalTangentTexture>& new_pntnt_vertices,
			vector<VertexPositionNormalTextureTangent>& new_pnttn_vertices
		) {
			new_pc_vertices.clear();
			new_pn_vertices.clear();
			new_pt_vertices.clear();
			new_pntnt_vertices.clear();
			new_pnttn_vertices.clear();
			for (size_t i = 0; i < vertices.size(); i++) {
				VertexPositionColor new_pc_v;
				VertexPositionNormal new_pn_v;
				VertexPositionTexture new_pt_v;
				VertexPositionNormalTangentTexture new_pntnt_v;
				VertexPositionNormalTextureTangent new_pnttn_v;

				new_pc_v.position = vertices[i].position;
				new_pc_v.color = bsm::Float4(1.0f, 1.0f, 1.0f, 1.0f);

				new_pn_v.position = vertices[i].position;
				new_pn_v.normal = vertices[i].normal;

				new_pt_v.position = vertices[i].position;
				new_pt_v.textureCoordinate = vertices[i].textureCoordinate;

				new_pntnt_v.position = vertices[i].position;
				new_pntnt_v.normal = vertices[i].normal;
				new_pntnt_v.textureCoordinate = vertices[i].textureCoordinate;
				bsm::Float3 n = bsm::cross((bsm::Float3)new_pntnt_v.normal, bsm::Float3(0, 1, 0));
				new_pntnt_v.tangent = bsm::Float4(n.x, n.y, n.z, 0.0f);
				new_pntnt_v.tangent.w = 0.0f;

				new_pnttn_v.position = vertices[i].position;
				new_pnttn_v.normal = vertices[i].normal;
				new_pnttn_v.textureCoordinate = vertices[i].textureCoordinate;
				bsm::Float3 n2 = bsm::cross((bsm::Float3)new_pnttn_v.normal, bsm::Float3(0, 1, 0));
				new_pnttn_v.tangent = bsm::Float4(n2.x, n2.y, n2.z, 0.0f);
				new_pnttn_v.tangent.w = 0.0f;


				new_pc_vertices.push_back(new_pc_v);
				new_pn_vertices.push_back(new_pn_v);
				new_pt_vertices.push_back(new_pt_v);
				new_pntnt_vertices.push_back(new_pntnt_v);
				new_pnttn_vertices.push_back(new_pnttn_v);

			}

		}


	};




}
//end basedx12