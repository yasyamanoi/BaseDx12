#pragma once
#include "stdafx.h"

namespace basedx12 {

	class CellSquare : public BaseSquare {
	public:
		CellSquare() :
			BaseSquare()
		{}
		virtual ~CellSquare() {}
		virtual void OnInit() override;
	};

}
//end  basedx12
