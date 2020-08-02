#include "stdafx.h"

namespace basedx12 {

	void SceneBase::ConvertVertex(const vector<VertexPositionNormalTexture>& vertices,
		vector<VertexPositionColor>& new_pc_vertices
	)
	{
		new_pc_vertices.clear();
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionColor new_pc_v;
			new_pc_v.position = vertices[i].position;
			new_pc_v.color = Float4(1.0f, 1.0f, 1.0f, 1.0f);
			new_pc_vertices.push_back(new_pc_v);
		}
	}

	void SceneBase::ConvertVertex(const vector<VertexPositionNormalTexture>& vertices,
		vector<VertexPositionNormal>& new_pn_vertices
	)
	{
		new_pn_vertices.clear();
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionNormal new_pn_v;
			new_pn_v.position = vertices[i].position;
			new_pn_v.normal = vertices[i].normal;
			new_pn_vertices.push_back(new_pn_v);
		}
	}
	void SceneBase::ConvertVertex(const vector<VertexPositionNormalTexture>& vertices,
		vector<VertexPositionTexture>& new_pt_vertices
	) {
		new_pt_vertices.clear();
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionTexture new_pt_v;
			new_pt_v.position = vertices[i].position;
			new_pt_v.textureCoordinate = vertices[i].textureCoordinate;
			new_pt_vertices.push_back(new_pt_v);
		}

	}
	void SceneBase::ConvertVertex(const vector<VertexPositionNormalTexture>& vertices,
		vector<VertexPositionNormalTangentTexture>& new_pntnt_vertices
	) {
		new_pntnt_vertices.clear();
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionNormalTangentTexture new_pntnt_v;
			new_pntnt_v.position = vertices[i].position;
			new_pntnt_v.normal = vertices[i].normal;
			new_pntnt_v.textureCoordinate = vertices[i].textureCoordinate;
			Float3 n = bsm::cross((Float3)new_pntnt_v.normal, Float3(0, 1, 0));
			new_pntnt_v.tangent = Float4(n.x, n.y, n.z, 0.0f);
			new_pntnt_v.tangent.w = 0.0f;
			new_pntnt_vertices.push_back(new_pntnt_v);
		}
	}

}
//end basedx12
