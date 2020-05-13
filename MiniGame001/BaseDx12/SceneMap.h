#include "stdafx.h"

namespace basedx12 {


	vector<MapData> g_StageData1
	{
		{ CharaType::CellSquare, Float3(41.0,23.0,1.0), Float3(0.0), Quat(), Float3(0.0) },

		{ CharaType::Player, Float3(1.0f,3.0,1.0), Float3(0.0), Quat(), Float3(0.0,-8.0f,0.0) },

		{ CharaType::WallSquare, Float3(41.0,1.0,1.0), Float3(0.0), Quat(), Float3(0.0f,-11.0f,0.0f) },
		{ CharaType::WallSquare, Float3(7.0,9.0,1.0), Float3(0.0), Quat(), Float3(-20.0f,-6.0f,0.0f) },
		{ CharaType::WallSquare, Float3(7.0,3.0,1.0), Float3(0.0), Quat(), Float3(20.0f,-3.0f,0.0f) },
		{ CharaType::WallSquare, Float3(7.0,3.0,1.0), Float3(0.0), Quat(), Float3(17.0f,-6.0f,0.0f) },
		{ CharaType::WallSquare, Float3(11.0,3.0,1.0), Float3(0.0), Quat(), Float3(15.0f,-9.0f,0.0f) },
		{ CharaType::WallSquare, Float3(3.0,3.0,1.0), Float3(0.0), Quat(), Float3(0.0f,3.0f,0.0f) },
		{ CharaType::WallSquare, Float3(3.0,3.0,1.0), Float3(0.0), Quat(), Float3(-15.0f,-9.0f,0.0f) },
		{ CharaType::MoveSquare, Float3(5.0,1.0,1.0), Float3(0.0), Quat(), Float3(8.0f,1.0f,0.0f) },
		{ CharaType::ItemSquare, Float3(1.0,1.0,1.0), Float3(0.0), Quat(), Float3(0.0f,7.0f,0.0f) },

		{ CharaType::WallSquare, Float3(3.0,3.0,1.0), Float3(0.0), Quat(), Float3(5.0f,-9.0f,0.0f) },
	};

	vector<vector<MapData>> g_Stages;

	

}
//end basedx12
