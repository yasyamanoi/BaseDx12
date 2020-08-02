#pragma once
#include "stdafx.h"

namespace basedx12 {


	class Stage;

	//--------------------------------------------------------------------------------------
	///	�t���o�[�W�����ɂ�����V�[���̐e�N���X
	//--------------------------------------------------------------------------------------
	class SceneEx : public SceneBase {
		shared_ptr<Stage> m_activeStage;
	protected:
		SceneEx() {}
		virtual ~SceneEx() {}
		shared_ptr<Stage> GetActiveStage(bool ExceptionActive = true) const {
			if (!m_activeStage) {
				//�A�N�e�B�u�ȃX�e�[�W�������Ȃ�
				if (ExceptionActive) {
					throw BaseException(
						L"�A�N�e�B�u�ȃX�e�[�W������܂���",
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
		//�A�N�e�B�u�X�e�[�W�̐ݒ�
		template<typename T, typename... Ts>
		shared_ptr<T> ResetActiveStage(Ts&&... params) {
			auto ActStagePtr = GetActiveStage(false);
			if (ActStagePtr) {
				//�j����`����
				ActStagePtr->OnDestroy();
			}
			auto Ptr = ObjectFactory::Create<T>(params...);
			auto StagePtr = dynamic_pointer_cast<Stage>(Ptr);
			if (!StagePtr) {
				throw BaseException(
					L"�ȉ���Stage�Ɍ^�L���X�g�ł��܂���B",
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
