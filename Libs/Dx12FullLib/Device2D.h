#pragma once
#include "stdafx.h"

namespace basedx12 {

	//--------------------------------------------------------------------------------------
	///	一般的な2Dデバイス
	//--------------------------------------------------------------------------------------
	class Default2DDivece : public BaseDevice {
		void LoadPipeline();
		void LoadAssets();
		void PopulateCommandList();
	public:
		Default2DDivece(UINT frameCount);
		virtual ~Default2DDivece() {}
		virtual void OnInit()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override;
		virtual void OnDestroy()override;
	};


}
// end basedx12
