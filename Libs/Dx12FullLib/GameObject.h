#pragma once
#include "stdafx.h"

namespace basedx12 {

	class Stage;

	//--------------------------------------------------------------------------------------
	///	�Q�[���I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class GameObject : public ObjectInterface {
		bool m_UpdateActive = true;	//update���邩�ǂ���
		bool m_DrawActive = true;		//Draw���邩�ǂ���
		bool m_AlphaActive = false;		//�������ǂ���

		int m_DrawLayer = 0;	//�`�惌�C���[
		set<wstring> m_TagSet;	//�^�O�̃Z�b�g
		set<int> m_NumTagSet;	//�����^�O�̃Z�b�g

		weak_ptr<Stage> m_Stage;	//�����X�e�[�W
		map<type_index, shared_ptr<Component> > m_CompMap;
		list<type_index> m_CompOrder;	//�R���|�[�l���g���s����
											//�s���̃}�b�v
		map<type_index, shared_ptr<Behavior>> m_BehaviorMap;

		shared_ptr<Component> SearchStrictComponent(type_index TypeIndex)const {
			auto it = m_CompMap.find(TypeIndex);
			if (it != m_CompMap.end()) {
				return it->second;
			}
			return nullptr;
		}

		template<typename T>
		shared_ptr<T> SearchDynamicComponent()const {
			auto it = m_CompMap.begin();
			while (it != m_CompMap.end()) {
				auto Ptr = dynamic_pointer_cast<T>(it->second);
				if (Ptr) {
					return Ptr;
				}
				it++;
			}
			return nullptr;
		}

		const map<type_index, shared_ptr<Behavior> >& GetBehaviorMap() const {
			return m_BehaviorMap;
		}
		map<type_index, shared_ptr<Behavior> >& GetBehaviorMap() {
			return m_BehaviorMap;
		}
		shared_ptr<Behavior> SearchBehavior(type_index TypeIndex)const {
			auto it = m_BehaviorMap.find(TypeIndex);
			if (it != m_BehaviorMap.end()) {
				return it->second;
			}
			return nullptr;

		}
		void AddMakedBehavior(type_index TypeIndex, const shared_ptr<Behavior>& Ptr) {
			//map�ɒǉ��������͍X�V
			m_BehaviorMap[TypeIndex] = Ptr;
		}
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		explicit GameObject(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~GameObject();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�V�������L�����ǂ����𓾂�
		@return	�X�V�������L���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsUpdateActive() const {
			return m_UpdateActive;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�V�������L�����ǂ�����ݒ肷��
		@param[in]	b	�X�V�������L���Ȃ�Ȃ�true
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetUpdateActive(bool b) {
			m_UpdateActive = b;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`�揈�����L�����ǂ����𓾂�
		@return	�`�揈�����L���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsDrawActive() const {
			return m_DrawActive;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`�揈�����L�����ǂ�����ݒ肷��
		@param[in]	b	�`�揈�����L���Ȃ�Ȃ�true
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetDrawActive(bool b) {
			m_DrawActive = b;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���������i�������܂ށj���L�����ǂ����𓾂�
		@return	���������i�������܂ށj���L���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsAlphaActive() const {
			return m_AlphaActive;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���������i�������܂ށj���L�����ǂ�����ݒ肷��
		@param[in]	b	���������i�������܂ށj���L���Ȃ�Ȃ�true
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetAlphaActive(bool b) {
			m_AlphaActive = b;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`�惌�C���[�𓾂�i�f�t�H���g0�j
		@return	�`�惌�C���[
		*/
		//--------------------------------------------------------------------------------------
		int GetDrawLayer() const {
			return m_DrawLayer;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`�惌�C���[��ݒ肷��
		@param[in]	l	�`�惌�C���[�i�}�C�i�X�ݒ�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void  SetDrawLayer(int l) {
			m_DrawLayer = l;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�^�O�̃Z�b�g�𓾂�(const)
		@return	�^�O�̃Z�b�g
		*/
		//--------------------------------------------------------------------------------------
		const set<wstring>& GetTagSet() const {
			return m_TagSet;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w�肷��^�O�����݂��邩�ǂ����𓾂�
		@param[in]	tagstr	���؂���^�O
		@return	���݂����true
		*/
		//--------------------------------------------------------------------------------------
		bool FindTag(const wstring& tagstr) const {
			if (m_TagSet.find(tagstr) == m_TagSet.end()) {
				return false;
			}
			return true;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w�肷��^�O��ǉ�����
		@param[in]	tagstr	�ǉ�����^�O
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void  AddTag(const wstring& tagstr) {
			if (tagstr == L"") {
				//�󔒂Ȃ��O
				throw BaseException(
					L"�ݒ肷��^�O����ł�",
					L"if (tagstr == L"")",
					L"GameObject::AddTag()"
				);
			}
			m_TagSet.insert(tagstr);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w�肷��^�O�����݂�����폜����i���݂��Ȃ��ꍇ�͉������Ȃ��j
		@param[in]	tagstr	�폜����^�O
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void  RemoveTag(const wstring& tagstr) {
			m_TagSet.erase(tagstr);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��������X�e�[�W�𓾂�(const)
		@param[in]	ExceptionActive	�Ώۂ�null�������ꍇ�ɗ�O�������邩�ǂ���
		@return	��������X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Stage> GetStage(bool ExceptionActive = true) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��������X�e�[�W�𓾂�(�^�`�F�b�N����)
		@param[in]	ExceptionActive	�Ώۂ�null�������ꍇ�ɗ�O�������邩�ǂ���
		@return	��������X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		shared_ptr<T> GetTypeStage(bool ExceptionActive = true) const {
			auto StagePtr = GetStage(ExceptionActive);
			if (!StagePtr) {
				return nullptr;
			}
			auto TargetPtr = dynamic_pointer_cast<T>(StagePtr);
			if (!TargetPtr) {
				if (ExceptionActive) {
					throw BaseException(
						L"�X�e�[�W������܂������A�^�L���X�g�ł��܂���",
						Util::GetWSTypeName<T>(),
						L"GameObject::GetTypeStage<T>()"
					);
				}
				else {
					return nullptr;
				}
			}
			return TargetPtr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���|�[�l���g�̎擾�i�h���N���X�ł��j
		@tparam	T	�擾����^
		@param[in]	ExceptionActive	�Ώۂ�null�������ꍇ�ɗ�O�������邩�ǂ���
		@return	�R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		shared_ptr<T> GetComponent(bool ExceptionActive = true)const {
			auto Ptr = SearchDynamicComponent<T>();
			if (Ptr) {
				//�w��̌^�̃R���|�[�l���g����������
				return Ptr;
			}
			else {
				if (ExceptionActive) {
					throw BaseException(
						L"�R���|�[�l���g��������܂���",
						Util::GetWSTypeName<T>(),
						L"GameObject::GetComponent<T>()"
					);
				}
			}
			return nullptr;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	���m�ȃR���|�[�l���g�̎擾
		@tparam	T	�擾����^
		@param[in]	ExceptionActive	�Ώۂ�null�������ꍇ�ɗ�O�������邩�ǂ���
		@return	�R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		shared_ptr<T> GetStrictComponent(bool ExceptionActive = true)const {
			auto CompPtr = SearchStrictComponent(type_index(typeid(T)));
			if (CompPtr) {
				//�w��̌^�̃R���|�[�l���g����������
				return dynamic_pointer_cast<T>(CompPtr);
			}
			else {
				if (ExceptionActive) {
					throw BaseException(
						L"�R���|�[�l���g��������܂���",
						Util::GetWSTypeName<T>(),
						L"GameObject::GetComponent<T>()"
					);
				}
			}
			return nullptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	DynamicCast�𗘗p�����R���|�[�l���g�̎擾�B�R���|�[�l���g�̔h���N���X�g�p���Ɏg��
		@tparam	T	�擾����^�iT�Ɍ^�ϊ��ł�����́j
		@param[in]	ExceptionActive	�Ώۂ�null�������ꍇ�ɗ�O�������邩�ǂ���
		@return	�R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		shared_ptr<T> GetDynamicComponent(bool ExceptionActive = true)const {
			auto Ptr = SearchDynamicComponent<T>();
			if (Ptr) {
				return Ptr;
			}
			else {
				if (ExceptionActive) {
					throw BaseException(
						L"�^�L���X�g�ł���R���|�[�l���g��������܂���",
						Util::GetWSTypeName<T>(),
						L"GameObject::GetDynamicComponent<T>()"
					);
				}
			}
			return nullptr;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���|�[�l���g�̒ǉ��B�V�K�ɃR���|�[�l���g�N���X�����ꍇ�A�R���X�g���N�^�̑�1������GameObject�Ƃ��č쐬����B
		@tparam	T	�ǉ�����^
		@param[in]	params	���̃R���|�[�l���g���\�z����̂Ɏg�p����p�����[�^�B�i��2�p�����[�^�ȍ~�j
		@return	�R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		shared_ptr<T> AddComponent(Ts&&... params) {
			type_index t_index = type_index(typeid(T));
			auto Ptr = SearchDynamicComponent<T>();
			//�w��̌^�̃R���|�[�l���g����������
			if (Ptr) {
				auto RetPtr = dynamic_pointer_cast<T>(Ptr);
				if (RetPtr) {
					return RetPtr;
				}
				else {
					throw BaseException(
						L"���łɃR���|�[�l���g������܂������A�^�L���X�g�ł��܂���",
						Util::GetWSTypeName<T>(),
						L"GameObject::AddComponent<T>()"
					);
				}
			}
			else {
				//������Ȃ��B�V���ɍ쐬����
				shared_ptr<T> newPtr = ObjectFactory::Create<T>(GetThis<GameObject>(), params...);
				//���̃R���|�[�l���g���܂��Ȃ���ΐV�K�o�^
				m_CompOrder.push_back(t_index);
				//map�ɒǉ��������͍X�V
				m_CompMap[t_index] = newPtr;
				newPtr->AttachGameObject(GetThis<GameObject>());
				return newPtr;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���|�[�l���g�̍폜
		@tparam	T	�폜����^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		void RemoveComponent() {
			auto TypeIndex = type_index(typeid(T));
			//���ԃ��X�g�����؂��č폜
			auto it = m_CompOrder.begin();
			while (it != m_CompOrder.end()) {
				if (*it == TypeIndex) {
					auto it2 = m_CompMap.find(*it);
					if (it2 != m_CompMap.end()) {
						//�w��̌^�̃R���|�[�l���g����������
						//map�f�[�^���폜
						m_CompMap.erase(it2);
					}
					m_CompOrder.erase(it);
					return;
				}
				it++;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�s���̎擾�B���݂��Ȃ���΍쐬����
		@tparam	T	�擾����^
		@return	�R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		shared_ptr<T> GetBehavior() {
			auto Ptr = SearchBehavior(type_index(typeid(T)));
			if (Ptr) {
				//�w��̌^�̍s������������
				auto RetPtr = dynamic_pointer_cast<T>(Ptr);
				if (RetPtr) {
					return RetPtr;
				}
				else {
					throw BaseException(
						L"�s��������܂������A�^�L���X�g�ł��܂���",
						Util::GetWSTypeName<T>(),
						L"GameObject::GetBehavior<T>()"
					);
				}
			}
			else {
				//������ΐV���ɐ��삷��
				shared_ptr<T> newPtr = ObjectFactory::Create<T>(GetThis<GameObject>());
				AddMakedBehavior(type_index(typeid(T)), newPtr);
				return newPtr;
			}
			return nullptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�s���̌����B
		@tparam	T	�擾����^
		@return	���݂����true
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		bool FindBehavior() {
			auto Ptr = SearchBehavior(type_index(typeid(T)));
			if (Ptr) {
				//�w��̌^�̍s������������
				auto RetPtr = dynamic_pointer_cast<T>(Ptr);
				if (RetPtr) {
					return true;
				}
				else {
					return false;
				}
			}
			return false;
		}

		void TransformInit();
		void ComponentUpdate();
		void ComponentDraw();
		void ComponentDestroy();

		virtual void OnPreInit() override;
		virtual void OnDraw() override;
		virtual void OnDestroy()override {}
	};


}
//end basedx12
