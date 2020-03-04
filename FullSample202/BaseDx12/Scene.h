#pragma once
#include "stdafx.h"

namespace basedx12{

	class Scene :public SceneEx {
	public:
		Scene() :SceneEx() {}
		virtual ~Scene() {}
		virtual void OnInit()override;
		virtual void OnInitAssets()override;
	};

}
//end basedx12


