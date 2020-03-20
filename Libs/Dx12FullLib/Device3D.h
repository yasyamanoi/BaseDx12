#pragma once
#include "stdafx.h"

namespace basedx12 {

	//--------------------------------------------------------------------------------------
	///	��ʓI��3D�f�o�C�X
	//--------------------------------------------------------------------------------------
	class Default3DDivece : public BaseDevice {
		void LoadPipeline();
		void LoadAssets();
		void PopulateCommandList();
	public:
		Default3DDivece(UINT frameCount);
		virtual ~Default3DDivece() {}
		virtual void OnInit()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override;
		virtual void OnDestroy()override;
	};

}
//end basedx12
