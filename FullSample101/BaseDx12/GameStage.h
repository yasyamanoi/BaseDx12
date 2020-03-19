#pragma once
#include "stdafx.h"

namespace basedx12 {

	class GameStage : public Stage {
	public:
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		virtual void OnInit()override;
	};

}
//end basedx12
