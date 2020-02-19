#pragma once
#include "GameDevice.h"

namespace basedx12 {



	class Scene :public SceneBase {
	public:
		Scene() :SceneBase() {}
		virtual ~Scene() {}
		virtual void OnInit();
		virtual void OnUpdate();
		virtual void OnRender();
		virtual void OnDestroy();
	};

}
//end basedx12


