/*!
@file TransHelper.h
@brief �Փ˔���A��ԏ����p���[�e�B���e�B
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basedx12 {

	//--------------------------------------------------------------------------------------
	///	Easing �e�N���X
	//--------------------------------------------------------------------------------------
	template<typename T>
	class EasingBase {
	protected:
		EasingBase() {}
		virtual ~EasingBase() {}
	public:
		static T Linear(float t, T b, T c, float d) {
			return c * t / d + b;
		}
		virtual T EaseIn(float t, T b, T c, float d) = 0;
		virtual T EaseOut(float t, T b, T c, float d) = 0;
		virtual T EaseInOut(float t, T b, T c, float d) = 0;
	};

	//--------------------------------------------------------------------------------------
	///	Quadratic Easing
	//--------------------------------------------------------------------------------------
	template<typename T>
	class EaseQuad : public EasingBase<T> {
	public:
		EaseQuad():EasingBase() {}
		virtual ~EaseQuad() {}
		virtual T EaseIn(float t, T b, T c, float d)override {
			t /= d;
			return c * t*t + b;
		}
		virtual T EaseOut(float t, T b, T c, float d)override {
			t /= d;
			return -c * t*(t - 2) + b;
		}
		virtual T EaseInOut(float t, T b, T c, float d)override {
			t /= d / 2;
			if (t < 1) return c / 2 * t*t + b;
			t--;
			return -c / 2 * (t*(t - 2) - 1) + b;
		}
	};

	//--------------------------------------------------------------------------------------
	///	Cubic Easing
	//--------------------------------------------------------------------------------------
	template<typename T>
	class EaseCubic : public EasingBase<T> {
	public:
		EaseCubic() :EasingBase() {}
		virtual ~EaseCubic() {}
		virtual T EaseIn(float t, T b, T c, float d)override {
			t /= d;
			return c * t*t*t + b;
		}
		virtual T EaseOut(float t, T b, T c, float d)override {
			t /= d;
			t--;
			return c * (t*t*t + 1) + b;
		}
		virtual T EaseInOut(float t, T b, T c, float d)override {
			t /= d / 2;
			if (t < 1) return c / 2 * t*t*t + b;
			t -= 2;
			return c / 2 * (t*t*t + 2) + b;
		}
	};

	//--------------------------------------------------------------------------------------
	///	Quartic Easing
	//--------------------------------------------------------------------------------------
	template<typename T>
	class EaseQuart : public EasingBase<T> {
	public:
		EaseQuart() :EasingBase() {}
		virtual ~EaseQuart() {}
		virtual T EaseIn(float t, T b, T c, float d)override {
			t /= d;
			return c * t*t*t*t + b;
		}
		virtual T EaseOut(float t, T b, T c, float d)override {
			t /= d;
			t--;
			return -c * (t*t*t*t - 1) + b;
		}
		virtual T EaseInOut(float t, T b, T c, float d)override {
			t /= d / 2;
			if (t < 1) return c / 2 * t*t*t*t + b;
			t -= 2;
			return -c / 2 * (t*t*t*t - 2) + b;
		}
	};


	//--------------------------------------------------------------------------------------
	///	Quintic Easing
	//--------------------------------------------------------------------------------------
	template<typename T>
	class EaseQuint : public EasingBase<T> {
	public:
		EaseQuint() :EasingBase() {}
		virtual ~EaseQuint() {}
		virtual T EaseIn(float t, T b, T c, float d)override {
			t /= d;
			return c * t*t*t*t*t + b;
		}
		virtual T EaseOut(float t, T b, T c, float d)override {
			t /= d;
			t--;
			return c * (t*t*t*t*t + 1) + b;
		}
		virtual T EaseInOut(float t, T b, T c, float d)override {
			t /= d / 2;
			if (t < 1) return c / 2 * t*t*t*t*t + b;
			t -= 2;
			return c / 2 * (t*t*t*t*t + 2) + b;
		}
	};


	//--------------------------------------------------------------------------------------
	///	Sin Easing
	//--------------------------------------------------------------------------------------
	template<typename T>
	class EaseSin : public EasingBase<T> {
	public:
		EaseSin() :EasingBase() {}
		virtual ~EaseSin() {}
		virtual T EaseIn(float t, T b, T c, float d)override {
			return -c * cos(t / d * (XM_PIDIV2)) + c + b;
		}
		virtual T EaseOut(float t, T b, T c, float d)override {
			return c * sin(t / d * (XM_PIDIV2)) + b;
		}
		virtual T EaseInOut(float t, T b, T c, float d)override {
			return -c / 2 * (cos(XM_PI*t / d) - 1) + b;
		}
	};

	//--------------------------------------------------------------------------------------
	///	Exponential Easing
	//--------------------------------------------------------------------------------------
	template<typename T>
	class EaseExpo : public EasingBase<T> {
	public:
		EaseExpo() :EasingBase() {}
		virtual ~EaseExpo() {}
		virtual T EaseIn(float t, T b, T c, float d)override {
			return c * (float)std::pow(2.0f, 10.0f * (t / d - 1.0f)) + b;
		}
		virtual T EaseOut(float t, T b, T c, float d)override {
			return c * (-1.0f * (float)std::pow(2.0f, -10.0f * t / d) + 1.0f) + b;
		}
		virtual T EaseInOut(float t, T b, T c, float d)override {
			t /= d / 2.0f;
			if (t < 1.0f) return c / 2.0f * (float)std::pow(2.0f, 10.0f * (t - 1.0f)) + b;
			t--;
			return c / 2.0f * (-1.0f * (float)std::pow(2.0f, -10.0f * t) + 2.0f) + b;
		}
	};

	//--------------------------------------------------------------------------------------
	///	Circular Easing
	//--------------------------------------------------------------------------------------
	template<typename T>
	class EaseCirc : public EasingBase<T> {
	public:
		EaseCirc() :EasingBase() {}
		virtual ~EaseCirc() {}
		virtual T EaseIn(float t, T b, T c, float d)override {
			t /= d;
			return -c * (std::sqrt(1 - t * t) - 1) + b;

		}
		virtual T EaseOut(float t, T b, T c, float d)override {
			t /= d;
			t--;
			return c * std::sqrt(1 - t * t) + b;
		}
		virtual T EaseInOut(float t, T b, T c, float d)override {
			t /= d / 2;
			if (t < 1) return -c / 2 * (std::sqrt(1 - t * t) - 1) + b;
			t -= 2;
			return c / 2 * (std::sqrt(1 - t * t) + 1) + b;
		}
	};

	enum class EasingType {
		Quadratic,
		Cubic,
		Quartic,
		Quintic,
		Sinusoidal,
		Exponential,
		Circular,
	};



	//--------------------------------------------------------------------------------------
	///	Easing�N���X�e���v���[�g��
	//--------------------------------------------------------------------------------------
	template<typename T>
	class Easing {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		Easing() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		~Easing() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief�@Linear���v�Z����
		@param[in]	Start	�J�n�l�iT�^�j
		@param[in]	End		�I���l�iT�^�j
		@param[in]	TgtTime	�v�Z����^�C��
		@param[in]	AllTime	�g�[�^���^�C��
		@return	�v�Z���ʃx�N�g��
		*/
		//--------------------------------------------------------------------------------------
		T Linear(const T& Start, T& End, float TgtTime, float AllTime) {
			auto SpanVec = End - Start;
			return EasingBase<T>::Linear(TgtTime, Start, SpanVec, AllTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief�@EaseIn���v�Z����
		@param[in]	type	EasingType
		@param[in]	Start	�J�n�l�iT�^�j
		@param[in]	End		�I���l�iT�^�j
		@param[in]	TgtTime	�v�Z����^�C��
		@param[in]	AllTime	�g�[�^���^�C��
		@return	�v�Z���ʃx�N�g��
		*/
		//--------------------------------------------------------------------------------------
		T EaseIn(EasingType type, const T& Start, const T& End, float TgtTime, float AllTime) {
			auto SpanVec = End - Start;
			switch (type) {
			case EasingType::Quadratic:
			{
				EaseQuad<T> es;
				return es.EaseIn(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Cubic:
			{
				EaseCubic<T> es;
				return es.EaseIn(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Quartic:
			{
				EaseQuart<T> es;
				return es.EaseIn(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Quintic:
			{
				EaseQuint<T> es;
				return es.EaseIn(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Sinusoidal:
			{
				EaseSin<T> es;
				return es.EaseIn(TgtTime, Start, SpanVec, AllTime);

			}
			break;
			case EasingType::Exponential:
			{
				EaseExpo<T> es;
				return es.EaseIn(TgtTime, Start, SpanVec, AllTime);

			}
			break;
			case EasingType::Circular:
			{
				EaseCirc<T> es;
				return es.EaseIn(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			}
			//�G���[�̏ꍇ�̓X�^�[�g��߂�
			return Start;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief�@EaseOut���v�Z����
		@param[in]	type	EasingType
		@param[in]	Start	�J�n�l�iT�^�j
		@param[in]	End		�I���l�iT�^�j
		@param[in]	TgtTime	�v�Z����^�C��
		@param[in]	AllTime	�g�[�^���^�C��
		@return	�v�Z���ʃx�N�g��
		*/
		//--------------------------------------------------------------------------------------
		T EaseOut(EasingType type, const T& Start, const T& End, float TgtTime, float AllTime) {
			auto SpanVec = End - Start;
			switch (type) {
			case EasingType::Quadratic:
			{
				EaseQuad<T> es;
				return es.EaseOut(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Cubic:
			{
				EaseCubic<T> es;
				return es.EaseOut(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Quartic:
			{
				EaseQuart<T> es;
				return es.EaseOut(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Quintic:
			{
				EaseQuint<T> es;
				return es.EaseOut(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Sinusoidal:
			{
				EaseSin<T> es;
				return es.EaseOut(TgtTime, Start, SpanVec, AllTime);

			}
			break;
			case EasingType::Exponential:
			{
				EaseExpo<T> es;
				return es.EaseOut(TgtTime, Start, SpanVec, AllTime);

			}
			break;
			case EasingType::Circular:
			{
				EaseCirc<T> es;
				return es.EaseOut(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			}
			//�G���[�̏ꍇ�̓X�^�[�g��߂�
			return Start;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief�@EaseInOut���v�Z����
		@param[in]	Start	�J�n�l�iT�^�j
		@param[in]	End		�I���l�iT�^�j
		@param[in]	End		�I���x�N�g��
		@param[in]	TgtTime	�v�Z����^�C��
		@param[in]	AllTime	�g�[�^���^�C��
		@return	�v�Z���ʃx�N�g��
		*/
		//--------------------------------------------------------------------------------------
		T EaseInOut(EasingType type, T& Start, const T& End, float TgtTime, float AllTime) {
			auto SpanVec = End - Start;
			switch (type) {
			case EasingType::Quadratic:
			{
				EaseQuad<T> es;
				return es.EaseInOut(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Cubic:
			{
				EaseCubic<T> es;
				return es.EaseInOut(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Quartic:
			{
				EaseQuart<T> es;
				return es.EaseInOut(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Quintic:
			{
				EaseQuint<T> es;
				return es.EaseInOut(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Sinusoidal:
			{
				EaseSin<T> es;
				return es.EaseInOut(TgtTime, Start, SpanVec, AllTime);

			}
			break;
			case EasingType::Exponential:
			{
				EaseExpo<T> es;
				return es.EaseInOut(TgtTime, Start, SpanVec, AllTime);

			}
			break;
			case EasingType::Circular:
			{
				EaseCirc<T> es;
				return es.EaseInOut(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			}
			//�G���[�̏ꍇ�̓X�^�[�g��߂�
			return Start;
		}
	};


	//--------------------------------------------------------------------------------------
	///	Lerp ��ԏ����v�Z�\����
	//--------------------------------------------------------------------------------------
	struct Lerp{
		enum rate{
			Linear,	//1���֐��i���`�j
			Easein,	//2���֐��������㏸
			EaseOut,//2���֐��Ōオ�������
			Cube,	//3���֐�
			Cos		//�R�T�C��
		};
		//--------------------------------------------------------------------------------------
		/*!
		@brief	T�^�̕�Ԍv�Z���s��
		@tparam	T	�v�Z����^
		@param[in]	Start	�J�n�l
		@param[in]	End	�I���l
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[in]	NowTime	���݂̎���
		@param[in]	r	�v�Z���@
		@return	T�^�̌v�Z����
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static T CalculateLerp(const T &Start, const T &End,
		float StartTime, float EndTime, float NowTime, rate r){
			float t = (NowTime - StartTime) / (EndTime - StartTime);
			if (t < 0.0f){
				t = 0;
			}
			if (t > 1.0f){
				t = 1.0f;
			}
			float cal_rate = t;   // 1���֐���Ԓl�ɕϊ�(�f�t�H���g)
			switch (r){
			case Linear:
				cal_rate = t;
				break;
			case Easein:
				cal_rate = t * t;
				break;
			case EaseOut:
				cal_rate = t * (2.0f - t);
				break;
			case Cube:
				cal_rate = t * t * (3.0f - 2.0f * t);
				break;
			case Cos:
				cal_rate = (1.0f - cos(t * XM_PI)) / 2.0f;
				break;
			default:
				break;
			}
			return Start * (1.0f - cal_rate) + End * cal_rate;
		}
	};


	//--------------------------------------------------------------------------------------
	///	XZ�|�C���g(Z�����v���X�̓_)
	//--------------------------------------------------------------------------------------
	struct POINT_XZ {
		float m_X;
		float m_Z;
		POINT_XZ() :
			m_X(0.0f), m_Z(0.0f)
		{}
		POINT_XZ(float x,float z) :
			m_X(x), m_Z(z)
		{}
		POINT_XZ(const POINT_XZ& other) :
			m_X(other.m_X),
			m_Z(other.m_Z)
		{}
		POINT_XZ(const bsm::Float2& other) :
			m_X(other.x),
			m_Z(other.y)
		{}
		POINT_XZ(const bsm::Float3& other) :
			m_X(other.x),
			m_Z(other.z)
		{}
		operator bsm::Float2() {
			return bsm::Float2(m_X,m_Z);
		}
		POINT_XZ& operator=(const POINT_XZ& other) {
			//���ȑ���̗}��
			if (this != &other) {
				m_X = other.m_X;
				m_Z = other.m_Z;
			}
			return *this;
		}
		POINT_XZ& operator=(const bsm::Float2& other) {
			m_X = other.x;
			m_Z = other.y;
			return *this;
		}
		POINT_XZ& operator=(const bsm::Float3& other) {
			m_X = other.x;
			m_Z = other.z;
			return *this;
		}
	};

	//--------------------------------------------------------------------------------------
	///	XZ�����̃Z���Ɏg�p����C���f�b�N�X
	//--------------------------------------------------------------------------------------
	struct CellIndex {
		int x;
		int z;
		CellIndex() :
			//�����l�ŏ�����
			x(-1), z(-1)
		{}
		CellIndex(int xx, int zz) :
			x(xx), z(zz)
		{
		}
		bool operator==(const CellIndex& other)const {
			return (x == other.x && z == other.z);
		}
		bool operator!=(const CellIndex& other)const {
			return ((x != other.x) || (z != other.z));
		}
		static int GetHeuristic(const CellIndex& Cell1, const CellIndex& Cell2) {
			int spanX = abs(Cell1.x - Cell2.x);
			int spanZ = abs(Cell1.z - Cell2.z);
			//�ǂ��炩�����ق���Ԃ�
			return (spanX >= spanZ) ? spanX : spanZ;
		}
	};


	//--------------------------------------------------------------------------------------
	///	XZ��`(Z�����v���X�̋�`)
	//--------------------------------------------------------------------------------------
	struct RECT_XZ {
		float m_Left;
		float m_Near;
		float m_Right;
		float m_Far;
		RECT_XZ() :
			m_Left(0.0f), m_Near(0.0f),
			m_Right(0.0f), m_Far(0.0f)
		{}
		RECT_XZ(float l,float n,float r,float f) :
			m_Left(l), m_Near(n),
			m_Right(r), m_Far(f)
		{}

		POINT_XZ CenterPoint() const{
			return POINT_XZ((m_Left + m_Right) / 2.0f, (m_Near + m_Far) / 2.0f);
		}
		void operator+=(const POINT_XZ& point){
			m_Left += point.m_X;
			m_Right += point.m_X;
			m_Near += point.m_Z;
			m_Far += point.m_Z;
		}
		RECT_XZ operator+(const POINT_XZ& point) const {
			RECT_XZ rect(*this);
			rect += point;
			return rect;
		}
		void operator-=(const POINT_XZ& point) {
			m_Left -= point.m_X;
			m_Right -= point.m_X;
			m_Near -= point.m_Z;
			m_Far -= point.m_Z;
		}
		RECT_XZ operator-(const POINT_XZ& point) const {
			RECT_XZ rect(*this);
			rect -= point;
			return rect;
		}
		bool IsRectEmpty()const{
			if (m_Left >= m_Right || m_Near >= m_Far) {
				return true;
			}
			return false;
		}
		bool PtInRect(const POINT_XZ& point) const{
			if (point.m_X >= m_Left && point.m_X < m_Right
				&& point.m_Z >= m_Near && point.m_Z < m_Far) {
				return true;
			}
			return false;
		}
		bool PtInRect(const bsm::Float2& point) const {
			POINT_XZ p(point);
			return PtInRect(p);
		}
		bool Intersect(const RECT_XZ& rect2) const{
			if (IsRectEmpty() || rect2.IsRectEmpty()) {
				//�ǂ��炩����̏ꍇ��false
				return false;
			}
			if (m_Left > rect2.m_Right) {
				return false;
			}
			if (m_Right < rect2.m_Left) {
				return false;
			}
			if (m_Near > rect2.m_Far) {
				return false;
			}
			if (m_Far < rect2.m_Near) {
				return false;
			}
			return true;
		}


		RECT_XZ UnionRect(const RECT_XZ& rect1, const RECT_XZ& rect2) const{
			RECT_XZ ret;
			ret.m_Left = (rect1.m_Left <= rect2.m_Left) ? rect1.m_Left : rect2.m_Left;
			ret.m_Right = (rect1.m_Right >= rect2.m_Right) ? rect1.m_Right : rect2.m_Right;
			ret.m_Near = (rect1.m_Near <= rect2.m_Near) ? rect1.m_Near : rect2.m_Near;
			ret.m_Far = (rect1.m_Far >= rect2.m_Far) ? rect1.m_Far : rect2.m_Far;
			return ret;
		}
		RECT_XZ UnionRect(const RECT_XZ& other){
			RECT_XZ ret;
			ret.m_Left = (m_Left <= other.m_Left) ? m_Left : other.m_Left;
			ret.m_Right = (m_Right >= other.m_Right) ? m_Right : other.m_Right;
			ret.m_Near = (m_Near <= other.m_Near) ? m_Near : other.m_Near;
			ret.m_Far = (m_Far >= other.m_Far) ? m_Far : other.m_Far;
			*this = ret;
			return ret;
		}
		RECT_XZ operator|=(const RECT_XZ& other) {
			return UnionRect(other);
		}
		RECT_XZ operator|(const RECT_XZ& other) const{
			return UnionRect(*this,other);
		}
		float Width()const{
			return abs(m_Right - m_Left);
		}
		float Dipth()const{
			return abs(m_Far - m_Near);
		}
	};

	struct SPHERE;

	//--------------------------------------------------------------------------------------
	///	AABB�{�����[�����E
	//--------------------------------------------------------------------------------------
	struct AABB {
		bsm::Float3 m_Min;     ///< �������ق��̍��W
		bsm::Float3 m_Max;     ///< �傫���ق��̍��W
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		AABB():
			m_Min(0,0,0),
			m_Max(0, 0, 0){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	Min	�������ق��̍��W
		@param[in]	Max	�傫���ق��̍��W
		*/
		//--------------------------------------------------------------------------------------
		AABB(const bsm::Float3& Min, const bsm::Float3& Max)
			:m_Min(Min), m_Max(Max) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	Center	���S
		@param[in]	XSize	X�����T�C�Y
		@param[in]	YSize	Y�����T�C�Y
		@param[in]	ZSize	Z�����T�C�Y
		*/
		//--------------------------------------------------------------------------------------
		AABB(const bsm::Float3& Center, float XSize, float YSize, float ZSize) {
			float xh = XSize / 2.0f;
			float yh = YSize / 2.0f;
			float zh = ZSize / 2.0f;

			m_Min.x = Center.x - xh;
			m_Max.x = Center.x + xh;

			m_Min.y = Center.y - yh;
			m_Max.y = Center.y + yh;

			m_Min.z = Center.z - zh;
			m_Max.z = Center.z + zh;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���S�𓾂�
		@param[out]	Pos	���S����������x�N�g��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetCenter(bsm::Float3& Pos)const {
			Pos.x = (m_Min.x + m_Max.x) / 2.0f;
			Pos.y = (m_Min.y + m_Max.y) / 2.0f;
			Pos.z = (m_Min.z + m_Max.z) / 2.0f;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���S�𓾂�
		@return	���S�̃x�N�g��
		*/
		//--------------------------------------------------------------------------------------
		bsm::Float3 GetCenter() const{
			bsm::Float3 ret;
			ret.x = (m_Min.x + m_Max.x) / 2.0f;
			ret.y = (m_Min.y + m_Max.y) / 2.0f;
			ret.z = (m_Min.z + m_Max.z) / 2.0f;
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���𓾂�
		@return	��
		*/
		//--------------------------------------------------------------------------------------
		float GetWidth() const {
			return abs(m_Max.x - m_Min.x);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����𓾂�
		@return	����
		*/
		//--------------------------------------------------------------------------------------
		float GetHeight() const {
			return abs(m_Max.y - m_Min.y);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���s�𓾂�
		@return	���s
		*/
		//--------------------------------------------------------------------------------------
		float GetDepth() const {
			return abs(m_Max.z - m_Min.z);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�ړ�����
		@param[in]	MoveVec	�ړ��x�N�g��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Move(const bsm::Float3& MoveVec) {
			m_Min += MoveVec;
			m_Max += MoveVec;
		}
		bool PtInAABB(const bsm::Float3& point) {
			if (
				point.x >= m_Min.x && point.x < m_Max.x
				&&
				point.y >= m_Min.y && point.y < m_Max.y
				&&
				point.z >= m_Min.z && point.z < m_Max.z
				) {
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ݍ���XZ��`�𓾂�
		@return	XZ��`
		*/
		//--------------------------------------------------------------------------------------
		RECT_XZ GetWrappedRECT_XZ() const {
			return RECT_XZ(m_Min.x, m_Min.z, m_Max.x, m_Max.z);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ݍ���SPHERE�𓾂�
		@return	SPHERE
		*/
		//--------------------------------------------------------------------------------------
		SPHERE GetWrappedSPHERE() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�������AABB���ݍ��ނő傫������
		@return	��ݍ���AABB
		*/
		//--------------------------------------------------------------------------------------
		AABB& UnionAABB(const AABB& other) {
			for (int i = 0; i < 3; i++) {
				if (other.m_Min[i] < m_Min[i]) {
					m_Min[i] = other.m_Min[i];
				}
				if (other.m_Max[i] > m_Max[i]) {
					m_Max[i] = other.m_Max[i];
				}
			}
			return *this;
		}
	};



	//--------------------------------------------------------------------------------------
	///	���{�����[�����E
	//--------------------------------------------------------------------------------------
	struct SPHERE {
		bsm::Float3 m_Center;	///< ���S�_�̍��W
		float m_Radius;			///< ���a
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		SPHERE() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	Center	���S
		@param[in]	Radius	���a
		*/
		//--------------------------------------------------------------------------------------
		SPHERE(const bsm::Float3& Center, float Radius) :
			m_Center(Center),
			m_Radius(Radius) {}
		AABB GetWrappedAABB() const {
			AABB ret;
			ret.m_Min = bsm::Float3(m_Center.x - m_Radius, m_Center.y - m_Radius, m_Center.z - m_Radius);
			ret.m_Max = bsm::Float3(m_Center.x + m_Radius, m_Center.y + m_Radius, m_Center.z + m_Radius);
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ݍ���XZ��`�𓾂�
		@return	XZ��`
		*/
		//--------------------------------------------------------------------------------------
		RECT_XZ GetWrappedRECT_XZ() const {
			RECT_XZ ret;
			ret.m_Left = m_Center.x - m_Radius;
			ret.m_Near = m_Center.z - m_Radius;
			ret.m_Right = m_Center.x + m_Radius;
			ret.m_Far = m_Center.z + m_Radius;
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ݍ���SPHERE�𓾂�
		@return	SPHERE
		*/
		//--------------------------------------------------------------------------------------
		SPHERE GetWrappedSPHERE() const {
			return *this;
		}

	};

	inline SPHERE AABB::GetWrappedSPHERE() const {
		SPHERE sp;
		auto Size = m_Max - m_Min;
		Size *= 0.5f;
		sp.m_Radius = bsm::length(Size);
		sp.m_Center = GetCenter();
		return sp;
	}



	//--------------------------------------------------------------------------------------
	///	OBB�{�����[�����E
	//--------------------------------------------------------------------------------------
	struct OBB{
		bsm::Float3 m_Center;     ///< ���S�_�̍��W
		bsm::Float3 m_Rot[3];  ///< XYZ �̊e���W���̌X����\�������x�N�g��
		bsm::Float3 m_Size;     ///< OBB �̊e���W���ɉ����������̔����i���S�_����ʂ܂ł̒����j
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�쐬���ꂽ�T�C�Y�ƕϊ��s�񂩂�AOBB����蒼��
		@param[in]	Size	�쐬�T�C�Y
		@param[in]	Matrix	�ϊ��s��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void CreateOBB(const bsm::Float3& Size,const bsm::Mat4x4& Matrix){
			m_Center.x = Matrix._41;
			m_Center.y = Matrix._42;
			m_Center.z = Matrix._43;
			bsm::Float3 VecX(Matrix._11,Matrix._12,Matrix._13);
			bsm::Float3 VecY(Matrix._21,Matrix._22,Matrix._23);
			bsm::Float3 VecZ(Matrix._31,Matrix._32,Matrix._33);
			m_Size = bsm::Float3(
				Size.x * bsm::length(VecX),
				Size.y * bsm::length(VecY),
				Size.z * bsm::length(VecZ)
				);
			m_Size *= 0.5f;
			//��]�𓾂�
			m_Rot[0] = bsm::normalize(VecX);
			m_Rot[1] = bsm::normalize(VecY);
			m_Rot[2] = bsm::normalize(VecZ);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		OBB(){
			//�f�t�H���g��1��1.0��OBB
			bsm::Float3 SizeVec(1.0f,1.0f,1.0f);
			//�s��̓A�C�f���e�B�e�B
			bsm::Mat4x4 Matrix;
			CreateOBB(SizeVec,Matrix);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	Size	�T�C�Y
		@param[in]	Rot	��]�x�N�g��
		@param[in]	Center	���S�ʒu
		*/
		//--------------------------------------------------------------------------------------
		OBB(const bsm::Float3& Size,const bsm::Float3& Rot,const bsm::Float3& Center):
			m_Center(Center),
			m_Size(Size)
		{
			m_Size *= 0.5f;
			//��]�𓾂�
			bsm::Float3 LocaRot = Rot;
			LocaRot.normalize();
			bsm::Mat4x4 mRot(XMMatrixRotationRollPitchYawFromVector(LocaRot));
			m_Rot[0] = bsm::Float3(mRot._11,mRot._12,mRot._13);
			m_Rot[1] = bsm::Float3(mRot._21,mRot._22,mRot._23);
			m_Rot[2] = bsm::Float3(mRot._31,mRot._32,mRot._33);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	Size	�T�C�Y
		@param[in]	Qt	��]�N�I�[�^�j�I��
		@param[in]	Center	���S�ʒu
		*/
		//--------------------------------------------------------------------------------------
		OBB(const bsm::Float3& Size,const bsm::Quat& Qt,const bsm::Float3& Center):
			m_Center(Center),
			m_Size(Size)
		{
			m_Size *= 0.5f;
			//��]�𓾂�
			bsm::Quat LocalQt = Qt;
			//�N�I�[�^�j�I���𐳋K��
			LocalQt.normalize();
			//�N�I�[�^�j�I�������]�s����쐬
			bsm::Mat4x4 mRot(XMMatrixRotationQuaternion(LocalQt));
			m_Rot[0] = bsm::Float3(mRot._11,mRot._12,mRot._13);
			m_Rot[1] = bsm::Float3(mRot._21,mRot._22,mRot._23);
			m_Rot[2] = bsm::Float3(mRot._31,mRot._32,mRot._33);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^�i�쐬���ꂽ�T�C�Y�ƕϊ��s�񂩂�AOBB���쐬�j
		@param[in]	Size	�T�C�Y
		@param[in]	Matrix	�ϊ��s��
		*/
		//--------------------------------------------------------------------------------------
		OBB(const bsm::Float3& Size,const bsm::Mat4x4& Matrix)
		{
			m_Center.x = Matrix._41;
			m_Center.y = Matrix._42;
			m_Center.z = Matrix._43;
			bsm::Float3 VecX(Matrix._11,Matrix._12,Matrix._13);
			bsm::Float3 VecY(Matrix._21,Matrix._22,Matrix._23);
			bsm::Float3 VecZ(Matrix._31,Matrix._32,Matrix._33);
			m_Size = bsm::Float3(
				Size.x * bsm::length(VecX),
				Size.y * bsm::length(VecY),
				Size.z * bsm::length(VecZ)
				);
			m_Size *= 0.5f;
			//��]�𓾂�
			m_Rot[0] = bsm::normalize(VecX);
			m_Rot[1] = bsm::normalize(VecY);
			m_Rot[2] = bsm::normalize(VecZ);

		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��]�s��𓾂�
		@return	��]�s��
		*/
		//--------------------------------------------------------------------------------------
		bsm::Mat4x4 GetRotMatrix()const{
			bsm::Mat4x4 ret;
			ret._11 = m_Rot[0].x;
			ret._12 = m_Rot[0].y;
			ret._13 = m_Rot[0].z;
			ret._21 = m_Rot[1].x;
			ret._22 = m_Rot[1].y;
			ret._23 = m_Rot[1].z;
			ret._31 = m_Rot[2].x;
			ret._32 = m_Rot[2].y;
			ret._33 = m_Rot[2].z;
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�s��𓾂�
		@return	���[���h�s��
		*/
		//--------------------------------------------------------------------------------------
		bsm::Mat4x4 GetWorldMatrix()const {
			bsm::Mat4x4 ret,Pos;
			ret = XMMatrixScaling(m_Size.x * 2.0f, m_Size.y * 2.0f, m_Size.z * 2.0f);
			ret = XMMatrixMultiply(ret, GetRotMatrix());
			Pos = XMMatrixTranslation(m_Center.x, m_Center.y, m_Center.z);
			ret = XMMatrixMultiply(ret, Pos);
			return ret;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ԑ傫���ӂ̔��������𓾂�
		@return	��ԑ傫���ӂ̔����̒���
		*/
		//--------------------------------------------------------------------------------------
		float GetMaxHalfSize()const {
			float ret = m_Size.x;
			if (ret < m_Size.y) {
				ret = m_Size.y;
			}
			if (ret < m_Size.z) {
				ret = m_Size.z;
			}
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ԑ傫���ӂ̒����𓾂�
		@return	��ԑ傫���ӂ̒���
		*/
		//--------------------------------------------------------------------------------------
		float GetMaxSize()const {
			return GetMaxHalfSize() * 2.0f;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ԏ������ӂ̒��������𓾂�
		@return	��ԏ������ӂ̒����̔���
		*/
		//--------------------------------------------------------------------------------------
		float GetMinHalfSize() const {
			float ret = m_Size.x;
			if (ret > m_Size.y) {
				ret = m_Size.y;
			}
			if (ret > m_Size.z) {
				ret = m_Size.z;
			}
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ԏ������ӂ̒����𓾂�
		@return	��ԏ������ӂ̒���
		*/
		//--------------------------------------------------------------------------------------
		float GetMinSize() const {
			return  GetMinHalfSize() * 2.0f;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�x�N�g���Ɉ�ԋ߂���]�𓾂�
		@param[in]	Check	�`�F�b�N����x�N�g��
		@param[in]	Epsilon	�덷
		@param[out]	Ret	�߂����x�N�g��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetNearNormalRot(const bsm::Float3& Check, float Epsilon,bsm::Float3& Ret){
			for (int i = 0; i < 3; i++){
				bsm::Float4 VecEp(Epsilon);
				if (XMVector3NearEqual(Check, m_Rot[i], VecEp)) {
					Ret = m_Rot[i];
					return;
				}
				if (XMVector3NearEqual(Check, -m_Rot[i], VecEp)) {
					Ret = -m_Rot[i];
					return;
				}
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OBB����]���ĂȂ����ǂ����𒲂ׂ�iAABB�Ƃ��Ďg���邩�ǂ����𒲂ׂ���j
		@return	��]���ĂȂ����true
		*/
		//--------------------------------------------------------------------------------------
		bool IsRotIdentity() const{
			if (m_Rot[0] == bsm::Float3(1.0f, 0.0f, 0.0f) 
				&& m_Rot[1] == bsm::Float3(0.0f, 1.0f, 0.0f) 
				&& m_Rot[2] == bsm::Float3(0.0f, 0.0f, 1.0f)){
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ݍ���XZ��`�𓾂�
		@return	XZ��`
		*/
		//--------------------------------------------------------------------------------------
		RECT_XZ GetWrappedRECT_XZ() const {
			RECT_XZ ret;
			SPHERE sp;
			sp.m_Radius = bsm::length(m_Size);
			sp.m_Center = m_Center;
			return sp.GetWrappedRECT_XZ();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ݍ���SPHERE�𓾂�
		@return	SPHERE
		*/
		//--------------------------------------------------------------------------------------
		SPHERE GetWrappedSPHERE() const {
			SPHERE sp;
			sp.m_Radius = bsm::length(m_Size);
			sp.m_Center = m_Center;
			return sp;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ݍ���AABB�𓾂�
		@return	AABB
		*/
		//--------------------------------------------------------------------------------------
		AABB GetWrappedAABB() const {
			AABB ret;
			if (IsRotIdentity()) {
				ret.m_Min = bsm::Float3(-m_Size.x, -m_Size.y, -m_Size.z);
				ret.m_Max = bsm::Float3(m_Size.x, m_Size.y, m_Size.z);
				ret.Move(m_Center);
				return ret;
			}
			float Half = 0.5f;
			bsm::Float3 Vec[] = {
				bsm::Float3(-Half,-Half,-Half),
				bsm::Float3(Half,-Half,-Half),
				bsm::Float3(Half,-Half,Half),
				bsm::Float3(-Half,-Half,Half),

				bsm::Float3(-Half,Half,-Half),
				bsm::Float3(Half,Half,-Half),
				bsm::Float3(Half,Half,Half),
				bsm::Float3(-Half,Half,Half),
			};
			bool First = true;
			auto m = GetWorldMatrix();
			for (auto& v : Vec) {
				v *= m;
				if (First) {
					ret.m_Min = v;
					ret.m_Max = v;
					First = false;
				}
				else {
					if (ret.m_Min.x > v.x) {
						ret.m_Min.x = v.x;
					}
					if (ret.m_Max.x < v.x) {
						ret.m_Max.x = v.x;
					}

					if (ret.m_Min.y > v.y) {
						ret.m_Min.y = v.y;
					}
					if (ret.m_Max.y < v.y) {
						ret.m_Max.y = v.y;
					}

					if (ret.m_Min.z > v.z) {
						ret.m_Min.z = v.z;
					}
					if (ret.m_Max.z < v.z) {
						ret.m_Max.z = v.z;
					}
				}
			}
			return ret;
		}

	};



	//--------------------------------------------------------------------------------------
	///	���z��f�[�^
	//--------------------------------------------------------------------------------------
	struct SPHERE_ARR{
		vector<SPHERE> m_SphereArr;
	};

	//--------------------------------------------------------------------------------------
	///	����
	//--------------------------------------------------------------------------------------
	struct PLANE{
		bsm::Float3 m_Normal;	///< �@��
		float m_DotValue;	///< ���ϒl
		//--------------------------------------------------------------------------------------
		/*!
		@brief	3�̓_���畽�ʂ��쐬
		@param[in]	PointA	�_A
		@param[in]	PointB	�_B
		@param[in]	PointC	�_C
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Set(const bsm::Float3& PointA,const bsm::Float3& PointB,const bsm::Float3& PointC){
			m_Normal = bsm::normalize(bsm::cross(PointB - PointA,PointC - PointA));
			m_DotValue = bsm::dot(m_Normal,PointA);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		PLANE(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^�i3�̓_���畽�ʂ��쐬�j
		@param[in]	PointA	�_A
		@param[in]	PointB	�_B
		@param[in]	PointC	�_C
		*/
		//--------------------------------------------------------------------------------------
		PLANE(const bsm::Float3& PointA,const bsm::Float3& PointB,const bsm::Float3& PointC){
			Set(PointA,PointB,PointC);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^�ibsm::Float4�^���畽�ʂ��쐬�j
		@param[in]	plane	bsm::Float4�^�̒l
		*/
		//--------------------------------------------------------------------------------------
		PLANE(const bsm::Float4& plane){
			m_Normal.x = plane.x;
			m_Normal.y = plane.x;
			m_Normal.z = plane.z;
			m_DotValue = -plane.w;
		}
	};


	//--------------------------------------------------------------------------------------
	///	�V�����_�[�{�����[�����E
	//--------------------------------------------------------------------------------------
	struct CYLINDER {
		float m_Radius;			///< ���a
		bsm::Float3 m_PointBottom;		///< ���ԕ������̊J�n�_
		bsm::Float3 m_PointTop;		///< ���ԕ������̏I���_
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		CYLINDER() :
			m_Radius(1.0f),
			m_PointBottom(0, -0.5f, 0),
			m_PointTop(0, 0.5f, 0)
		{
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	Radius	���a
		@param[in]	PointBottom	���ԕ������̊J�n�_
		@param[in]	PointTop	���ԕ������̏I���_
		*/
		//--------------------------------------------------------------------------------------
		CYLINDER(float Radius, const bsm::Float3& PointBottom, const bsm::Float3& PointTop) :
			m_Radius(Radius),
			m_PointBottom(PointBottom),
			m_PointTop(PointTop)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���S�_�𓾂�
		@return	���S�_
		*/
		//--------------------------------------------------------------------------------------
		bsm::Float3 GetCenter() const {
			return m_PointBottom + ((m_PointTop - m_PointBottom) * 0.5f);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����𓾂�
		@return	����
		*/
		//--------------------------------------------------------------------------------------
		float GetHeight() const {
			return bsm::length(m_PointTop - m_PointBottom);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���a1.0�A����1.0�̊�{�`����̃��[���h�s��𓾂�
		@return	���[���h�s��
		*/
		//--------------------------------------------------------------------------------------
		bsm::Mat4x4 GetMatrix() const{
			bsm::Mat4x4 mat;
			bsm::Float3 Pos = GetCenter();
			bsm::Float3 Rot = m_PointTop - m_PointBottom;
			Rot.normalize();
			bsm::Quat Qt(Rot,0.0f);
			float Height = bsm::length(m_PointTop - m_PointBottom);
			bsm::Float3 Scale(m_Radius, Height, m_Radius);
			mat.affineTransformation(Scale, bsm::Float3(0, 0, 0), Qt, Pos);
			return mat;
		}
	};


	//--------------------------------------------------------------------------------------
	///	�J�v�Z���{�����[�����E
	//--------------------------------------------------------------------------------------
	struct CAPSULE {
		float m_Radius;			///< ���a
		bsm::Float3 m_PointBottom;		///< ���ԕ������̊J�n�_
		bsm::Float3 m_PointTop;		///< ���ԕ������̏I���_
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		CAPSULE():
			m_Radius(0.5f),
			m_PointBottom(0,-0.5f,0),
			m_PointTop(0,0.5f,0)
		{
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	Radius	���a
		@param[in]	PointBottom	���ԕ������̊J�n�_
		@param[in]	PointTop	���ԕ������̏I���_
		*/
		//--------------------------------------------------------------------------------------
		CAPSULE(float Radius,const bsm::Float3& PointBottom,const bsm::Float3& PointTop):
			m_Radius(Radius),
			m_PointBottom(PointBottom),
			m_PointTop(PointTop)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	MakedRadius	�쐬���̔��a
		@param[in]	MakedPointBottom	�쐬���̒��ԕ������̊J�n�_
		@param[in]	MakedPointTop	�쐬���̒��ԕ������̏I���_
		@param[in]	Matrix	�ϊ��s��
		*/
		//--------------------------------------------------------------------------------------
		CAPSULE(float MakedRadius,const bsm::Float3& MakedPointBottom,const bsm::Float3& MakedPointTop,
			const bsm::Mat4x4& Matrix):
			m_Radius(MakedRadius),
			m_PointBottom(MakedPointBottom),
			m_PointTop(MakedPointTop)
		{
			m_PointBottom *= Matrix;
			m_PointTop *= Matrix;
			//�X�P�[�����O�̂�1�����Ōv�Z
			bsm::Float3 Scale = Matrix.scaleInMatrix();
			m_Radius *= Scale.x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���S�_�𓾂�
		@return	���S�_
		*/
		//--------------------------------------------------------------------------------------
		bsm::Float3 GetCenter() const{
			return m_PointBottom + ((m_PointTop - m_PointBottom) * 0.5f);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���S�_��ݒ肷��
		@param[in]	Center	���S�_
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetCenter(const bsm::Float3& Center){
			bsm::Float3 CenterToPointA = ((m_PointBottom - m_PointTop) * 0.5f);
			bsm::Float3 CenterToPointB = ((m_PointTop - m_PointBottom) * 0.5f);
			m_PointBottom = Center + CenterToPointA;
			m_PointTop = Center + CenterToPointB;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�c�̔��a�𓾂�
		@return	�c�̔��a
		*/
		//--------------------------------------------------------------------------------------
		float GetHeightRadius()const{
			float PointLen = bsm::length(m_PointTop - m_PointBottom) * 0.5f;
			PointLen += m_Radius;
			return PointLen;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ݍ���XZ��`�𓾂�
		@return	XZ��`
		*/
		//--------------------------------------------------------------------------------------
		RECT_XZ GetWrappedRECT_XZ() const {
			RECT_XZ ret;
			POINT_XZ P1(m_PointBottom);
			POINT_XZ P2(m_PointTop);
			if (bsm::length(bsm::Float2(P1)) <= bsm::length(bsm::Float2(P2))) {
				ret.m_Left = P1.m_X - m_Radius;
				ret.m_Right = P2.m_X + m_Radius;
				ret.m_Near = P1.m_Z - m_Radius;
				ret.m_Far = P2.m_Z + m_Radius;
			}
			else {
				ret.m_Left = P2.m_X - m_Radius;
				ret.m_Right = P1.m_X + m_Radius;
				ret.m_Near = P2.m_Z - m_Radius;
				ret.m_Far = P1.m_Z + m_Radius;
			}
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ݍ���SPHERE�𓾂�
		@return	SPHERE
		*/
		//--------------------------------------------------------------------------------------
		SPHERE GetWrappedSPHERE() const {
			SPHERE sp;
			auto Size = bsm::length(m_PointTop - m_PointBottom);
			Size *= 0.5f;
			Size += m_Radius;
			sp.m_Radius = Size;
			sp.m_Center = GetCenter();
			return sp;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ݍ���AABB�𓾂�
		@return	AABB
		*/
		//--------------------------------------------------------------------------------------
		AABB GetWrappedAABB() const;
	};

	//--------------------------------------------------------------------------------------
	///	�Փ˔���Ɏg�p����O�p�`�i���v���j
	//--------------------------------------------------------------------------------------
	struct TRIANGLE{
		bsm::Float3 m_A;
		bsm::Float3 m_B;
		bsm::Float3 m_C;
		void Set(const bsm::Float3& baseA, const bsm::Float3& baseB, const bsm::Float3& baseC,
			const bsm::Mat4x4& m) {
			m_A = m * baseA;
			m_B = m * baseB;
			m_C = m * baseC;
		}
		TRIANGLE(){}
		TRIANGLE(const bsm::Float3& baseA, const bsm::Float3& baseB, const bsm::Float3& baseC,
			const bsm::Mat4x4& m) {
			Set(baseA,baseB,baseC,m);
		}
		bool IsValid() {
			if (m_A == m_B) {
				return false;
			}
			if (m_A == m_C) {
				return false;
			}
			if (m_B == m_C) {
				return false;
			}
			return true;
		}
		bsm::Float3 GetNormal()const {
			bsm::Float3 Ret =
				bsm::cross(
					m_B - m_A,
					m_C - m_A
				);
			Ret.normalize();
			return Ret;
		}
		PLANE GetPLANE() const {
			//3�_���g���Ėʂ��쐬
			PLANE ret(m_A, m_B, m_C);
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ݍ���XZ��`�𓾂�
		@return	XZ��`
		*/
		//--------------------------------------------------------------------------------------
		RECT_XZ GetWrappedRECT_XZ() const {
			RECT_XZ ret;
			POINT_XZ P1(m_A);
			POINT_XZ P2(m_B);
			POINT_XZ P3(m_C);

			ret.m_Left = P1.m_X;
			if (ret.m_Left < P2.m_X) {
				ret.m_Left = P2.m_X;
			}
			if (ret.m_Left < P3.m_X) {
				ret.m_Left = P3.m_X;
			}

			ret.m_Right = P1.m_X;
			if (ret.m_Right > P2.m_X) {
				ret.m_Right = P2.m_X;
			}
			if (ret.m_Right > P3.m_X) {
				ret.m_Right = P3.m_X;
			}

			ret.m_Near = P1.m_Z;
			if (ret.m_Near < P2.m_Z) {
				ret.m_Near = P2.m_Z;
			}
			if (ret.m_Near < P3.m_Z) {
				ret.m_Near = P3.m_Z;
			}

			ret.m_Far = P1.m_Z;
			if (ret.m_Far > P2.m_Z) {
				ret.m_Far = P2.m_Z;
			}
			if (ret.m_Far > P3.m_Z) {
				ret.m_Far = P3.m_Z;
			}
			return ret;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ݍ���AABB�𓾂�
		@return	AABB
		*/
		//--------------------------------------------------------------------------------------
		AABB GetWrappedAABB() const {
			AABB ret;
			ret.m_Min = m_A;
			ret.m_Max = m_A;
			for (int i = 0; i < 3; i++) {
				if (m_B[i] < ret.m_Min[i]) {
					ret.m_Min[i] = m_B[i];
				}
				if (m_B[i] > ret.m_Max[i]) {
					ret.m_Max[i] = m_B[i];
				}
			}
			for (int i = 0; i < 3; i++) {
				if (m_C[i] < ret.m_Min[i]) {
					ret.m_Min[i] = m_C[i];
				}
				if (m_C[i] > ret.m_Max[i]) {
					ret.m_Max[i] = m_C[i];
				}
			}
			return ret;
		}

	};


	//--------------------------------------------------------------------------------------
	///	�Փ˔���Ɏg�p����RECT
	//--------------------------------------------------------------------------------------
	struct COLRECT {
		bsm::Float3 m_Center;     //���S�_�̍��W
		bsm::Float3 m_Rot[2];  //XY �̊e���W���̌X����\�������x�N�g��
		float m_UVec[2];     //XY���W���ɉ����������̔����i���S�_����ʂ܂ł̒����j
		float m_BaseXSize;	//���쎞�̃T�C�YX�i�e��v�Z�Ɏg���j
		float m_BaseYSize;	//���쎞�̃T�C�YY�i�e��v�Z�Ɏg���j
		bsm::Mat4x4 m_Matrix;	//�s��i�e��v�Z�Ɏg���j
		bsm::Float3 m_Vertex[4];	//�s��ɂ���ĕϊ����ꂽ���_(�e��v�Z�Ɏg��)
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^<br />
		���̊�{�傫���A�c�̊�{�傫�����ƕϊ��s���^����RECT������������<br />
		COLRECT�̓f�t�H���g�R���X�g���N�^�͂Ȃ��B
		@param[in]	BaseXSize	�������̑傫��
		@param[in]	BaseYSize	�c�����̑傫��
		@param[in]	Matrix	�ϊ��s��
		*/
		//--------------------------------------------------------------------------------------
		COLRECT(float BaseXSize, float BaseYSize, const bsm::Mat4x4& Matrix):
			m_BaseXSize(BaseXSize),
			m_BaseYSize(BaseYSize),
			m_Matrix(Matrix)
			{
			m_Center.x = Matrix._41;
			m_Center.y = Matrix._42;
			m_Center.z = Matrix._43;
			bsm::Float3 VecX(Matrix._11, Matrix._12, Matrix._13);
			bsm::Float3 VecY(Matrix._21, Matrix._22, Matrix._23);
			m_UVec[0] = BaseXSize * bsm::length(VecX) * 0.5f;
			m_UVec[1] = BaseYSize * bsm::length(VecY) * 0.5f;
			//��]�𓾂�
			m_Rot[0] = bsm::normalize(VecX);
			m_Rot[1] = bsm::normalize(VecY);

			//���_�̐ݒ�
			float HalfX = m_BaseXSize * 0.5f;
			float HalfY = m_BaseYSize * 0.5f;
			m_Vertex[0] = bsm::Float3(-HalfX, HalfY, 0);
			m_Vertex[1] = bsm::Float3(HalfX, HalfY, 0);
			m_Vertex[2] = bsm::Float3(-HalfX, -HalfY, 0);
			m_Vertex[3] = bsm::Float3(HalfX, -HalfY, 0);
			for (auto& v : m_Vertex) {
				v *= m_Matrix;
			}
		}
		PLANE GetPLANE() const {
			//�\�ʏ��3�̓_���g����PLANE���쐬
			//1�ڂ̓_�͒��S
			bsm::Float3 Point0 = m_Center;
			float MakedHalfX = m_BaseXSize * 0.5f;
			float MakedHalfY = m_BaseYSize * 0.5f;
			//2�ڂ�-0.5,-0.5,0�̓_�����[���h�ϊ���������
			bsm::Float3 Point1(-MakedHalfX, -MakedHalfY,0);
			Point1 = m_Matrix * Point1;
			//3�ڂ�-0.5,0.5,0�̓_�����[���h�ϊ���������
			bsm::Float3 Point2(MakedHalfX, -MakedHalfY, 0);
			Point2 = m_Matrix * Point2;
			//3�_���g���Ėʂ��쐬
			PLANE ret(Point0, Point1, Point2);
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ݍ���XZ��`�𓾂�
		@return	XZ��`
		*/
		//--------------------------------------------------------------------------------------
		RECT_XZ GetWrappedRECT_XZ() const {
			RECT_XZ ret;

			POINT_XZ P1(m_Vertex[0].x, m_Vertex[0].z);
			POINT_XZ P2(m_Vertex[1].x, m_Vertex[1].z);
			POINT_XZ P3(m_Vertex[2].x, m_Vertex[2].z);
			POINT_XZ P4(m_Vertex[3].x, m_Vertex[3].z);

			ret.m_Left = P1.m_X;
			if (ret.m_Left < P2.m_X) {
				ret.m_Left = P2.m_X;
			}
			if (ret.m_Left < P3.m_X) {
				ret.m_Left = P3.m_X;
			}
			if (ret.m_Left < P4.m_X) {
				ret.m_Left = P4.m_X;
			}


			ret.m_Right = P1.m_X;
			if (ret.m_Right > P2.m_X) {
				ret.m_Right = P2.m_X;
			}
			if (ret.m_Right > P3.m_X) {
				ret.m_Right = P3.m_X;
			}
			if (ret.m_Right > P4.m_X) {
				ret.m_Right = P4.m_X;
			}


			ret.m_Near = P1.m_Z;
			if (ret.m_Near < P2.m_Z) {
				ret.m_Near = P2.m_Z;
			}
			if (ret.m_Near < P3.m_Z) {
				ret.m_Near = P3.m_Z;
			}
			if (ret.m_Near < P4.m_Z) {
				ret.m_Near = P4.m_Z;
			}

			ret.m_Far = P1.m_Z;
			if (ret.m_Far > P2.m_Z) {
				ret.m_Far = P2.m_Z;
			}
			if (ret.m_Far > P3.m_Z) {
				ret.m_Far = P3.m_Z;
			}
			if (ret.m_Far > P4.m_Z) {
				ret.m_Far = P4.m_Z;
			}
			return ret;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ݍ���SPHERE�𓾂�
		@return	SPHERE
		*/
		//--------------------------------------------------------------------------------------
		SPHERE GetWrappedSPHERE() const {
			SPHERE sp;
			auto Size = bsm::length(m_Vertex[1] - m_Vertex[2]);
			Size *= 0.5f;
			sp.m_Radius = Size;
			sp.m_Center = m_Center;
			return sp;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ݍ���AABB�𓾂�
		@return	AABB
		*/
		//--------------------------------------------------------------------------------------
		AABB GetWrappedAABB() const {
			AABB ret;
			ret.m_Min = m_Vertex[0];
			ret.m_Max = m_Vertex[0];
			for (int i = 1; i < 4; i++) {
				if (ret.m_Min.x > m_Vertex[i].x) {
					ret.m_Min.x = m_Vertex[i].x;
				}
				if (ret.m_Max.x < m_Vertex[i].x) {
					ret.m_Max.x = m_Vertex[i].x;
				}

				if (ret.m_Min.y > m_Vertex[i].y) {
					ret.m_Min.y = m_Vertex[i].y;
				}
				if (ret.m_Max.y < m_Vertex[i].y) {
					ret.m_Max.y = m_Vertex[i].y;
				}

				if (ret.m_Min.z > m_Vertex[i].z) {
					ret.m_Min.z = m_Vertex[i].z;
				}
				if (ret.m_Max.z < m_Vertex[i].z) {
					ret.m_Max.z = m_Vertex[i].z;
				}
			}
			return ret;
		}
	};

#define HITTEST_INTERVAL_EPSILON 0.0005f

	//--------------------------------------------------------------------------------------
	///	�Փ˔���p�̃��[�e�B���e�B�\����
	//--------------------------------------------------------------------------------------
	struct HitTest{
		HitTest()
		{}
		~HitTest(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	float�^�̃N�����v
		@param[in]	n	�N�����v����l
		@param[in]	min	�ŏ��l
		@param[in]	max	�ő�l
		@return	�N�����v���ꂽfloat
		*/
		//--------------------------------------------------------------------------------------
		static float floatClamp(float n,float min,float max){
			if(n < min) return min;
			if(n > max) return max;
			return n;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	chk��min_v�����max_v�Ɣ�r���čŏ��܂��͍ő�̃x�N�g����ݒ肷��
		@param[in]	chk	�`�F�b�N����x�N�g��
		@param[in,out]	min_v	��r���čŏ��l��ݒ肷��x�N�g��
		@param[in,out]	max_v	��r���čő�l��ݒ肷��x�N�g��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void ChkSetMinMax(const bsm::Float3& chk, bsm::Float3& min_v, bsm::Float3& max_v) {
			if (min_v.x > chk.x) {
				min_v.x = chk.x;
			}
			if (max_v.x < chk.x) {
				max_v.x = chk.x;
			}
			if (min_v.y > chk.y) {
				min_v.y = chk.y;
			}
			if (max_v.y < chk.y) {
				max_v.y = chk.y;
			}
			if (min_v.z > chk.z) {
				min_v.z = chk.z;
			}
			if (max_v.z < chk.z) {
				max_v.z = chk.z;
			}
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�_���ʂ̗����ɂ��邩�ǂ�������
		@param[in]	p	���؂���_
		@param[in]	pl	��
		@return	�����Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		static bool InsidePtPlane(const bsm::Float3& p, const PLANE& pl){
			float dist = bsm::dot(p,pl.m_Normal) - pl.m_DotValue;
			return dist < 0.0f;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�������m�̍ŋߐړ_���v�Z����i�J�v�Z�����m�̔���Ɏg�p�j
		@param[in]	p1	�J�v�Z��1�̐����J�n�_
		@param[in]	q1	�J�v�Z��1�̐����I���_
		@param[in]	p2	�J�v�Z��2�̐����J�n�_
		@param[in]	q2	�J�v�Z��2�̐����I���_
		@param[out]	s	����1�̍ŋߐړ_�̃p�����[�^
		@param[out]	t	����2�̍ŋߐړ_�̃p�����[�^
		@param[out]	c1	�ŋߐړ_1
		@param[out]	c2	�ŋߐړ_2
		@return	�ŋߐړ_c1,c2���v�Z���A���̃p�����[�^s,t���v�Z���������m�̋����̕�����Ԃ�
		*/
		//--------------------------------------------------------------------------------------
		static float ClosestPtSegmentSegment(
			const bsm::Float3& p1,const bsm::Float3& q1,const bsm::Float3& p2,const bsm::Float3& q2,
			float& s,float& t,bsm::Float3& c1,bsm::Float3& c2){
			const float EPSILON = 1.175494e-37f;
			bsm::Float3 d1 = q1 - p1;
			bsm::Float3 d2 = q2 - p2;
			bsm::Float3 r = p1 - p2;
			float a = bsm::dot(d1,d1);
			float e = bsm::dot(d2,d2);
			float f = bsm::dot(d2,r);

			if(f <= EPSILON && e <= EPSILON){
				s = t = 0.0f;
				c1 = p1;
				c2 = p2;
				return bsm::dot(c1 - c2,c1 - c2);
			}
			if(a <= EPSILON){
				s = 0.0f;
				t = f / e;
				t = floatClamp(t,0.0f,1.0f);
			}
			else{
				float c = bsm::dot(d1,r);
				if(e <= EPSILON){
					t = 0.0f;
					s = floatClamp(-c / a,0.0f,1.0f);
				}
				else{
					float b = bsm::dot(d1,d2);
					float denom = a * e - b * b;
					if(denom != 0.0f){
						s = floatClamp((b * f - c * e) / denom,0.0f,1.0f);
					}
					else{
						s = 0.0f;
					}
					t = (b * s + f) / e;
					if(t < 0.0f){
						t = 0.0f;
						s = floatClamp(-c / a,0.0f,1.0f);
					}
					else if(t > 1.0f){
						t = 1.0f;
						s = floatClamp((b - c) / a,0.0f,1.0f);
					}
				}
			}
			c1 = p1 + d1 * s;
			c2 = p2 + d2 * t;
			return bsm::dot(c1 - c2,c1 - c2);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�_c�Ɛ���ab�̋����̕�����Ԃ�
		@param[in]	a	�����J�n�_
		@param[in]	b	�����I���_
		@param[in]	c	�v�Z�_
		@return	�_c�Ɛ���ab�̋����̕���
		*/
		//--------------------------------------------------------------------------------------
		static float SqDistPointSegment(const bsm::Float3& a, const bsm::Float3& b, const bsm::Float3& c){
			bsm::Float3 ab = b - a;
			bsm::Float3 ac = c - a;
			bsm::Float3 bc = c - b;
			float e = bsm::dot(ac,ab);
			if(e <= 0.0f){
				return bsm::dot(ac,ac);
			}
			float f = bsm::dot(ab,ab);
			if(e >= f){
				return bsm::dot(bc,bc);
			}
			return bsm::dot(ac,ac) - e * e / f;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�_c�Ɛ���ab�̍ŋߐړ_��Ԃ�
		@param[in]	c	���ƂɂȂ�_
		@param[in]	a	�����J�n�_
		@param[in]	b	�����I���_
		@param[out]	t	�ŋߐړ_�̐�����̈ʒu�̊����̖߂�l
		@param[out]	d	�ŋߐړ_�̖߂�l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void ClosetPtPointSegment(const bsm::Float3& c,
			const bsm::Float3& a,const bsm::Float3& b,
			float& t,bsm::Float3& d){
			bsm::Float3 ab = b - a;
			t = bsm::dot(c - a,ab) / bsm::dot(ab,ab);
			if(t < 0.0f){
				t = 0.0f;
			}
			if(t > 1.0f){
				t = 1.0f;
			}
			d = a + (ab * t);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�_c�ƃv���[���̍ŋߐړ_��Ԃ�
		@param[in]	c	���ƂɂȂ�_
		@param[in]	c	���ƂɂȂ�_
		@param[out]	d	�ŋߐړ_�̖߂�l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void ClosetPtPointPlane(const bsm::Float3& c,
			const PLANE& pl,bsm::Float3& d) {
			float t = (bsm::dot(pl.m_Normal, c) - pl.m_DotValue) / bsm::dot(pl.m_Normal, pl.m_Normal);
			d = c - pl.m_Normal * t;
		}

		static SPHERE SphereEnclosingSphere(const SPHERE& s0, const SPHERE& s1) {
			const float EPSILON = 1.175494e-37f;
			SPHERE s;
			bsm::Float3 d = s1.m_Center - s0.m_Center;
			float dist2 = bsm::dot(d,d);
			float f = s1.m_Radius - s0.m_Radius;
			bsm::Float4 vec0(f);
			vec0 = XMVectorPow(vec0, bsm::Float4(2.0f));
			if (vec0.x >= dist2) {
				if (s1.m_Radius >= s0.m_Radius) {
					s = s1;
				}
				else {
					s = s0;
				}
			}
			else {
				bsm::Float4 vec(dist2);
				vec = XMVectorSqrt(vec);
				float dist = vec.x;
				s.m_Radius = (dist + s0.m_Radius + s1.m_Radius) * 0.5f;
				s.m_Center = s0.m_Center;
				if (dist > EPSILON) {
					s.m_Center += ((s.m_Radius - s0.m_Radius) / dist) * d;
				}

			}
			return s;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	point���猩���O�p�`�̍ŋߐړ_�𓾂�
		@param[in]	point	��_
		@param[in]	t	�O�p�`
		@param[out]	retvec	�ŋߐړ_��Ԃ��Q��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void ClosestPtPointTriangle(const bsm::Float3& point, const TRIANGLE& t, bsm::Float3& retvec) {
			bsm::Float3 ab = t.m_B - t.m_A;
			bsm::Float3 ac = t.m_C - t.m_A;
			bsm::Float3 ap = point - t.m_A;
			float d1 = bsm::dot(ab, ap);
			float d2 = bsm::dot(ac, ap);
			if (d1 <= 0.0f && d2 <= 0.0f) {
				retvec = t.m_A;
				return;
			}
			bsm::Float3 bp = point - t.m_B;
			float d3 = bsm::dot(ab, bp);
			float d4 = bsm::dot(ac, bp);
			if (d3 >= 0.0f && d4 <= d3) {
				retvec = t.m_B;
				return;
			}
			float vc = d1 * d4 - d3 * d2;
			if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
				float v = d1 / (d1 - d3);
				retvec = v * ab + t.m_A;
				return;
			}

			bsm::Float3 cp = point - t.m_C;
			float d5 = bsm::dot(ab, cp);
			float d6 = bsm::dot(ac, cp);
			if (d6 >= 0.0f && d5 <= d6) {
				retvec = t.m_C;
				return;
			}
			float vb = d5 * d2 - d1 * d6;

			if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
				float w = d2 / (d2 - d6);
				retvec = w * ac + t.m_A;
				return;
			}

			float va = d3 * d6 - d5 * d4;
			if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
				float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
				retvec = w * (t.m_C - t.m_B) + t.m_B;
				return;
			}
			float denon = 1.0f / (va + vb + vc);
			float v = vb * denon;
			float w = vc * denon;
			retvec = ab * v + ac * w + t.m_A;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	point���猩��OBB�̍ŋߐړ_�𓾂�
		@param[in]	point	��_
		@param[in]	obb	OBB
		@param[out]	retvec	�ŋߐړ_��Ԃ��Q��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void ClosestPtPointOBB(const bsm::Float3& point, const OBB& obb, bsm::Float3& retvec) {
			bsm::Float3 d = point - obb.m_Center;
			retvec = obb.m_Center;
			float dist;
			for (int i = 0; i < 3; i++)
			{
				dist = bsm::dot(d, obb.m_Rot[i]);
				if (dist > obb.m_Size[i])
				{
					dist = obb.m_Size[i];
				}
				if (dist < -obb.m_Size[i])
				{
					dist = -obb.m_Size[i];
				}
				retvec += dist * obb.m_Rot[i];
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�_��AABB�Ƃ̍ŋߐړ_�𓾂�
		@param[in]	p	�_
		@param[in]	b	AABB
		@param[out]	retvec	�ŋߐړ_����������Q��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void ClosestPtPointAABB(const bsm::Float3& p, const AABB& b,
			bsm::Float3& retvec) {
			for (int i = 0; i < 3; i++) {
				float v = p[i];
				if (v < b.m_Min[i]) {
					v = b.m_Min[i];
				}
				if (v > b.m_Max[i]) {
					v = b.m_Max[i];
				}
				retvec[i] = v;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�_��AABB�Ƃ̋����̕����𑪂�
		@param[in]	p	�_
		@param[in]	b	AABB
		@return	�_��AABB�Ƃ̋����̕���
		*/
		//--------------------------------------------------------------------------------------
		static float SqDistPointAABB(const bsm::Float3& p, const AABB& b) {
			float sqDist = 0.0f;
			for (int i = 0; i < 3; i++) {
				float v = p[i];
				if (v < b.m_Min[i]) {
					sqDist += (b.m_Min[i] - v) * (b.m_Min[i] - v);
				}
				if (v > b.m_Max[i]) {
					sqDist += (v - b.m_Max[i]) * (v - b.m_Max[i]);
				}
			}
			return sqDist;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�_��COLRECT�Ƃ̍ŋߐړ_�𓾂�
		@param[in]	p	�_
		@param[in]	rect	COLRECT
		@param[out]	retvec	�ŋߐړ_����������Q��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void ClosestPtPointCOLRECT(const bsm::Float3& point, const COLRECT& rect,
			bsm::Float3& retvec) {
			bsm::Float3 d = point - rect.m_Center;
			retvec = rect.m_Center;
			for (int i = 0; i < 2; i++) {
				float dist = bsm::dot(d, rect.m_Rot[i]);
				if (dist > rect.m_UVec[i]) {
					dist = rect.m_UVec[i];
				}
				if (dist < -rect.m_UVec[i]) {
					dist = -rect.m_UVec[i];
				}
				retvec += rect.m_Rot[i] * dist;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���ƃJ�v�Z���Ƃ̏Փ˔���
		@param[in]	sp	��
		@param[in]	cap	�J�v�Z��
		@param[out]	d	�ŋߐړ_�̖߂�l
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_CAPSULE(const SPHERE& sp, const CAPSULE& cap, bsm::Float3& d){
			//�܂��ŋߐړ_��ݒ�
			float t;
			bsm::Float3 ClosetPoint;
			ClosetPtPointSegment(sp.m_Center, cap.m_PointBottom, cap.m_PointTop, t, ClosetPoint);
			//ClosetPoint�́A�J�v�Z��������̓_�ł���
			bsm::Float3 Normal = sp.m_Center - ClosetPoint;
			Normal.normalize();
			Normal *= cap.m_Radius;
			//�ŋߐړ_��ݒ�
			d = ClosetPoint + Normal;
			//�Փ˂��Ă��邩����
			float dist2 = SqDistPointSegment(cap.m_PointBottom,cap.m_PointTop,sp.m_Center);
			float radius = sp.m_Radius + cap.m_Radius;
			return dist2 <= radius * radius;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���ƕ��ʂƂ̏Փ˔���
		@param[in]	sp	��
		@param[in]	pl	����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_PLANE(const SPHERE& sp, const PLANE& pl) {
			float dist = bsm::dot(sp.m_Center, pl.m_Normal) - pl.m_DotValue;
			return dist <= sp.m_Radius;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���ƃV�����_�[�Ƃ̏Փ˔���
		@param[in]	sp	��
		@param[in]	cy	�V�����_�[
		@param[out]	d	�ŋߐړ_�̖߂�l
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_CYLINDER(const SPHERE& sp, const CYLINDER& cy, bsm::Float3& d) {
			CAPSULE cap;
			cap.m_PointBottom = cy.m_PointBottom;
			cap.m_PointTop = cy.m_PointTop;
			cap.m_Radius = cy.m_Radius;
			//�܂��J�v�Z�����쐬���Ĕ���
			if (!SPHERE_CAPSULE(sp, cap, d)) {
				return false;
			}
			bsm::Float3 CenterPole = cap.m_PointTop - cap.m_PointBottom;
			float CenterPoleLen = CenterPole.length();
			CenterPole.normalize();
			float f = bsm::dot(CenterPole, d - cap.m_PointBottom);
			if (f >= 0 && f <= CenterPoleLen) {
				return true;
			}

			bsm::Float3 t0(0, 0.5f, 0);
			bsm::Float3 t1(0.0f, 0.5f, 1.0);
			bsm::Float3 t2(1.0, 0.5f, 0.0f);

			bsm::Float3 b0(0, -0.5f, 0);
			bsm::Float3 b1(1.0f, -0.5f, 0.0);
			bsm::Float3 b2(0.0, -0.5f, 1.0f);

			bsm::Mat4x4 cyWorld = cy.GetMatrix();
			t0 *= cyWorld;
			t1 *= cyWorld;
			t2 *= cyWorld;

			b0 *= cyWorld;
			b1 *= cyWorld;
			b2 *= cyWorld;

			PLANE topPlane(t0,t1,t2);
			PLANE bottomPlane(b0, b1, b2);

			bool tb = SPHERE_PLANE(sp, topPlane);
			bool bb = SPHERE_PLANE(sp, bottomPlane);
			if (tb && bb) {
				bsm::Float3 topV,bottomV;
				ClosetPtPointPlane(sp.m_Center, topPlane, topV);
				ClosetPtPointPlane(sp.m_Center, bottomPlane, bottomV);
				if (bsm::lengthSqr(sp.m_Center - topV) <= bsm::lengthSqr(sp.m_Center - bottomV)) {
					d = topV;
				}
				else {
					d = bottomV;
				}
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�J�v�Z���ƃJ�v�Z���Ƃ̏Փ˔���
		@param[in]	cap1	�J�v�Z��1
		@param[in]	cap2	�J�v�Z��2
		@param[out]	retvec1	�J�v�Z��1�̍ŋߐړ_��Ԃ��Q��
		@param[out]	retvec2	�J�v�Z��2�̍ŋߐړ_��Ԃ��Q��
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CAPSULE_CAPSULE(
			const CAPSULE& cap1,
			const CAPSULE& cap2,
			bsm::Float3& retvec1,
			bsm::Float3& retvec2){
			float s,t;
			float dist2 = ClosestPtSegmentSegment(cap1.m_PointBottom,cap1.m_PointTop,cap2.m_PointBottom,cap2.m_PointTop,
				s,t,retvec1,retvec2);
			float radius = cap1.m_Radius + cap2.m_Radius;
			return dist2 <= radius * radius;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���Ƌ��Ƃ̏Փ˔���
		@param[in]	sp1	��1
		@param[in]	sp2	��2
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_SPHERE(const SPHERE& sp1, const SPHERE& sp2){
			bsm::Float3 d = sp1.m_Center - sp2.m_Center;
			float dist2 = bsm::dot(d,d);
			float radiussum = sp1.m_Radius + sp2.m_Radius;
			return dist2 <= radiussum * radiussum;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���Ƌ��z��Ƃ̏Փ˔���
		@param[in]	sp1	��1
		@param[in]	sparr	���z��
		@param[out]	HitIndex	�Փ˂��Ă����ꍇ�̔z��̃C���f�b�N�X
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_SPHERE_ARR(const SPHERE& sp1, const SPHERE_ARR& sparr, size_t& HitIndex){
			size_t sz = sparr.m_SphereArr.size();
			for(size_t i = 0;i < sz;i++){
				if(SPHERE_SPHERE(sp1,sparr.m_SphereArr[i])){
					HitIndex = i;
					return true;
				}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���z��Ƌ��z��Ƃ̏Փ˔���
		@param[in]	srcarr	���z��src
		@param[in]	destarr	���z��dest
		@param[out]	SrcHitIndex	�Փ˂��Ă����ꍇ��Src�z��̃C���f�b�N�X
		@param[out]	DestHitIndex	�Փ˂��Ă����ꍇ��Dest�z��̃C���f�b�N�X
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_ARR_SPHERE_ARR(const SPHERE_ARR& srcarr, const SPHERE_ARR& destarr,
			size_t& SrcHitIndex,size_t& DestHitIndex){
			size_t SrcSz = srcarr.m_SphereArr.size();
			size_t DestSz = destarr.m_SphereArr.size();
			for(size_t i = 0;i < SrcSz;i++){
				for(size_t j = 0;j < DestSz;j++){
					if(SPHERE_SPHERE(srcarr.m_SphereArr[i],destarr.m_SphereArr[j])){
						SrcHitIndex = i;
						DestHitIndex = j;
						return true;
					}
				}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	����AABB�Ƃ̏Փ˔���
		@param[in]	sp	��
		@param[in]	b	AABB
		@param[out]	retvec	�ŋߐړ_����������Q��
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_AABB(const SPHERE& sp, const AABB& b,
			bsm::Float3& retvec){
				//�ŋߐړ_�𓾂�
				ClosestPtPointAABB(sp.m_Center,b,retvec);
				//�����̕��������̔��a�̕������Ȃ�true
				if(SqDistPointAABB(sp.m_Center,b) <= (sp.m_Radius * sp.m_Radius)){
					return true;
				}
				return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	AABB��AABB�Ƃ̏Փ˔���
		@param[in]	a	AABB1
		@param[in]	b	AABB2
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool AABB_AABB(const AABB& a, const AABB& b){
			if(a.m_Max.x < b.m_Min.x || a.m_Min.x > b.m_Max.x)
				return false;
			if(a.m_Max.y < b.m_Min.y || a.m_Min.y > b.m_Max.y)
				return false;
			if(a.m_Max.z < b.m_Min.z || a.m_Min.z > b.m_Max.z)
				return false;
			return true;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	AABB��AABB�Ƃ̏Փ˔���(min��������max�������l��������Փ˂��ĂȂ�)
		@param[in]	a	AABB1
		@param[in]	b	AABB2
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool AABB_AABB_NOT_EQUAL(const AABB& a, const AABB& b) {
			if (a.m_Max.x <= b.m_Min.x || a.m_Min.x >= b.m_Max.x)
				return false;
			if (a.m_Max.y <= b.m_Min.y || a.m_Min.y >= b.m_Max.y)
				return false;
			if (a.m_Max.z <= b.m_Min.z || a.m_Min.z >= b.m_Max.z)
				return false;
			return true;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	AABB a��AABB ���Ɋ��S�Ɋ܂܂�邩����
		@param[in]	a	AABB1
		@param[in]	b	AABB2
		@return	���S�Ɋ܂܂���true
		*/
		//--------------------------------------------------------------------------------------
		static bool AABB_IN_AABB(const AABB& a, const AABB& b){
			if(a.m_Min.x < b.m_Min.x || a.m_Max.x > b.m_Max.x)
				return false;
			if(a.m_Min.y < b.m_Min.y || a.m_Max.y > b.m_Max.y)
				return false;
			if(a.m_Min.z < b.m_Min.z || a.m_Max.z > b.m_Max.z)
				return false;
			return true;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	AABB a��AABB ������AABB��Ԃ�
		@param[in]	a	AABB1
		@param[in]	b	AABB2
		@return	���S�Ɋ܂܂���true
		*/
		//--------------------------------------------------------------------------------------
		static AABB AABB_OR_AABB(const AABB& a, const AABB& b) {
			AABB ret;
			ret.m_Min.x = (a.m_Min.x <= b.m_Min.x) ? a.m_Min.x : b.m_Min.x;
			ret.m_Min.y = (a.m_Min.y <= b.m_Min.y) ? a.m_Min.y : b.m_Min.y;
			ret.m_Min.z = (a.m_Min.z <= b.m_Min.z) ? a.m_Min.z : b.m_Min.z;

			ret.m_Max.x = (a.m_Max.x >= b.m_Max.x) ? a.m_Max.x : b.m_Max.x;
			ret.m_Max.y = (a.m_Max.y >= b.m_Max.y) ? a.m_Max.y : b.m_Max.y;
			ret.m_Max.z = (a.m_Max.z >= b.m_Max.z) ? a.m_Max.z : b.m_Max.z;

			return ret;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	����OBB�Ƃ̏Փ˔���
		@param[in]	sp	��
		@param[in]	obb	OBB
		@param[out]	retvec	�ŋߐړ_����������Q��
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_OBB(const SPHERE& sp, const OBB& obb, bsm::Float3& retvec){
			ClosestPtPointOBB(sp.m_Center,obb,retvec);
			bsm::Float3 v = retvec - sp.m_Center;
			return bsm::dot(v,v) <= sp.m_Radius * sp.m_Radius;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OBB��OBB�Ƃ̏Փ˔���
		@param[in]	obb1	OBB1
		@param[in]	obb2	OBB2
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool OBB_OBB(const OBB& obb1, const OBB& obb2) {
			const float EPSILON = 1.175494e-37f;
			float R[3][3], AbsR[3][3];
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					R[i][j] = bsm::dot(obb1.m_Rot[i], obb2.m_Rot[j]);
					AbsR[i][j] = fabsf(R[i][j]) + EPSILON;
				}
			}
			bsm::Float3 t = obb2.m_Center - obb1.m_Center;
			t = bsm::Float3(bsm::dot(t, obb1.m_Rot[0]),
				bsm::dot(t, obb1.m_Rot[1]),
				bsm::dot(t, obb1.m_Rot[2]));
			//��L=A0, L=A1, L=A2����
			float ra, rb;
			for (int i = 0; i < 3; i++) {
				ra = obb1.m_Size[i];
				rb = obb2.m_Size[0] * AbsR[i][0] + obb2.m_Size[1] * AbsR[i][1] + obb2.m_Size[2] * AbsR[i][2];
				if (fabsf(t[i]) > ra + rb) {
					return false;
				}
			}
			//��L=B0, L=B1, L=B2����
			for (int i = 0; i < 3; i++) {
				ra = obb1.m_Size[0] * AbsR[0][i] + obb1.m_Size[1] * AbsR[1][i] + obb1.m_Size[2] * AbsR[2][i];
				rb = obb2.m_Size[i];
				if (fabsf(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb) {
					return false;
				}
			}
			//��L=A0 X B0����
			ra = obb1.m_Size[1] * AbsR[2][0] + obb1.m_Size[2] * AbsR[1][0];
			rb = obb2.m_Size[1] * AbsR[0][2] + obb2.m_Size[2] * AbsR[0][1];
			if (fabsf(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) {
				return false;
			}
			//��L=A0 X B1����
			ra = obb1.m_Size[1] * AbsR[2][1] + obb1.m_Size[2] * AbsR[1][1];
			rb = obb2.m_Size[0] * AbsR[0][2] + obb2.m_Size[2] * AbsR[0][0];
			if (fabsf(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb) {
				return false;
			}
			//��L=A0 X B2����
			ra = obb1.m_Size[1] * AbsR[2][2] + obb1.m_Size[2] * AbsR[1][2];
			rb = obb2.m_Size[0] * AbsR[0][1] + obb2.m_Size[1] * AbsR[0][0];
			if (fabsf(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb) {
				return false;
			}
			//��L=A1 X B0����
			ra = obb1.m_Size[0] * AbsR[2][0] + obb1.m_Size[2] * AbsR[0][0];
			rb = obb2.m_Size[1] * AbsR[1][2] + obb2.m_Size[2] * AbsR[1][1];
			if (fabsf(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb) {
				return false;
			}
			//��L=A1 X B1����
			ra = obb1.m_Size[0] * AbsR[2][1] + obb1.m_Size[2] * AbsR[0][1];
			rb = obb2.m_Size[0] * AbsR[1][2] + obb2.m_Size[2] * AbsR[1][0];
			if (fabsf(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb) {
				return false;
			}
			//��L=A1 X B2����
			ra = obb1.m_Size[0] * AbsR[2][2] + obb1.m_Size[2] * AbsR[0][2];
			rb = obb2.m_Size[0] * AbsR[1][1] + obb2.m_Size[1] * AbsR[1][0];
			if (fabsf(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb) {
				return false;
			}
			//��L=A2 X B0����
			ra = obb1.m_Size[0] * AbsR[1][0] + obb1.m_Size[1] * AbsR[0][0];
			rb = obb2.m_Size[1] * AbsR[2][2] + obb2.m_Size[2] * AbsR[2][1];
			if (fabsf(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb) {
				return false;
			}
			//��L=A2 X B1����
			ra = obb1.m_Size[0] * AbsR[1][1] + obb1.m_Size[1] * AbsR[0][1];
			rb = obb2.m_Size[0] * AbsR[2][2] + obb2.m_Size[2] * AbsR[2][0];
			if (fabsf(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb) {
				return false;
			}
			//��L=A2 X B2����
			ra = obb1.m_Size[0] * AbsR[1][2] + obb1.m_Size[1] * AbsR[0][2];
			rb = obb2.m_Size[0] * AbsR[2][1] + obb2.m_Size[1] * AbsR[2][0];
			if (fabsf(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) {
				return false;
			}
			return true;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	����ab�Ɩʔ���
		@param[in]	a	�����J�n�_
		@param[in]	b	�����I���_
		@param[in]	p	��
		@param[out]	t	�ŋߐړ_�̐�����̈ʒu�̊����̖߂�l
		@param[out]	q	�ŋߐړ_�̖߂�l
		@return	�������Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SEGMENT_PLANE(const bsm::Float3& a, const bsm::Float3& b,
			const PLANE& p,float& t,bsm::Float3& q){
			bsm::Float3 ab = b - a;
			t = (p.m_DotValue - bsm::dot(p.m_Normal,a)) / bsm::dot(p.m_Normal,ab);
			if(t >= 0.0f && t <= 1.0f){
				q = a + (ab * t);
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	������AABB�̃��C����
		@param[in]	p0	�����J�n�_
		@param[in]	p1	�����I���_
		@param[in]	b	AABB
		@return	�������Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SEGMENT_AABB(const bsm::Float3& p0, const bsm::Float3& p1, const AABB& b){
			const float EPSILON = 1.175494e-37f;
			bsm::Float3 c = (b.m_Min + b.m_Max) * 0.5f;
			bsm::Float3 e = b.m_Max - c;
			bsm::Float3 m = (p0 + p1) * 0.5f;
			bsm::Float3 d = p1 - m;
			m = m - c;
			float adx = fabsf(d.x);
			if(fabsf(m.x) > e.x + adx){
				return false;
			}
			float ady = fabsf(d.y);
			if(fabsf(m.y) > e.y + ady){
				return false;
			}
			float adz = fabsf(d.z);
			if(fabsf(m.z) > e.z + adz){
				return false;
			}
			adx += EPSILON;
			ady += EPSILON;
			adz += EPSILON;
			if(fabsf(m.y * d.z - m.z * d.y) > e.y * adz + e.z * ady){
				return false;
			}
			if(fabsf(m.z * d.x - m.x * d.z) > e.x * adz + e.z * adx){
				return false;
			}
			if(fabsf(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx){
				return false;
			}
			return true;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	������OBB�̃��C����
		@param[in]	p0	�����J�n�_
		@param[in]	p1	�����I���_
		@param[in]	obb	OBB
		@return	�������Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SEGMENT_OBB(const bsm::Float3& p0, const bsm::Float3& p1, const OBB& obb){
			const float EPSILON = 1.175494e-37f;

			bsm::Float3 m = (p0 + p1) * 0.5f;
			bsm::Float3 d = p1 - m;
			m = m - obb.m_Center;
			m = bsm::Float3(bsm::dot(obb.m_Rot[0], m), bsm::dot(obb.m_Rot[1], m), bsm::dot(obb.m_Rot[2], m));
			d = bsm::Float3(bsm::dot(obb.m_Rot[0], d), bsm::dot(obb.m_Rot[1], d), bsm::dot(obb.m_Rot[2], d));

			float adx = fabsf(d.x);
			if(fabsf(m.x) > obb.m_Size.x + adx) return false;
			float ady = fabsf(d.y);
			if(fabsf(m.y) > obb.m_Size.y + ady) return false;
			float adz = fabsf(d.z);
			if(fabsf(m.z) > obb.m_Size.z + adz) return false;
			adx += EPSILON; 
			ady += EPSILON; 
			adz += EPSILON;
        
			if(fabsf(m.y * d.z - m.z * d.y) > obb.m_Size.y * adz + obb.m_Size.z * ady ) return false;
			if(fabsf(m.z * d.x - m.x * d.z) > obb.m_Size.x * adz + obb.m_Size.z * adx ) return false;
			if(fabsf(m.x * d.y - m.y * d.x) > obb.m_Size.x * ady + obb.m_Size.y * adx ) return false;

			return true;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����Ƌ��̌���
		@param[in]	p	�����̌��_
		@param[in]	d	�����̌���
		@param[in]	s	��
		@param[in]	t	�����_����̋���
		@param[in]	q	�����_
		@return	�������Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool RAY_SPHERE(const bsm::Float3& p, const bsm::Float3& d,const SPHERE& s,
			float &t,bsm::Float3& q) {
			bsm::Float3 m = p - s.m_Center;
			float b = bsm::dot(m, d);
			float c = bsm::dot(m, m) - s.m_Radius * s.m_Radius;
			if (c > 0.0f && b > 0.0f) {
				return false;
			}
			float discr = b * b - c;
			if (discr < 0.0f) {
				return false;
			}
			bsm::Float3 VecDiscr(discr);
			VecDiscr = XMVectorSqrt(VecDiscr);
			t = -b - VecDiscr.x;
			if (t < 0.0f) {
				t = 0.0f;
			}
			q = d * t + p;
			return true;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	������SPHERE�̃��C����
		@param[in]	p0	�����J�n�_
		@param[in]	p1	�����I���_
		@param[in]	s	SPHERE
		@return	�������Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SEGMENT_SPHERE(const bsm::Float3& p0, const bsm::Float3& p1, const SPHERE& s) {
			bsm::Float3 norm = p1 - p0;
			norm.normalize();
			float t;
			bsm::Float3 q;
			if (RAY_SPHERE(p0, norm, s, t, q)) {
				if (t <= bsm::length(p1 - p0)) {
					return true;
				}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	������CYLINDER�̃��C����
		@param[in]	sa	�����J�n�_
		@param[in]	sb	�����I���_
		@param[in]	cy	CYLINDER
		@param[in]	t	�����_����̋���(0.0����1.0)
		@return	�������Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SEGMENT_CYLINDER(const bsm::Float3& sa, const bsm::Float3& sb, const CYLINDER& cy,
			float &t) {
			const float EPSILON = 1.175494e-37f;
			bsm::Float3 p = cy.m_PointBottom;
			bsm::Float3 q = cy.m_PointTop;
			float r = cy.m_Radius;
			bsm::Float3 d = q - p;
			bsm::Float3 m = sa - p;
			bsm::Float3 n = sb - sa;
			float md = bsm::dot(m, d);
			float nd = bsm::dot(n, d);
			float dd = bsm::dot(d, d);
			if (md < 0.0f && md + nd < 0.0f) {
				return false;
			}
			if (md > dd && md + nd > dd) {
				return false;
			}
			float nn = bsm::dot(n, n);
			float mn = bsm::dot(m, n);
			float a = dd * nn - nd * nd;
			float k = bsm::dot(m, m) - r * r;
			float c = dd * k - md * md;
			if (abs(a) < EPSILON) {
				if (c > 0.0f) {
					return false;
				}
				if (md < 0.0f) {
					t = -mn / nn;
				}
				else if (md > dd) {
					t = (nd - mn) / nn;
				}
				else {
					t = 0.0f;
				}
				return true;
			}
			float b = dd * mn - nd * md;
			float discr = b * b - a * c;
			if (discr < 0.0f) {
				return false;
			}
			bsm::Float3 VecDiscr(discr);
			VecDiscr = XMVectorSqrt(VecDiscr);
			t = (-b - VecDiscr.x) / a;
			if (t < 0.0f || t > 1.0f) {
				return false;
			}
			if (md + t * nd < 0.0f) {
				if (nd <= 0.0f) {
					return false;
				}
				t = -md / nd;
				return k + 2 * t * (mn + t * mn) <= 0.0f;
			}
			else if (md + t * nd > dd) {
				if (nd >= 0.0f) {
					return false;
				}
				t = (dd - md) / nd;
				return k + dd - 2 * md + t * (2 * (mn - nd) + t * nn) <= 0.0f;
			}
			return true;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	������CAPSULE�̃��C����
		@param[in]	p0	�����J�n�_
		@param[in]	p1	�����I���_
		@param[in]	cap	CAPSULE
		@return	�������Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SEGMENT_CAPSULE(const bsm::Float3& p0, const bsm::Float3& p1, const CAPSULE& cap) {
			bsm::Float3 normR = cap.m_PointTop - cap.m_PointBottom;
			normR.normalize();
			normR *= cap.m_Radius;
			//�J�v�Z���̔��������𑝂₵���V�����_�[���쐬
			CYLINDER more_cy(cap.m_Radius, cap.m_PointBottom - normR, cap.m_PointTop + normR);
			float t;
			if (SEGMENT_CYLINDER(p0, p1, more_cy, t)) {
				//���������̋����Q�쐬
				SPHERE sp0,sp1;
				sp0.m_Center = cap.m_PointBottom;
				sp0.m_Radius = cap.m_Radius;
				sp1.m_Center = cap.m_PointTop;
				sp1.m_Radius = cap.m_Radius;
				if (SEGMENT_SPHERE(p0, p1, sp0) || SEGMENT_SPHERE(p0, p1, sp1)) {
					return true;
				}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	����COLRECT�Ƃ̏Փ˔���
		@param[in]	sp	��
		@param[in]	rect	COLRECT
		@param[out]	retvec	�ŋߐړ_����������Q��
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_COLRECT(const SPHERE& sp, const COLRECT& rect, bsm::Float3& retvec) {
			//�l�p�`�Ƃ̍ŋߐړ_�𓾂�
			ClosestPtPointCOLRECT(sp.m_Center, rect, retvec);
			//�ŋߐړ_�����a�ȉ��Ȃ�Փ˂��Ă���
			if (bsm::length(sp.m_Center - retvec) <= sp.m_Radius) {
				return true;
			}
			return false;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�O�p�`�Ɛ����Ƃ̌�������
		@param[in]	p	�����̎n�_
		@param[in]	q	�����̏I�_
		@param[in]	tri	�O�p�`
		@param[out]	uvw	�����_����������Q��
		@param[out]	t	�����_�̐�����̊���
		@return	�������Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SEGMENT_TRIANGLE(
			const bsm::Float3& p,
			const bsm::Float3& q,
			const TRIANGLE& tri,
			bsm::Float3& uvw,
			float& t
		) {
			bsm::Float3 ab = tri.m_B - tri.m_A;
			bsm::Float3 ac = tri.m_C - tri.m_A;
			bsm::Float3 qp = p - q;
			bsm::Float3 n = bsm::cross(ab, ac);
			float d = bsm::dot(qp, n);
			if (d <= 0.0f) {
				return false;
			}
			bsm::Float3 ap = p - tri.m_A;
			t = bsm::dot(ap, n);
			if (t < 0.0f) {
				return false;
			}
			if (t > d) {
				return false;
			}
			bsm::Float3 e = bsm::cross(qp, ap);
			uvw.y = bsm::dot(ac, e);
			if (uvw.y < 0.0f || uvw.y > d) {
				return false;
			}
			uvw.z = -bsm::dot(ab, e);
			if (uvw.z < 0.0f || uvw.y + uvw.z > d) {
				return false;
			}
			float ood = 1.0f / d;
			t *= ood;
			uvw.y *= ood;
			uvw.z *= ood;
			uvw.x = 1.0f - uvw.y - uvw.z;
			return true;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OBB��PLANE�Ƃ̏Փ˔���
		@param[in]	obb	OBB
		@param[in]	plane	PLANE
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool OBB_PLANE(const OBB& obb, const PLANE& plane) {
			float r = obb.m_Size.x * abs(bsm::dot(plane.m_Normal, obb.m_Rot[0]))
				+ obb.m_Size.y * abs(bsm::dot(plane.m_Normal, obb.m_Rot[1]))
				+ obb.m_Size.z * abs(bsm::dot(plane.m_Normal, obb.m_Rot[2]));
			float s = bsm::dot(plane.m_Normal, obb.m_Center) - plane.m_DotValue;
			return abs(s) <= r;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OBB��COLRECT�Ƃ̏Փ˔���
		@param[in]	obb	OBB
		@param[in]	rect	COLRECT
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool OBB_COLRECT(const OBB& obb, const COLRECT& rect) {
			if (OBB_PLANE(obb, rect.GetPLANE())) {
				//���ʂƌ������Ă������̂�OBB�ƒ���
				OBB obb2(bsm::Float3(rect.m_BaseXSize, rect.m_BaseYSize, 1.0f), rect.m_Matrix);
				return OBB_OBB(obb, obb2);
			}
			return false;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	SPHERE��TRIANGLE�Ƃ̏Փ˔���
		@param[in]	sp	SPHERE
		@param[in]	tri	TRIANGLE
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_TRIANGLE(const SPHERE& sp, const TRIANGLE& tri, bsm::Float3& retvec) {
			ClosestPtPointTriangle(sp.m_Center, tri, retvec);
			bsm::Float3 v = retvec - sp.m_Center;
			return bsm::dot(v, v) <= sp.m_Radius * sp.m_Radius;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�J�v�Z����Obb�̍ŋߐړ_�𓾂�
		@param[in]	cp	�J�v�Z��
		@param[in]	obb	OBB
		@param[out]	flg	2���̂Ƃ̈ʒu�֌W
		@return	�ŋߐړ_
		*/
		//--------------------------------------------------------------------------------------
		static bsm::Float3 ClosestPtCapsuleOBB(const CAPSULE& cp, const OBB& obb, int& flg) {
			SPHERE Sp;
			Sp.m_Center = cp.m_PointBottom;
			Sp.m_Radius = cp.m_Radius;
			bsm::Float3 retvec;
			//�X�^�[�g�ʒu�ōŋߐړ_�𓾂�
			HitTest::SPHERE_OBB(Sp, obb, retvec);
			//���ς�}��
			bsm::Float3 Base = cp.m_PointTop - cp.m_PointBottom;
			Base.normalize();
			bsm::Float3 Dest = retvec - cp.m_PointBottom;
			float dot = bsm::dot(Base, Dest);
			if (dot < 0) {
				//�X�^�[�g�ʒu�̋��̂̊O��
				//retvec�͗L��
				flg = -1;
				return retvec;
			}
			float  size = bsm::length(cp.m_PointTop - cp.m_PointBottom);
			if (dot > size) {
				//�I�_����ɂ���
				Sp.m_Center = cp.m_PointTop;
				HitTest::SPHERE_OBB(Sp, obb, retvec);
				//�I�_�ōŋߐړ_���Ƃ�
				flg = 1;
				return retvec;
			}
			//���S��obb�̍ŋߐړ_�𓾂�
			HitTest::ClosestPtPointOBB(cp.GetCenter(), obb, retvec);
			float t;
			bsm::Float3 SegPoint;
			HitTest::ClosetPtPointSegment(retvec, cp.m_PointBottom, cp.m_PointTop, t, SegPoint);
			bsm::Float3 Span = retvec - SegPoint;
			Span.normalize();
			Span *= cp.m_Radius;
			SegPoint += Span;
			retvec = SegPoint;
			flg = 0;
			return retvec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�J�v�Z����Aabb�̍ŋߐړ_�𓾂�
		@param[in]	cp	�J�v�Z��
		@param[in]	aabb	AABB
		@param[out]	flg	�ʒu�֌W
		@return	�ŋߐړ_
		*/
		//--------------------------------------------------------------------------------------
		static bsm::Float3 ClosestPtCapsuleAABB(const CAPSULE& cp, const AABB& aabb, int& flg) {
			SPHERE Sp;
			Sp.m_Center = cp.m_PointBottom;
			Sp.m_Radius = cp.m_Radius;
			bsm::Float3 retvec;
			//�X�^�[�g�ʒu�ōŋߐړ_�𓾂�
			HitTest::SPHERE_AABB(Sp, aabb, retvec);
			//���ς�}��
			bsm::Float3 Base = cp.m_PointTop - cp.m_PointBottom;
			Base.normalize();
			bsm::Float3 Dest = retvec - cp.m_PointBottom;
			float dot = bsm::dot(Base, Dest);
			if (dot < 0) {
				//�X�^�[�g�ʒu�̋��̂̊O��
				//retvec�͗L��
				flg = -1;
				return retvec;
			}
			float  size = bsm::length(cp.m_PointTop - cp.m_PointBottom);
			if (dot > size) {
				//�I�_����ɂ���
				Sp.m_Center = cp.m_PointTop;
				HitTest::SPHERE_AABB(Sp, aabb, retvec);
				//�I�_�ōŋߐړ_���Ƃ�
				flg = 1;
				return retvec;
			}
			//���S��aabb�̍ŋߐړ_�𓾂�
			HitTest::ClosestPtPointAABB(cp.GetCenter(), aabb, retvec);
			float t;
			bsm::Float3 SegPoint;
			HitTest::ClosetPtPointSegment(retvec, cp.m_PointBottom, cp.m_PointTop, t, SegPoint);
			bsm::Float3 Span = retvec - SegPoint;
			Span.normalize();
			Span *= cp.m_Radius;
			SegPoint += Span;
			retvec = SegPoint;
			flg = 0;
			return retvec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�J�v�Z����AABB�̏Փ˔���
		@param[in]	cp	�J�v�Z��
		@param[in]	aabb	AABB
		@param[out]	retvec	�ŋߐړ_
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CAPSULE_AABB(const CAPSULE& cp, const AABB& aabb, bsm::Float3& retvec) {
			//�X�B�[�v�����鋅
			SPHERE StartSp, EndSp;
			StartSp.m_Center = cp.m_PointBottom;
			StartSp.m_Radius = cp.m_Radius;
			EndSp.m_Center = cp.m_PointTop;
			EndSp.m_Radius = cp.m_Radius;
			//�e�_��aabb�̍ŋߐړ_�𓾂�
			//�J�v�Z����AABB�̍ŋߐړ_�𓾂�i�Փ˂��Ă邩�ǂ����͊֌W�Ȃ��j
			int flg;
			retvec = ClosestPtCapsuleAABB(cp, aabb, flg);
			float HitTime;
			bsm::Float3 Velocity = EndSp.m_Center - StartSp.m_Center;
			if (CollisionTestSphereAabb(StartSp, Velocity, aabb, 0, 1.0f, HitTime)) {
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�J�v�Z����Obb�̏Փ˔���
		@param[in]	cp	�J�v�Z��
		@param[in]	obb	OBB
		@param[out]	retvec	�ŋߐړ_
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CAPSULE_OBB(const CAPSULE& cp, const OBB& obb, bsm::Float3& retvec) {
			//�X�B�[�v�����鋅
			SPHERE StartSp, EndSp;
			StartSp.m_Center = cp.m_PointBottom;
			StartSp.m_Radius = cp.m_Radius;
			EndSp.m_Center = cp.m_PointTop;
			EndSp.m_Radius = cp.m_Radius;
			//�e�_��obb�̍ŋߐړ_�𓾂�
			//�J�v�Z����OBB�̍ŋߐړ_�𓾂�i�Փ˂��Ă邩�ǂ����͊֌W�Ȃ��j
			int flg;
			retvec = ClosestPtCapsuleOBB(cp, obb, flg);
			float HitTime;
			bsm::Float3 Velocity = EndSp.m_Center - StartSp.m_Center;
			if (CollisionTestSphereObb(StartSp, Velocity, obb, 0, 1.0f, HitTime)) {
				SPHERE HitSp = StartSp;
				bsm::Float3 TempRet;
				if (SPHERE_OBB(StartSp, obb, TempRet) && SPHERE_OBB(EndSp, obb, TempRet)) {
					//�X�^�[�g�ʒu�ƃG���h�ʒu�ŗ����q�b�g���Ă���A�ʂŏՓ˂��Ă���
					HitSp.m_Center = cp.GetCenter();
				}
				else {
					HitSp.m_Center += Velocity * HitTime;
				}
				SPHERE_OBB(HitSp, obb, retvec);
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	CAPSULE��TRIANGLE�Ƃ̏Փ˔���
		@param[in]	cp	CAPSULE
		@param[in]	rect	COLRECT
		@param[in]	retvec	�ŋߐړ_
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CAPSULE_TRIANGLE(const CAPSULE& cp, const TRIANGLE& tri, bsm::Float3& retvec) {
			//�X�B�[�v�����鋅
			SPHERE StartSp, EndSp;
			//�������
			StartSp.m_Center = cp.m_PointBottom;
			StartSp.m_Radius = cp.m_Radius;
			EndSp.m_Center = cp.m_PointTop;
			EndSp.m_Radius = cp.m_Radius;

			PLANE p = tri.GetPLANE();
			float t;
			bsm::Float3 q;
			SEGMENT_PLANE(StartSp.m_Center, EndSp.m_Center, p, t, q);
			//���ɉ��̓_�ŏ�����
			bsm::Float3 Centor = StartSp.m_Center;
			if (t <= 0) {
				Centor = StartSp.m_Center;
			}
			else if (t >= 1.0f) {
				Centor = EndSp.m_Center;
			}
			else {
				Centor = q;
			}
			//Center�́A����̖ʂƂ̍ŋߐړ_
			//�_��TRIANGLE�̍ŋߐړ_�𓾂�i�Փ˂��Ă邩�ǂ����͊֌W�Ȃ��j
			ClosestPtPointTriangle(Centor, tri, retvec);
			float HitTime;
			bsm::Float3 Velocity = EndSp.m_Center - StartSp.m_Center;
			if (CollisionTestSphereTriangle(StartSp, Velocity, tri, 0, 1.0f, HitTime)) {
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	CAPSULE��COLRECT�Ƃ̏Փ˔���
		@param[in]	cp	CAPSULE
		@param[in]	rect	COLRECT
		@param[in]	retvec	�ŋߐړ_
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CAPSULE_COLRECT(const CAPSULE& cp, const COLRECT& rect, bsm::Float3& retvec) {
			//�X�B�[�v�����鋅
			SPHERE StartSp, EndSp;
			//�������
			StartSp.m_Center = cp.m_PointBottom;
			StartSp.m_Radius = cp.m_Radius;
			EndSp.m_Center = cp.m_PointTop;
			EndSp.m_Radius = cp.m_Radius;

			PLANE p = rect.GetPLANE();
			float t;
			bsm::Float3 q;
			SEGMENT_PLANE(StartSp.m_Center, EndSp.m_Center, p, t, q);
			//���ɉ��̓_�ŏ�����
			bsm::Float3 Centor = StartSp.m_Center;
			if (t <= 0) {
				Centor = StartSp.m_Center;
			}
			else if (t >= 1.0f) {
				Centor = EndSp.m_Center;
			}
			else {
				Centor = q;
			}
			//Center�́A����̖ʂƂ̍ŋߐړ_
			//���Ƃ�COLRECT�̍ŋߐړ_�𓾂�i�Փ˂��Ă邩�ǂ����͊֌W�Ȃ��j
			ClosestPtPointCOLRECT(Centor, rect, retvec);
			float HitTime;
			bsm::Float3 Velocity = EndSp.m_Center - StartSp.m_Center;
			if (CollisionTestSphereRect(StartSp, Velocity, rect, 0, 1.0f, HitTime)) {
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	SPHERE��SPHERE�Ƃ�AABB�Փ˔���
		@param[in]	sp1	SPHERE
		@param[in]	sp2	SPHERE
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_SPHERE_BY_AABB(const SPHERE& sp1, const SPHERE& sp2) {
			if (HitTest::AABB_AABB(sp1.GetWrappedAABB(), sp2.GetWrappedAABB())) {
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	SPHERE��CAPSULE�Ƃ�AABB�Փ˔���
		@param[in]	sp	SPHERE
		@param[in]	cp	CAPSULE
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_CAPSULE_BY_AABB(const SPHERE& sp, const CAPSULE& cp) {
			if (HitTest::AABB_AABB(sp.GetWrappedAABB(), cp.GetWrappedAABB())) {
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	SPHERE��OBB�Ƃ�AABB�Փ˔���
		@param[in]	sp	SPHERE
		@param[in]	obb	OBB
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_OBB_BY_AABB(const SPHERE& sp, const OBB& obb) {
			if (HitTest::AABB_AABB(sp.GetWrappedAABB(), obb.GetWrappedAABB())) {
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	CAPSULE��CAPSULE�Ƃ�AABB�Փ˔���
		@param[in]	cp1	CAPSULE
		@param[in]	cp2	CAPSULE
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CAPSULE_CAPSULE_BY_AABB(const CAPSULE& cp1, const CAPSULE& cp2) {
			if (HitTest::AABB_AABB(cp1.GetWrappedAABB(), cp2.GetWrappedAABB())) {
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	CAPSULE��OBB�Ƃ�AABB�Փ˔���
		@param[in]	cp	CAPSULE
		@param[in]	obb	OBB
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CAPSULE_OBB_BY_AABB(const CAPSULE& cp, const OBB& obb) {
			if (HitTest::AABB_AABB(cp.GetWrappedAABB(), obb.GetWrappedAABB())) {
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OBB��OBB�Ƃ�AABB�Փ˔���
		@param[in]	obb1 OBB
		@param[in]	obb2 OBB
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool OBB_OBB_BY_AABB(const OBB& obb1, const OBB& obb2) {
			if (HitTest::AABB_AABB(obb1.GetWrappedAABB(), obb2.GetWrappedAABB())) {
				return true;
			}
			return false;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	Sphere�Ɠ����Ȃ�Rect�̏Փ˔���
		@param[in]	SrcSp	Src�̋�
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestRect	Dest��`
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestSphereRect(const SPHERE& SrcSp, const bsm::Float3& SrcVelocity,
			const COLRECT& DestRect, float StartTime, float EndTime, float& HitTime) {
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcSp.m_Radius;
			bsm::Float3 RetVec;
			if (!HitTest::SPHERE_COLRECT(SrcSp2, DestRect, RetVec)) {
				return false;
			}
			if (EndTime - StartTime < HITTEST_INTERVAL_EPSILON) {
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestSphereRect(SrcSp, SrcVelocity, DestRect, StartTime, mid, HitTime)) {
				return true;
			}
			return CollisionTestSphereRect(SrcSp, SrcVelocity, DestRect, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Sphere�Ɠ����Ȃ�Triangle�̏Փ˔���
		@param[in]	SrcSp	Src�̋�
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestTri	DestTriangle
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestSphereTriangle(const SPHERE& SrcSp, const bsm::Float3& SrcVelocity,
			const TRIANGLE& DestTri, float StartTime, float EndTime, float& HitTime) {
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcSp.m_Radius;
			bsm::Float3 RetVec;
			if (!HitTest::SPHERE_TRIANGLE(SrcSp2, DestTri, RetVec)) {
				return false;
			}
			if (EndTime - StartTime < HITTEST_INTERVAL_EPSILON) {
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestSphereTriangle(SrcSp, SrcVelocity, DestTri, StartTime, mid, HitTime)) {
				return true;
			}
			return CollisionTestSphereTriangle(SrcSp, SrcVelocity, DestTri, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Obb�Ɠ����Ȃ�Rect�̏Փ˔���
		@param[in]	SrcObb	Src��Obb
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestRect	Dest��`
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestObbRect(const OBB& SrcObb, const bsm::Float3& SrcVelocity,
			const COLRECT& DestRect, float StartTime, float EndTime, float& HitTime) {
			OBB SrcObb2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcObb2.m_Center = SrcObb.m_Center + SrcVelocity * mid;
			//OBB�̊e�ӂ̒������g��
			SrcObb2.m_Size.x = (mid - StartTime) * bsm::length(SrcVelocity) + SrcObb.m_Size.x;
			SrcObb2.m_Size.y = (mid - StartTime) * bsm::length(SrcVelocity) + SrcObb.m_Size.y;
			SrcObb2.m_Size.z = (mid - StartTime) * bsm::length(SrcVelocity) + SrcObb.m_Size.z;
			if (!HitTest::OBB_COLRECT(SrcObb2, DestRect)) {
				return false;
			}
			if (EndTime - StartTime < HITTEST_INTERVAL_EPSILON) {
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestObbRect(SrcObb, SrcVelocity, DestRect, StartTime, mid, HitTime)) {
				return true;
			}
			return CollisionTestObbRect(SrcObb, SrcVelocity, DestRect, mid, EndTime, HitTime);
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	Sphere�Ɠ����Ȃ�Sphere�̏Փ˔���
		@param[in]	SrcSp	Src�̋�
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestSp	Dest��
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestSphereSphere(const SPHERE& SrcSp, const bsm::Float3& SrcVelocity,
			const SPHERE& DestSp, float StartTime, float EndTime, float& HitTime){
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcSp.m_Radius;
			if (!HitTest::SPHERE_SPHERE(SrcSp2, DestSp)){
				return false;
			}
			if (EndTime - StartTime < HITTEST_INTERVAL_EPSILON){
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestSphereSphere(SrcSp, SrcVelocity, DestSp, StartTime, mid, HitTime)){
				return true;
			}
			return CollisionTestSphereSphere(SrcSp, SrcVelocity, DestSp, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Sphere�Ɠ����Ȃ�Cylinder�̏Փ˔���
		@param[in]	SrcSp	Src�̋�
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestCy	Dest�V�����_�[
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestSphereCylinder(const SPHERE& SrcSp, const bsm::Float3& SrcVelocity,
			const CYLINDER& DestCy,
			float StartTime, float EndTime, float& HitTime) {
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcSp.m_Radius;
			bsm::Float3 RetVec;
			if (!HitTest::SPHERE_CYLINDER(SrcSp2, DestCy, RetVec)) {
				return false;
			}
			if (EndTime - StartTime < HITTEST_INTERVAL_EPSILON) {
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestSphereCylinder(SrcSp, SrcVelocity, DestCy, StartTime, mid, HitTime)) {
				return true;
			}
			return CollisionTestSphereCylinder(SrcSp, SrcVelocity, DestCy, mid, EndTime, HitTime);
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	Sphere�Ɠ����Ȃ�Capsule�̏Փ˔���
		@param[in]	SrcSp	Src�̋�
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestCap	Dest�J�v�Z��
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestSphereCapsule(const SPHERE& SrcSp, const bsm::Float3& SrcVelocity,
			const CAPSULE& DestCap,
			float StartTime, float EndTime, float& HitTime){
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcSp.m_Radius;
			bsm::Float3 RetVec;
			if (!HitTest::SPHERE_CAPSULE(SrcSp2, DestCap, RetVec)){
				return false;
			}
			if (EndTime - StartTime < HITTEST_INTERVAL_EPSILON){
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestSphereCapsule(SrcSp, SrcVelocity, DestCap, StartTime, mid, HitTime)){
				return true;
			}
			return CollisionTestSphereCapsule(SrcSp, SrcVelocity, DestCap, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Capsule�Ɠ����Ȃ�Capsule�̏Փ˔���
		@param[in]	SrcCap	Src�̃J�v�Z��
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestCap	Dest�J�v�Z��
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestCapsuleCapsule(const CAPSULE& SrcCapsule, const bsm::Float3& SrcVelocity,
			const CAPSULE& DestCap,
			float StartTime, float EndTime, float& HitTime){
			CAPSULE SrcCapsule2 = SrcCapsule;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcCapsule2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcCapsule.m_Radius;
			float Scale = SrcCapsule2.m_Radius / SrcCapsule.m_Radius;

			//���S�����_�̌��̃J�v�Z�����쐬
			CAPSULE SrcBaseCapsule = SrcCapsule;
			SrcBaseCapsule.SetCenter(bsm::Float3(0, 0, 0));
			//���_�J�v�Z���ŃX�P�[�����O
			//�X�P�[�����O�s��̍쐬
			bsm::Mat4x4 ScalMat;
			ScalMat.scale(bsm::Float3(Scale));
			//�e���_���X�P�[�����O
			SrcCapsule2.m_PointBottom = SrcBaseCapsule.m_PointBottom * ScalMat;
			SrcCapsule2.m_PointTop = SrcBaseCapsule.m_PointTop * ScalMat;
			//���S���ړ�
			SrcCapsule2.SetCenter(SrcCapsule.GetCenter() + SrcVelocity * mid);

			bsm::Float3 RetVec1, RetFloat2;
			if (!HitTest::CAPSULE_CAPSULE(SrcCapsule2, DestCap, RetVec1, RetFloat2)){
				return false;
			}
			if (EndTime - StartTime < HITTEST_INTERVAL_EPSILON){
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestCapsuleCapsule(SrcCapsule, SrcVelocity, DestCap, StartTime, mid, HitTime)){
				return true;
			}
			return CollisionTestCapsuleCapsule(SrcCapsule, SrcVelocity, DestCap, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Sphere�Ɠ����Ȃ�Aabb�̏Փ˔���
		@param[in]	SrcSp	Src�̋�
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestAabb	Dest��AABB
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestSphereAabb(const SPHERE& SrcSp, const bsm::Float3& SrcVelocity,
			const AABB& DestAabb,
			float StartTime, float EndTime, float& HitTime){
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcSp.m_Radius;
			bsm::Float3 RetVec;
			if (!HitTest::SPHERE_AABB(SrcSp2, DestAabb, RetVec)){
				return false;
			}
			if (EndTime - StartTime < HITTEST_INTERVAL_EPSILON){
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestSphereAabb(SrcSp, SrcVelocity, DestAabb, StartTime, mid, HitTime)){
				return true;
			}
			return CollisionTestSphereAabb(SrcSp, SrcVelocity, DestAabb, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Sphere�Ɠ����Ȃ�Obb�̏Փ˔���
		@param[in]	SrcSp	Src�̋�
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestObb	Dest��OBB
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestSphereObb(const SPHERE& SrcSp, const bsm::Float3& SrcVelocity,
			const OBB& DestObb,
			float StartTime, float EndTime, float& HitTime){
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcSp.m_Radius;
			bsm::Float3 RetVec;
			if (!HitTest::SPHERE_OBB(SrcSp2, DestObb, RetVec)){
				return false;
			}
			if (EndTime - StartTime < HITTEST_INTERVAL_EPSILON){
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestSphereObb(SrcSp, SrcVelocity, DestObb, StartTime, mid, HitTime)){
				return true;
			}
			return CollisionTestSphereObb(SrcSp, SrcVelocity, DestObb, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Sphere�Ɠ����Ȃ�Obb�̏Փ˔���(Epsilon�w��)
		@param[in]	SrcSp	Src�̋�
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestObb	Dest��OBB
		@param[in]	Epsilon	�w���Epsilon
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestSphereObbWithEpsilon(const SPHERE& SrcSp, const bsm::Float3& SrcVelocity,
			const OBB& DestObb,float Epsilon,
			float StartTime, float EndTime, float& HitTime) {
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcSp.m_Radius;
			bsm::Float3 RetVec;
			if (!HitTest::SPHERE_OBB(SrcSp2, DestObb, RetVec)) {
				return false;
			}
			if (EndTime - StartTime < Epsilon) {
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestSphereObbWithEpsilon(SrcSp, SrcVelocity, DestObb, Epsilon, StartTime, mid, HitTime)) {
				return true;
			}
			return CollisionTestSphereObbWithEpsilon(SrcSp, SrcVelocity, DestObb, Epsilon, mid, EndTime, HitTime);
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�J�v�Z���Ɠ����Ȃ�AABB�̏Փ˔���
		@param[in]	SrcCapsule	Src�̃J�v�Z��
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestAabb	Dest��AABB
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestCapsuleAabb(const CAPSULE& SrcCapsule, const bsm::Float3& SrcVelocity,
			const AABB& DestAabb, float StartTime, float EndTime, float& HitTime) {
			CAPSULE SrcCapsule2 = SrcCapsule;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcCapsule2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcCapsule.m_Radius;
			float Scale = SrcCapsule2.m_Radius / SrcCapsule.m_Radius;
			//���S�����_�̌��̃J�v�Z�����쐬
			CAPSULE SrcBaseCapsule = SrcCapsule;
			SrcBaseCapsule.SetCenter(bsm::Float3(0, 0, 0));
			//���_�J�v�Z���ŃX�P�[�����O
			//�X�P�[�����O�s��̍쐬
			bsm::Mat4x4 ScalMat;
			ScalMat.scale(bsm::Float3(Scale));
			//�e���_���X�P�[�����O
			SrcCapsule2.m_PointBottom = SrcBaseCapsule.m_PointBottom * ScalMat;
			SrcCapsule2.m_PointTop = SrcBaseCapsule.m_PointTop * ScalMat;
			//���S���ړ�
			SrcCapsule2.SetCenter(SrcCapsule.GetCenter() + SrcVelocity * mid);
			bsm::Float3 RetVec;
			if (!HitTest::CAPSULE_AABB(SrcCapsule2, DestAabb, RetVec)) {
				return false;
			}
			if (EndTime - StartTime < HITTEST_INTERVAL_EPSILON) {
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestCapsuleAabb(SrcCapsule, SrcVelocity, DestAabb, StartTime, mid, HitTime)) {
				return true;
			}
			return CollisionTestCapsuleAabb(SrcCapsule, SrcVelocity, DestAabb, mid, EndTime, HitTime);
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�J�v�Z���Ɠ����Ȃ�Obb�̏Փ˔���
		@param[in]	SrcCapsule	Src�̃J�v�Z��
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestObb	Dest��OBB
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestCapsuleObb(const CAPSULE& SrcCapsule, const bsm::Float3& SrcVelocity,
			const OBB& DestObb, float StartTime, float EndTime, float& HitTime){
			CAPSULE SrcCapsule2 = SrcCapsule;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcCapsule2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcCapsule.m_Radius;
			float Scale = SrcCapsule2.m_Radius / SrcCapsule.m_Radius;
			//���S�����_�̌��̃J�v�Z�����쐬
			CAPSULE SrcBaseCapsule = SrcCapsule;
			SrcBaseCapsule.SetCenter(bsm::Float3(0, 0, 0));
			//���_�J�v�Z���ŃX�P�[�����O
			//�X�P�[�����O�s��̍쐬
			bsm::Mat4x4 ScalMat;
			ScalMat.scale(bsm::Float3(Scale));
			//�e���_���X�P�[�����O
			SrcCapsule2.m_PointBottom = SrcBaseCapsule.m_PointBottom * ScalMat;
			SrcCapsule2.m_PointTop = SrcBaseCapsule.m_PointTop * ScalMat;
			//���S���ړ�
			SrcCapsule2.SetCenter(SrcCapsule.GetCenter() + SrcVelocity * mid);
			bsm::Float3 RetVec;
			if (!HitTest::CAPSULE_OBB(SrcCapsule2, DestObb, RetVec)){
				return false;
			}
			if (EndTime - StartTime < HITTEST_INTERVAL_EPSILON){
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestCapsuleObb(SrcCapsule, SrcVelocity, DestObb, StartTime, mid, HitTime)){
				return true;
			}
			return CollisionTestCapsuleObb(SrcCapsule, SrcVelocity, DestObb, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Capsule�Ɠ����Ȃ�Triangle�̏Փ˔���
		@param[in]	SrcCapsule	Src�̃J�v�Z��
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestTri		Dest�O�p�`
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestCapsuleTriangle(const CAPSULE& SrcCapsule, const bsm::Float3& SrcVelocity,
			const TRIANGLE& DestTri, float StartTime, float EndTime, float& HitTime) {
			CAPSULE SrcCapsule2 = SrcCapsule;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcCapsule2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcCapsule.m_Radius;
			float Scale = SrcCapsule2.m_Radius / SrcCapsule.m_Radius;
			//���S�����_�̌��̃J�v�Z�����쐬
			CAPSULE SrcBaseCapsule = SrcCapsule;
			SrcBaseCapsule.SetCenter(bsm::Float3(0, 0, 0));
			//���_�J�v�Z���ŃX�P�[�����O
			//�X�P�[�����O�s��̍쐬
			bsm::Mat4x4 ScalMat;
			ScalMat.scale(bsm::Float3(Scale));
			//�e���_���X�P�[�����O
			SrcCapsule2.m_PointBottom = SrcBaseCapsule.m_PointBottom * ScalMat;
			SrcCapsule2.m_PointTop = SrcBaseCapsule.m_PointTop * ScalMat;
			//���S���ړ�
			SrcCapsule2.SetCenter(SrcCapsule.GetCenter() + SrcVelocity * mid);
			bsm::Float3 RetVec;
			if (!HitTest::CAPSULE_TRIANGLE(SrcCapsule2, DestTri, RetVec)) {
				return false;
			}
			if (EndTime - StartTime < HITTEST_INTERVAL_EPSILON) {
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestCapsuleTriangle(SrcCapsule, SrcVelocity, DestTri, StartTime, mid, HitTime)) {
				return true;
			}
			return CollisionTestCapsuleTriangle(SrcCapsule, SrcVelocity, DestTri, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Capsule�Ɠ����Ȃ�Rect�̏Փ˔���
		@param[in]	SrcCapsule	Src�̋�
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestRect	Dest��`
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestCapsuleRect(const CAPSULE& SrcCapsule, const bsm::Float3& SrcVelocity,
			const COLRECT& DestRect, float StartTime, float EndTime, float& HitTime) {
			CAPSULE SrcCapsule2 = SrcCapsule;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcCapsule2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcCapsule.m_Radius;
			float Scale = SrcCapsule2.m_Radius / SrcCapsule.m_Radius;
			//���S�����_�̌��̃J�v�Z�����쐬
			CAPSULE SrcBaseCapsule = SrcCapsule;
			SrcBaseCapsule.SetCenter(bsm::Float3(0, 0, 0));
			//���_�J�v�Z���ŃX�P�[�����O
			//�X�P�[�����O�s��̍쐬
			bsm::Mat4x4 ScalMat;
			ScalMat.scale(bsm::Float3(Scale));
			//�e���_���X�P�[�����O
			SrcCapsule2.m_PointBottom = SrcBaseCapsule.m_PointBottom * ScalMat;
			SrcCapsule2.m_PointTop = SrcBaseCapsule.m_PointTop * ScalMat;
			//���S���ړ�
			SrcCapsule2.SetCenter(SrcCapsule.GetCenter() + SrcVelocity * mid);
			bsm::Float3 RetVec;
			if (!HitTest::CAPSULE_COLRECT(SrcCapsule2, DestRect, RetVec)) {
				return false;
			}
			if (EndTime - StartTime < HITTEST_INTERVAL_EPSILON) {
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestCapsuleRect(SrcCapsule, SrcVelocity, DestRect, StartTime, mid, HitTime)) {
				return true;
			}
			return CollisionTestCapsuleRect(SrcCapsule, SrcVelocity, DestRect, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OBB�Ɠ����Ȃ�OBB�̏Փ˔���i�T�u�֐��Ƃ��Ď����j
		@param[in]	SrcObb	Src��Obb
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestObb	Dest��OBB
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestObbObbSub(const OBB& SrcObb, const bsm::Float3& SrcVelocity,
			const OBB& DestObb, float StartTime, float EndTime, float& HitTime){
			OBB SrcObb2 = SrcObb;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcObb2.m_Center = SrcObb.m_Center + SrcVelocity * mid;
			//OBB�̊e�ӂ̒������g��
			SrcObb2.m_Size.x = (mid - StartTime) * bsm::length(SrcVelocity) + SrcObb.m_Size.x;
			SrcObb2.m_Size.y = (mid - StartTime) * bsm::length(SrcVelocity) + SrcObb.m_Size.y;
			SrcObb2.m_Size.z = (mid - StartTime) * bsm::length(SrcVelocity) + SrcObb.m_Size.z;
			if (!HitTest::OBB_OBB(SrcObb2, DestObb)){
				return false;
			}
			if (EndTime - StartTime < HITTEST_INTERVAL_EPSILON){
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestObbObbSub(SrcObb, SrcVelocity, DestObb, StartTime, mid, HitTime)){
				return true;
			}
			return CollisionTestObbObbSub(SrcObb, SrcVelocity, DestObb, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OBB�Ɠ����Ȃ�OBB�̏Փ˔���i�T�u�֐��Ƃ��Ď����AEpsilon�w��j
		@param[in]	SrcObb	Src��Obb
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestObb	Dest��OBB
		@param[in]	Epsilon	�w���Epsilon
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestObbObbSubWithEpsilon(const OBB& SrcObb, const bsm::Float3& SrcVelocity,
			const OBB& DestObb,float Epsilon,
			float StartTime, float EndTime, float& HitTime) {
			OBB SrcObb2 = SrcObb;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcObb2.m_Center = SrcObb.m_Center + SrcVelocity * mid;
			//OBB�̊e�ӂ̒������g��
			SrcObb2.m_Size.x = (mid - StartTime) * bsm::length(SrcVelocity) + SrcObb.m_Size.x;
			SrcObb2.m_Size.y = (mid - StartTime) * bsm::length(SrcVelocity) + SrcObb.m_Size.y;
			SrcObb2.m_Size.z = (mid - StartTime) * bsm::length(SrcVelocity) + SrcObb.m_Size.z;
			if (!HitTest::OBB_OBB(SrcObb2, DestObb)) {
				return false;
			}
			if (EndTime - StartTime < Epsilon) {
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestObbObbSubWithEpsilon(SrcObb, SrcVelocity, DestObb, Epsilon,StartTime, mid, HitTime)) {
				return true;
			}
			return CollisionTestObbObbSubWithEpsilon(SrcObb, SrcVelocity, DestObb, Epsilon, mid, EndTime, HitTime);

		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	OBB�Ɠ����Ȃ�OBB�̏Փ˔���iSrcObb��SPHERE���ł���OBB�j
		@param[in]	SrcObb	Src��Obb
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestObb	Dest��OBB
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestObbObb(const OBB& SrcObb, const bsm::Float3& SrcVelocity,
			const OBB& DestObb, float StartTime, float EndTime, float& HitTime){
			SPHERE BeforeSrcSphere;
			BeforeSrcSphere.m_Center = SrcObb.m_Center;
			BeforeSrcSphere.m_Radius = bsm::length(SrcObb.m_Size);
			if (!CollisionTestSphereObb(BeforeSrcSphere, SrcVelocity, DestObb, StartTime, EndTime, HitTime)){
				//�Փ˂��Ȃ����OBB���Փ˂Ȃ�
				return false;
			}
			return CollisionTestObbObbSub(SrcObb, SrcVelocity,DestObb, StartTime, EndTime,HitTime);
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	OBB�Ɠ����Ȃ�OBB�̏Փ˔���iSrcObb��SPHERE���ł���OBB�AEpsilon�w��j
		@param[in]	SrcObb	Src��Obb
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestObb	Dest��OBB
		@param[in]	Epsilon	�w���Epsilon
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestObbObbWithEpsilon(const OBB& SrcObb, const bsm::Float3& SrcVelocity,
			const OBB& DestObb,float Epsilon, float StartTime, float EndTime, float& HitTime) {
			SPHERE BeforeSrcSphere;
			BeforeSrcSphere.m_Center = SrcObb.m_Center;
			BeforeSrcSphere.m_Radius = bsm::length(SrcObb.m_Size);
			if (!CollisionTestSphereObbWithEpsilon(BeforeSrcSphere, SrcVelocity, DestObb, Epsilon, StartTime, EndTime, HitTime)) {
				//�Փ˂��Ȃ����OBB���Փ˂Ȃ�
				return false;
			}
			return CollisionTestObbObbSubWithEpsilon(SrcObb, SrcVelocity, DestObb, Epsilon, StartTime, EndTime, HitTime);
		}

	};

	inline AABB CAPSULE::GetWrappedAABB() const {
		SPHERE sp_bottom, sp_top;
		sp_bottom.m_Center = m_PointBottom;
		sp_bottom.m_Radius = m_Radius;
		sp_top.m_Center = m_PointBottom;
		sp_top.m_Radius = m_Radius;
		return HitTest::AABB_OR_AABB(sp_bottom.GetWrappedAABB(), sp_top.GetWrappedAABB());
	}

}
//end of namespace basedx12.


