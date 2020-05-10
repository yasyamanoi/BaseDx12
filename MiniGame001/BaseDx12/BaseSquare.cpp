#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

	IMPLEMENT_DX12SHADER(VSPTSprite, App::GetShadersPath() + L"VSPTSprite.cso")
	IMPLEMENT_DX12SHADER(PSPTSprite, App::GetShadersPath() + L"PSPTSprite.cso")



	shared_ptr<BaseMesh> BaseSquare::CreateSquareMesh(float u, float v) {
		float helfSize = 0.5f;
		//頂点配列
		vector<VertexPositionTexture> vertices = {
			{ VertexPositionTexture(Float3(-helfSize, helfSize, 0),	Float2(0.0f, 0.0f)) },
			{ VertexPositionTexture(Float3(helfSize, helfSize, 0),	Float2(u, 0.0f)) },
			{ VertexPositionTexture(Float3(-helfSize, -helfSize, 0),	Float2(0.0f, v)) },
			{ VertexPositionTexture(Float3(helfSize, -helfSize, 0),	Float2(u, v)) },
		};
		//インデックス配列
		vector<uint32_t> indices = { 0, 1, 2, 1, 3, 2 };
		//四角形メッシュの作成
		return BaseMesh::CreateBaseMesh<VertexPositionTexture>(vertices, indices);
	}

	shared_ptr<BaseTexture> BaseSquare::CreateTextureFromFile(const wstring& file, UINT& index) {
		auto baseDevice = App::GetBaseDevice();
		//テクスチャの作成
		//シェーダリソースハンドルを作成
		index = baseDevice->GetCbvSrvUavNextIndex();
		CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
			index,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		//画像ファイルをもとにテクスチャを作成
		return BaseTexture::CreateBaseTexture(file, srvHandle);
	}

	shared_ptr<ConstantBuffer> BaseSquare::CreateConstantBuffer(const SpriteConstantBuffer& data, UINT& index) {
		auto baseDevice = App::GetBaseDevice();
		//コンスタントバッファハンドルを作成
		index = baseDevice->GetCbvSrvUavNextIndex();
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
			index,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		return ConstantBuffer::CreateDirect(handle, data);
	}

	void BaseSquare::SetInitDrawData() {
		auto scene = App::GetTypedSceneBase<Scene>();
		m_drawData.m_scale = m_initData.m_scale * (float)scene->GetCellSize();
		m_drawData.m_scale.z = 1.0f;
		m_drawData.m_pivot = m_initData.m_pivot * (float)scene->GetCellSize();
		m_drawData.m_pivot.z = 0.0f;
		m_drawData.m_qt = m_initData.m_qt;
		m_drawData.m_pos = m_initData.m_pos * (float)scene->GetCellSize();
		m_drawData.m_pos.z = m_initData.m_pos.z;
		m_drawData.m_pos.y += scene->GetLowSpan();
		//射影行列は固定
		float w = static_cast<float>(App::GetGameWidth());
		float h = static_cast<float>(App::GetGameHeight());
		Mat4x4 proj;
		m_drawData.m_proj.orthographicLH(w, h, -1.0, 1.0f);
		//初期化時は強制的にコンスタントバッファ更新
		m_drawData.m_dirtyflag = true;
	}


	bool BaseSquare::UpdateDrawMatrix() {
		if (m_drawData.m_dirtyflag) {
			m_drawData.m_world.affineTransformation(
				m_drawData.m_scale,
				m_drawData.m_pivot,
				m_drawData.m_qt,
				m_drawData.m_pos
			);
			m_drawData.m_dirtyflag = false;
			return true;
		}
		return false;
	}

	OBB BaseSquare::GetOBB()const {
		Mat4x4 world = GetWorldMatrix();
		OBB obb(Float3(1.0), world);
		return obb;
	}

	OBB BaseSquare::GetBeforeOBB() const {
		Mat4x4 world = GetBeforeWorldMatrix();
		OBB obb(Float3(1.0), world);
		return obb;

	}



	void BaseSquare::SetToBefore() {
		m_drawData.m_beforeScale = m_drawData.m_scale;
		m_drawData.m_beforePivot = m_drawData.m_pivot;
		m_drawData.m_beforeQt = m_drawData.m_qt;
		m_drawData.m_beforePos = m_drawData.m_pos;
	}

	void BaseSquare::SetDirtyflag(bool b) {
		m_drawData.m_dirtyflag = b;
	}


	Mat4x4 BaseSquare::GetWorldMatrix()const {
		Mat4x4 world;
		world.affineTransformation(
			m_drawData.m_scale,
			m_drawData.m_pivot,
			m_drawData.m_qt,
			m_drawData.m_pos
		);
		return world;
	}

	Mat4x4 BaseSquare::GetBeforeWorldMatrix()const {
		Mat4x4 world;
		world.affineTransformation(
			m_drawData.m_beforeScale,
			m_drawData.m_pivot,
			m_drawData.m_beforeQt,
			m_drawData.m_beforePos
		);
		return world;
	}

	Float3 BaseSquare::GetWorldPosition() const {
		return m_drawData.m_pos;
	}
	void BaseSquare::SetWorldPosition(const Float3& pos) {
		m_drawData.m_pos = pos;
		m_drawData.m_dirtyflag = true;
	}

	Float3 BaseSquare::GetWorldVelocity() const {
		return m_drawData.m_velocity;
	}
	void BaseSquare::SetWorldVelocity(const Float3& velo) {
		m_drawData.m_velocity = velo;
	}






}
// end basedx12
