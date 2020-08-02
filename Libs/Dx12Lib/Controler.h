#pragma once
#include "stdafx.h"

namespace basedx12 {

	//--------------------------------------------------------------------------------------
	/// XBoxコントローラーのステータス。XINPUT_STATE派生
	//--------------------------------------------------------------------------------------
	struct CONTROLER_STATE : public XINPUT_STATE
	{

		WORD wButtons;		///< ボタン状態
		BYTE bLeftTrigger;	///< 左トリガ
		BYTE bRightTrigger;	///< 右トリガ

		float fThumbRX;	///< 右スティックXのflort変換
		float fThumbRY;	///< 右スティックYのflort変換
		float fThumbLX;	///< 左スティックXのflort変換
		float fThumbLY;	///< 左スティックYのflort変換
		WORD wNowUpdateButtons;	///< 状態が変わったときにフラグが立つボタン
		WORD wPressedButtons;	///< 押された瞬間を所持するボタン
		WORD wReleasedButtons;	///< 離された瞬間を所持するボタン
		WORD wLastButtons;	///< 一つ前のボタン
		bool bConnected;	///< 接続しているかどうか
	};

	//--------------------------------------------------------------------------------------
	///	入力機器
	//--------------------------------------------------------------------------------------
	class Controler {
		static const DWORD MAX_CONTROLLERS = 4; // コントローラ数
		vector<CONTROLER_STATE> m_state; // コントローラステートの配列
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Controler() :
			m_state(MAX_CONTROLLERS)
		{
			for (DWORD i = 0; i < MAX_CONTROLLERS; i++) {
				::ZeroMemory(&m_state[i], sizeof(CONTROLER_STATE));
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		~Controler() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief コントローラーの状態をm_Stateに取得する
		*/
		//--------------------------------------------------------------------------------------
		void ResetControlerState() {
			DWORD dwResult;
			for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
			{
				//一つ前の状態を保存
				m_state[i].wLastButtons = m_state[i].Gamepad.wButtons;
				// XInputから現在のステータスを得る
				XINPUT_STATE state;
				dwResult = XInputGetState(i, &state);
				if (dwResult == ERROR_SUCCESS) {
					m_state[i].dwPacketNumber = state.dwPacketNumber;
					m_state[i].Gamepad = state.Gamepad;
					m_state[i].bConnected = true;
					//ボタンの設定
					//コピー
					m_state[i].wButtons = m_state[i].Gamepad.wButtons;
					//更新ボタン
					m_state[i].wNowUpdateButtons = m_state[i].Gamepad.wButtons;
					m_state[i].bLeftTrigger = m_state[i].Gamepad.bLeftTrigger;
					m_state[i].bRightTrigger = m_state[i].Gamepad.bRightTrigger;
					//前回より変化しているボタンのみフラグを立てる
					//XOR演算により実装
					m_state[i].wNowUpdateButtons ^= m_state[i].wLastButtons;
					//押された瞬間をセット
					m_state[i].wPressedButtons = m_state[i].wNowUpdateButtons & m_state[i].wButtons;
					//離された瞬間をセット
					m_state[i].wReleasedButtons = m_state[i].wNowUpdateButtons & (m_state[i].wButtons ^ 0xffff);
					//レバーのデッドゾーンの設定
					if ((m_state[i].Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
						m_state[i].Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
						(m_state[i].Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
							m_state[i].Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
					{
						m_state[i].Gamepad.sThumbLX = 0;
						m_state[i].Gamepad.sThumbLY = 0;
					}
					if (m_state[i].Gamepad.sThumbLX >= 0) {
						m_state[i].fThumbLX = (float)m_state[i].Gamepad.sThumbLX / (float)SHRT_MAX;
					}
					else {
						m_state[i].fThumbLX = (float)m_state[i].Gamepad.sThumbLX / (float)(SHRT_MAX + 1);
					}
					if (m_state[i].Gamepad.sThumbLY >= 0) {
						m_state[i].fThumbLY = (float)m_state[i].Gamepad.sThumbLY / (float)SHRT_MAX;
					}
					else {
						m_state[i].fThumbLY = (float)m_state[i].Gamepad.sThumbLY / (float)(SHRT_MAX + 1);
					}

					if ((m_state[i].Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
						m_state[i].Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
						(m_state[i].Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
							m_state[i].Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
					{
						m_state[i].Gamepad.sThumbRX = 0;
						m_state[i].Gamepad.sThumbRY = 0;
					}
					if (m_state[i].Gamepad.sThumbRX >= 0) {
						m_state[i].fThumbRX = (float)m_state[i].Gamepad.sThumbRX / (float)SHRT_MAX;
					}
					else {
						m_state[i].fThumbRX = (float)m_state[i].Gamepad.sThumbRX / (float)(SHRT_MAX + 1);
					}
					if (m_state[i].Gamepad.sThumbRY >= 0) {
						m_state[i].fThumbRY = (float)m_state[i].Gamepad.sThumbRY / (float)SHRT_MAX;
					}
					else {
						m_state[i].fThumbRY = (float)m_state[i].Gamepad.sThumbRY / (float)(SHRT_MAX + 1);
					}
				}
				else {
					m_state[i].bConnected = false;
				}
			}
		}
		//	アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief コントローラステートの取得
		@return	コントローラステート
		*/
		//--------------------------------------------------------------------------------------
		const vector<CONTROLER_STATE>& GetControlers()const { return m_state; }
	};


}
//end basedx12

