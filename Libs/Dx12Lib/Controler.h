#pragma once
#include "stdafx.h"

namespace basedx12 {

	//--------------------------------------------------------------------------------------
	/// XBox�R���g���[���[�̃X�e�[�^�X�BXINPUT_STATE�h��
	//--------------------------------------------------------------------------------------
	struct CONTROLER_STATE : public XINPUT_STATE
	{

		WORD wButtons;		///< �{�^�����
		BYTE bLeftTrigger;	///< ���g���K
		BYTE bRightTrigger;	///< �E�g���K

		float fThumbRX;	///< �E�X�e�B�b�NX��flort�ϊ�
		float fThumbRY;	///< �E�X�e�B�b�NY��flort�ϊ�
		float fThumbLX;	///< ���X�e�B�b�NX��flort�ϊ�
		float fThumbLY;	///< ���X�e�B�b�NY��flort�ϊ�
		WORD wNowUpdateButtons;	///< ��Ԃ��ς�����Ƃ��Ƀt���O�����{�^��
		WORD wPressedButtons;	///< �����ꂽ�u�Ԃ���������{�^��
		WORD wReleasedButtons;	///< �����ꂽ�u�Ԃ���������{�^��
		WORD wLastButtons;	///< ��O�̃{�^��
		bool bConnected;	///< �ڑ����Ă��邩�ǂ���
	};

	//--------------------------------------------------------------------------------------
	///	���͋@��
	//--------------------------------------------------------------------------------------
	class Controler {
		static const DWORD MAX_CONTROLLERS = 4; // �R���g���[����
		vector<CONTROLER_STATE> m_state; // �R���g���[���X�e�[�g�̔z��
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
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
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		~Controler() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���g���[���[�̏�Ԃ�m_State�Ɏ擾����
		*/
		//--------------------------------------------------------------------------------------
		void ResetControlerState() {
			DWORD dwResult;
			for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
			{
				//��O�̏�Ԃ�ۑ�
				m_state[i].wLastButtons = m_state[i].Gamepad.wButtons;
				// XInput���猻�݂̃X�e�[�^�X�𓾂�
				XINPUT_STATE state;
				dwResult = XInputGetState(i, &state);
				if (dwResult == ERROR_SUCCESS) {
					m_state[i].dwPacketNumber = state.dwPacketNumber;
					m_state[i].Gamepad = state.Gamepad;
					m_state[i].bConnected = true;
					//�{�^���̐ݒ�
					//�R�s�[
					m_state[i].wButtons = m_state[i].Gamepad.wButtons;
					//�X�V�{�^��
					m_state[i].wNowUpdateButtons = m_state[i].Gamepad.wButtons;
					m_state[i].bLeftTrigger = m_state[i].Gamepad.bLeftTrigger;
					m_state[i].bRightTrigger = m_state[i].Gamepad.bRightTrigger;
					//�O����ω����Ă���{�^���̂݃t���O�𗧂Ă�
					//XOR���Z�ɂ�����
					m_state[i].wNowUpdateButtons ^= m_state[i].wLastButtons;
					//�����ꂽ�u�Ԃ��Z�b�g
					m_state[i].wPressedButtons = m_state[i].wNowUpdateButtons & m_state[i].wButtons;
					//�����ꂽ�u�Ԃ��Z�b�g
					m_state[i].wReleasedButtons = m_state[i].wNowUpdateButtons & (m_state[i].wButtons ^ 0xffff);
					//���o�[�̃f�b�h�]�[���̐ݒ�
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
		//	�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���g���[���X�e�[�g�̎擾
		@return	�R���g���[���X�e�[�g
		*/
		//--------------------------------------------------------------------------------------
		const vector<CONTROLER_STATE>& GetControlers()const { return m_state; }
	};


}
//end basedx12

