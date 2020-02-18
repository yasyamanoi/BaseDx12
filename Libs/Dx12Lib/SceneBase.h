#pragma once

namespace basedx12 {

	class App;

	class SceneBase {
	protected:
		SceneBase() {}
		virtual ~SceneBase() {}
		template<typename T>
		void Createdx12Device() {
			auto device = shared_ptr<T>(new T());
		}
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


		virtual void OnInit() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
		virtual void OnDestroy() = 0;
		virtual void OnKeyDown(UINT8 key) {}
		virtual void OnKeyUp(UINT8 key) {}
	};
}
// end basedx12

