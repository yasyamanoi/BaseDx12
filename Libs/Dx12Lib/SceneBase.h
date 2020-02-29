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
		shared_ptr<T> ResetActiveDx12Device(Ts&&... params) {
			if (App::GetDx12Device()) {
				App::GetDx12Device()->OnDestroy();
			}
			shared_ptr<T> Ptr = shared_ptr<T>(new T(params...));
			auto Dx12DevicePtr = dynamic_pointer_cast<Dx12Device>(Ptr);
			if (!Dx12DevicePtr) {
				throw runtime_error("Dx12Deviceに型キャストできません。");
			}
			App::SetDx12Device(Ptr);
			Ptr->OnInit();
			return Ptr;
		}
		virtual void OnInitAssets() = 0;

	};
}
// end basedx12

