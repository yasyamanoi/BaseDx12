#pragma once
#include "stdafx.h"

namespace basedx12 {

	DECLARE_DX12SHADER(VSPTSprite)
	DECLARE_DX12SHADER(PSPTSprite)



	//2Dコンスタントバッファ
	struct SpriteConstantBuffer
	{
		Mat4x4 worldProj;
		Float4 emissive;
		Float4 diffuse;
		SpriteConstantBuffer() :
			worldProj(),
			emissive(0.0f),
			diffuse(1.0f)
		{
		};
	};

	enum class CharaType {
		Undefined,
		CellSquare,
		Player,
		TransSquare,
		WallSquare,
		MoveSquare,
		ItemSquare,
		Enemy1
	};

	struct MapData {
		CharaType m_charaType;
		Float3 m_scale;
		Float3 m_pivot;
		Quat m_qt;
		Float3 m_pos;
		MapData() :
			m_charaType(CharaType::Undefined),
			m_scale(1.0),
			m_pivot(0.0),
			m_qt(),
			m_pos(0.0)
		{}
		MapData(
			CharaType charaType,
			Float3& scale,
			Float3& pivot,
			Quat& qt,
			Float3& pos
		) :
			m_charaType(charaType),
			m_scale(scale),
			m_pivot(pivot),
			m_qt(qt),
			m_pos(pos)
		{}
	};

	struct DrawData {
		Float3 m_beforeScale;
		Float3 m_beforePivot;
		Quat m_beforeQt;
		Float3 m_beforePos;
		Float3 m_scale;
		Float3 m_pivot;
		Quat m_qt;
		Float3 m_pos;
		Mat4x4 m_world;
		Mat4x4 m_proj;
		bool m_dirtyflag;
		//速度
		Float3 m_velocity;
		//色関連
		Float4 m_diffuse;
		Float4 m_emissive;
		DrawData() :
			m_scale(1.0f),
			m_pivot(0.0f),
			m_qt(),
			m_pos(0.0f),
			m_world(),
			m_proj(),
			m_dirtyflag(true),
			m_velocity(0.0f),
			m_diffuse(1.0f),
			m_emissive(0.0f)
		{
			m_beforeScale = m_scale;
			m_beforePivot = m_pivot;
			m_beforeQt = m_qt;
			m_beforePos = m_pos;
		}
	};

	//--------------------------------------------------------------------------------------
	///	コントローラのボタンのハンドラ
	//--------------------------------------------------------------------------------------
	template<typename T>
	struct InputHandler {
		void PushHandle(T* obj) {
			//コントローラの取得
			auto cntlStats = App::GetControlers();
			if (cntlStats[0].bConnected) {
				//Aボタン
				if (cntlStats[0].wPressedButtons & XINPUT_GAMEPAD_A) {
					obj->OnPushA();
				}
			}
		}
	};



}
//end basedx12
