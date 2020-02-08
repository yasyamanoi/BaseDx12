#pragma once

class SceneBase {
	shared_ptr<Stage> m_ActiveStage;
protected:
	SceneBase() :m_ActiveStage(nullptr) {}
	virtual ~SceneBase() {}
public:
	template<typename T, typename... Ts>
	shared_ptr<T> ResetActiveStage(Ts&&... params) {
		if (m_ActiveStage) {
			m_ActiveStage->OnDestroy();
			m_ActiveStage.reset();
			m_ActiveStage = nullptr;
		}
		shared_ptr<T> Ptr = shared_ptr<T>(new T(params...));
		auto StagePtr = dynamic_pointer_cast<Stage>(Ptr);
		if (!StagePtr) {
			throw runtime_error("Stageに型キャストできません。");
		}
		m_ActiveStage = Ptr;
		m_ActiveStage->OnInit();
		return Ptr;
	}
	template<typename T>
	shared_ptr<T> GetActiveStage() {
		if (m_ActiveStage) {
			auto TgtPtr = dynamic_pointer_cast<T>(m_ActiveStage);
			if (!TgtPtr) {
				throw runtime_error("Stageに型キャストできません。");
			}
			return TgtPtr;
		}
		else {
			throw runtime_error("まだStageが設定されていません");
		}
	}
	virtual void OnInit() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
	virtual void OnDestroy() = 0;
	virtual void OnKeyDown(UINT8 key) {}
	virtual void OnKeyUp(UINT8 key) {}
};
