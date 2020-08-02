#pragma once
#include "stdafx.h"

namespace basedx12 {

	class WallSquare : public BaseSquare {
	public:
		WallSquare(const MapData& data) :
			BaseSquare(data)
		{}
		virtual ~WallSquare() {}
		virtual void OnInit() override;
	};

}
//end basedx12
