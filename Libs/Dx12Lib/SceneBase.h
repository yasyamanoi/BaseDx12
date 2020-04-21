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
		virtual void OnInitAssets() = 0;
		virtual void OnDrawPath(UINT index) {}
	};
}
// end basedx12

