#pragma once
#include "GameDevice.h"

namespace basedx12 {

	class Scene :public SceneBase {
		shared_ptr<GameDivece> m_device;
	public:
		Scene() :SceneBase(), m_device() {}
		virtual ~Scene() {}
		virtual void OnInit();
		virtual void OnUpdate();
		virtual void OnRender();
		virtual void OnDestroy();
	};

}
//end basedx12


