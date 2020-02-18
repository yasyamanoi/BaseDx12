#pragma once
#include "GameDevice.h"

namespace basedx12 {

	class Scene :public SceneBase {
		shared_ptr<GameDivece> m_device;
	public:
		Scene() :SceneBase() {}
		virtual ~Scene() {}
		virtual void OnInit();
		virtual void OnUpdate();
		virtual void OnRender();
		virtual void OnDestroy();
		virtual void OnKeyDown(UINT8 key);
		virtual void OnKeyUp(UINT8 key);
	};
}
//end basedx12


