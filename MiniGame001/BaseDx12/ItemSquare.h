#pragma once
#include "stdafx.h"

namespace basedx12 {

	class ItemSquare : public BaseSquare {
	public:
		ItemSquare(const MapData& data) :
			BaseSquare(data)
		{}
		virtual ~ItemSquare() {}
		virtual void OnCollisionEnter(BaseSquare* other) override;
		virtual void OnInit() override;
	};

}
//end basedx12
