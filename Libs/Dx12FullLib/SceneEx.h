#pragma once
#include "stdafx.h"

namespace basedx12 {


	class Stage;

	//--------------------------------------------------------------------------------------
	///	フルバージョンにおけるシーンの親クラス
	//--------------------------------------------------------------------------------------
	class SceneEx : public SceneBase {
		shared_ptr<Stage> m_activeStage;
	protected:
		SceneEx() {}
		virtual ~SceneEx() {}
		shared_ptr<Stage> GetActiveStage(bool ExceptionActive = true) const {
			if (!m_activeStage) {
				//アクティブなステージが無効なら
				if (ExceptionActive) {
					throw BaseException(
						L"アクティブなステージがありません",
						L"if(!m_activeStage)",
						L"SceneEx::GetActiveStage()"
					);
				}
				else {
					return nullptr;
				}
			}
			return m_activeStage;

		}
		void SetActiveStage(const shared_ptr<Stage>& stage) {
			m_activeStage = stage;
		}
		//アクティブステージの設定
		template<typename T, typename... Ts>
		shared_ptr<T> ResetActiveStage(Ts&&... params) {
			auto ActStagePtr = GetActiveStage(false);
			if (ActStagePtr) {
				//破棄を伝える
				ActStagePtr->OnDestroy();
			}
			auto Ptr = ObjectFactory::Create<T>(params...);
			auto StagePtr = dynamic_pointer_cast<Stage>(Ptr);
			if (!StagePtr) {
				throw BaseException(
					L"以下はStageに型キャストできません。",
					Util::GetWSTypeName<T>(),
					L"SceneEx::ResetActiveStage<T>()"
				);
			}
			SetActiveStage(StagePtr);
			return Ptr;
		}
	public:
		virtual void OnUpdate()override;
		virtual void OnDraw()override;
		virtual void OnDestroy()override;

	};


}
// end basedx12
