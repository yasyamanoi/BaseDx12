#pragma once
#include "stdafx.h"

namespace basedx12 {

	class App;

	class SceneBase : public ObjectInterface {
	protected:
		SceneBase() {}
		virtual ~SceneBase() {}
	public:
		template<typename T, typename... Ts>
		shared_ptr<T> ResetActiveBaseDevice(Ts&&... params) {
			if (App::GetBaseDevice()) {
				App::GetBaseDevice()->OnDestroy();
			}
			shared_ptr<T> ptr = shared_ptr<T>(new T(params...));
			auto baseDevicePtr = dynamic_pointer_cast<BaseDevice>(ptr);
			if (!baseDevicePtr) {
				throw runtime_error("BaseDeviceに型キャストできません。");
			}
			App::SetBaseDevice(ptr);
			ptr->OnInit();
			return ptr;
		}
		void ConvertVertex(const vector<VertexPositionNormalTexture>& vertices,
			vector<VertexPositionColor>& new_pc_vertices
		);
		void ConvertVertex(const vector<VertexPositionNormalTexture>& vertices,
			vector<VertexPositionNormal>& new_pn_vertices
		);
		void ConvertVertex(const vector<VertexPositionNormalTexture>& vertices,
			vector<VertexPositionTexture>& new_pt_vertices
		);
		void ConvertVertex(const vector<VertexPositionNormalTexture>& vertices,
			vector<VertexPositionNormalTangentTexture>& new_pntnt_vertices
		);
		virtual void OnInitAssets() = 0;
		virtual void OnDrawPath(UINT index) {}
	};
}
// end basedx12

