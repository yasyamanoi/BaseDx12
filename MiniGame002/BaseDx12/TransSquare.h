#pragma once
#include "stdafx.h"

namespace basedx12 {

	class TransSquare : public BaseSquare {
	public:
		TransSquare(const MapData& data) :
			BaseSquare(data)
		{}
		virtual ~TransSquare() {}
		virtual void OnInit() override;
	};

}
//end basedx12
