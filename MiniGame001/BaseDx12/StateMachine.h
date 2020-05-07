#pragma once
#include "stdafx.h"

namespace basedx12 {

	template <typename T>
	class StateMachine;


	//	�X�e�[�g�����e���v���[�g�N���X(���ۃN���X)
	template <typename T>
	class ObjState {
	public:
		ObjState() {}
		virtual ~ObjState() {}
		virtual void Enter(T* owner) = 0;
		virtual void Execute(T* owner) = 0;
		virtual void Exit(T* owner) = 0;
	};

	//	�X�e�[�g�}�V�������e���v���[�g�N���X(���ۃN���X)
	template <typename T>
	class StateMachine
	{
	private:
		//���̃X�e�[�g�}�V�������I�[�i�[
		T* m_owner;
		//���݂̃X�e�[�g
		weak_ptr< ObjState<T> > m_currentState;
		//��O�̃X�e�[�g
		weak_ptr< ObjState<T> > m_previousState;
	public:
		explicit StateMachine(T* owner) :
			m_owner(owner)
		{}
		virtual ~StateMachine() {}
		void SetCurrentState(const shared_ptr< ObjState<T> >& s) { m_currentState = s; }
		void SetPreviousState(const shared_ptr< ObjState<T> >& s) { m_previousState = s; }
		shared_ptr< ObjState<T> >  GetCurrentState() const {
			auto shptr = m_currentState.lock();
			if (shptr) {
				return shptr;
			}
			return nullptr;
		}
		shared_ptr< ObjState<T> >  GetPreviousState()const {
			auto shptr = m_previousState.lock();
			if (shptr) {
				return shptr;
			}
			return nullptr;
		}
		void Update() const {
			auto shptr = m_currentState.lock();
			if (shptr && m_owner) {
				shptr->Execute(m_owner);
			}
		}
		void  ChangeState(const shared_ptr< ObjState<T> >& newState) {
			//���̃X�e�[�g��ۑ�
			m_previousState = m_currentState;
			auto shptr = m_currentState.lock();
			if (shptr && m_owner) {
				//���̃X�e�[�g�ɏI����m�点��
				shptr->Exit(m_owner);
			}
			//�V�����X�e�[�g���J�����g�ɐݒ�
			m_currentState = newState;
			shptr = m_currentState.lock();
			if (shptr && m_owner) {
				//���̃X�e�[�g�ɏI����m�点��
				shptr->Enter(m_owner);
			}
		}
		void RevertToPreviousState() {
			ChangeState(m_previousState);
		}
		bool IsInState(const shared_ptr< ObjState<T> >& st)const {
			if (st == GetCurrentState()) {
				return true;
			}
			return false;
		}
	};



}
//end basedx12
