/*!
@file BaseMathInc.h
@brief XMMath���g�����߂̔������C���[(include�t�@k��)
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once

#include "stdafx.h"
#include "BaseMath.h"

namespace basedx12 {
namespace bsm {


	//--------------------------------------------------------------------------------------
	///	Flt2�C�����C������
	//--------------------------------------------------------------------------------------
	inline Flt2::Flt2() :
		XMFLOAT2(0, 0)
	{
	}

	inline Flt2::Flt2(const Flt2 & vec) :
		XMFLOAT2(vec.x, vec.y)
	{
	}

	inline Flt2::Flt2(const Flt3 & vec) :
		XMFLOAT2(vec.x, vec.y)
	{
	}

	inline Flt2::Flt2(const Flt4 & vec) :
		XMFLOAT2(vec.x, vec.y)
	{
	}


	inline Flt2::Flt2(const XMFLOAT2& v) :
		XMFLOAT2(v)
	{
	}


	inline Flt2::Flt2(float _x, float _y) :
		XMFLOAT2(_x, _y)
	{
	}

	inline Flt2::Flt2(float val) :
		XMFLOAT2(val, val)
	{
	}

	inline Flt2::Flt2(const XMVECTOR& vec) :
		XMFLOAT2()
	{
		XMVECTOR temp = vec;
		XMStoreFloat2((XMFLOAT2*)this, temp);
	}

	inline Flt2::operator XMVECTOR() const {
		XMFLOAT2 temp = *this;
		XMVECTOR Vec = XMLoadFloat2(&temp);
		return Vec;
	}

	inline Flt2 & Flt2::operator =(const Flt2 & other)
	{
		if (this != &other) {
			x = other.x;
			y = other.y;
		}
		return *this;
	}

	inline Flt2 & Flt2::operator =(const Flt3 & other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	inline Flt2 & Flt2::operator =(const Flt4 & other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}


	inline Flt2& Flt2::operator=(const XMFLOAT2& other) {
		(XMFLOAT2)*this = other;
		return *this;
	}


	inline Flt2&  Flt2::operator=(const XMVECTOR& other) {
		XMVECTOR temp = other;
		XMStoreFloat2((XMFLOAT2*)this, temp);
		return *this;
	}

	inline bool Flt2::operator==(const Flt2& other)const {
		return XMVector2Equal(*this, other);
	}

	inline bool Flt2::operator!=(const Flt2& other)const {
		return !XMVector2Equal(*this, other);
	}

	inline Flt2& Flt2::setAll(float val) {
		x = y = val;
		return *this;
	}

	inline Flt2& Flt2::set(float _x, float _y) {
		x = _x;
		y = _y;
		return *this;
	}


	inline Flt2 & Flt2::setX(float _x)
	{
		x = _x;
		return *this;
	}


	inline Flt2 & Flt2::setY(float _y)
	{
		y = _y;
		return *this;
	}

	inline float Flt2::getX()const {
		return x;
	}
	inline float Flt2::getY()const {
		return y;
	}

	inline Flt2 & Flt2::setElem(int idx, float value)
	{
		*(&x + idx) = value;
		return *this;
	}

	inline float Flt2::getElem(int idx) const
	{
		return *(&x + idx);
	}

	inline float & Flt2::operator [](int idx)
	{
		return *(&x + idx);
	}

	inline float Flt2::operator [](int idx) const
	{
		return *(&x + idx);
	}

	inline const Flt2 Flt2::operator +(const Flt2 & vec) const
	{
		return (Flt2)XMVectorAdd(*this, vec);
	}

	inline const Flt2 Flt2::operator +(float val) const
	{
		Flt2 temp(val, val);
		return (Flt2)XMVectorAdd(*this, temp);
	}


	inline const Flt2 Flt2::operator -(const Flt2 & vec) const
	{
		return (Flt2)XMVectorSubtract(*this, vec);
	}

	inline const Flt2 Flt2::operator -(float val) const
	{
		Flt2 temp(val, val);
		return (Flt2)XMVectorSubtract(*this, temp);
	}


	inline const Flt2 Flt2::operator *(float val) const
	{
		Flt2 temp(val, val);
		return (Flt2)XMVectorMultiply(*this, temp);
	}

	inline Flt2 & Flt2::operator +=(const Flt2 & vec)
	{
		*this = *this + vec;
		return *this;
	}

	inline Flt2 & Flt2::operator +=(float val)
	{
		*this = *this + val;
		return *this;
	}


	inline Flt2 & Flt2::operator -=(const Flt2 & vec)
	{
		*this = *this - vec;
		return *this;
	}

	inline Flt2 & Flt2::operator -=(float val)
	{
		*this = *this - val;
		return *this;
	}

	inline Flt2 & Flt2::operator *=(float val)
	{
		*this = *this * val;
		return *this;
	}

	inline const Flt2 Flt2::operator /(float val) const
	{
		Flt2 temp(XMVectorReciprocal(Flt2(val, val)));
		return (Flt2)XMVectorMultiply(*this, temp);
	}

	inline Flt2 & Flt2::operator /=(float val)
	{
		*this = *this / val;
		return *this;
	}

	inline const Flt2 Flt2::operator -() const
	{
		Flt2 temp(-1.0f, -1.0f);
		return (Flt2)XMVectorMultiply(*this, temp);
	}

	inline Flt2& Flt2::normalize() {
		*this = (Flt2)XMVector2Normalize(*this);
		return *this;
	}
	inline Flt2& Flt2::floor(int len) {
		x = bsm::floor(x, len);
		y = bsm::floor(y, len);
		return *this;

	}

	inline Flt2& Flt2::round(int len) {
		x = bsm::round(x, len);
		y = bsm::round(y, len);

		return *this;
	}

	inline Flt2& Flt2::ceil(int len) {
		x = bsm::ceil(x, len);
		y = bsm::ceil(y, len);
		return *this;
	}




	inline float Flt2::dot(const Flt2 & vec1)const {
		return ((Flt2)XMVector2Dot(*this, vec1)).x;
	}

	inline float Flt2::lengthSqr()const {
		return ((Flt2)XMVector2LengthSq(*this)).x;
	}

	inline float Flt2::length()const {
		return ((Flt2)XMVector2Length(*this)).x;

	}

	inline bool Flt2::isNaN() const {
		return XMVector2IsNaN(*this);
	}
	inline bool Flt2::isInfinite() const {
		return XMVector2IsInfinite(*this);
	}



	//--------------------------------------------------------------------------------------
	///	Flt3�C�����C������
	//--------------------------------------------------------------------------------------
	inline Flt3::Flt3() :
		XMFLOAT3(0, 0, 0)
	{
	}


	inline Flt3::Flt3(const Flt3 & vec) :
		XMFLOAT3(vec.x, vec.y, vec.z)
	{
	}

	inline Flt3::Flt3(const Flt2 & vec) :
		XMFLOAT3(vec.x, vec.y, 0)
	{
	}


	inline Flt3::Flt3(const Flt4 & vec) :
		XMFLOAT3(vec.x, vec.y, vec.z)
	{
	}



	inline Flt3::Flt3(const XMFLOAT3& v) :
		XMFLOAT3(v)
	{
	}


	inline Flt3::Flt3(float _x, float _y, float _z):
		XMFLOAT3(_x, _y, _z)
	{
	}

	inline Flt3::Flt3(float val):
		XMFLOAT3(val, val, val)
	{
	}

	inline Flt3::Flt3(const XMVECTOR& vec) :
		XMFLOAT3()
	{
		XMVECTOR temp = vec;
		XMStoreFloat3((XMFLOAT3*)this, temp);
	}

	inline Flt3::operator XMVECTOR() const {
		XMFLOAT3 temp = *this;
		XMVECTOR Vec = XMLoadFloat3(&temp);
		return Vec;
	}

	inline Flt3 & Flt3::operator =(const Flt2 & other)
	{
		x = other.x;
		y = other.y;
		z = 0;
		return *this;
	}

	inline Flt3 & Flt3::operator =(const Flt3 & other)
	{
		if (this != &other) {
			x = other.x;
			y = other.y;
			z = other.z;
		}
		return *this;
	}

	inline Flt3 & Flt3::operator =(const Flt4 & other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	inline Flt3& Flt3::operator=(const XMFLOAT3& other) {
		(XMFLOAT3)*this = other;
		return *this;
	}


	inline Flt3&  Flt3::operator=(const XMVECTOR& other) {
		XMVECTOR temp = other;
		XMStoreFloat3((XMFLOAT3*)this, temp);
		return *this;
	}

	inline bool Flt3::operator==(const Flt3& other)const {
		return XMVector3Equal(*this, other);
	}

	inline bool Flt3::operator!=(const Flt3& other)const {
		return !XMVector3Equal(*this, other);
	}

	inline Flt3& Flt3::setAll(float val) {
		x = y = z = val;
		return *this;
	}

	inline Flt3& Flt3::set(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
		return *this;
	}


	inline Flt3& Flt3::setX(float _x)
	{
		x = _x;
		return *this;
	}


	inline Flt3 & Flt3::setY(float _y)
	{
		y = _y;
		return *this;
	}


	inline Flt3 & Flt3::setZ(float _z)
	{
		z = _z;
		return *this;
	}

	inline float Flt3::getX()const {
		return x;
	}
	inline float Flt3::getY()const {
		return y;
	}
	inline float Flt3::getZ()const {
		return z;
	}

	inline Flt3 & Flt3::setElem(int idx, float value)
	{
		*(&x + idx) = value;
		return *this;
	}

	inline float Flt3::getElem(int idx) const
	{
		return *(&x + idx);
	}

	inline float & Flt3::operator [](int idx)
	{
		return *(&x + idx);
	}

	inline float Flt3::operator [](int idx) const
	{
		return *(&x + idx);
	}

	inline const Flt3 Flt3::operator +(const Flt3 & vec) const
	{
		return (Flt3)XMVectorAdd(*this, vec);
	}

	inline const Flt3 Flt3::operator +(float val) const
	{
		Flt3 temp(val, val, val);
		return (Flt3)XMVectorAdd(*this, temp);
	}


	inline const Flt3 Flt3::operator -(const Flt3 & vec) const
	{
		return (Flt3)XMVectorSubtract(*this, vec);
	}

	inline const Flt3 Flt3::operator -(float val) const
	{
		Flt3 temp(val, val, val);
		return (Flt3)XMVectorSubtract(*this, temp);
	}

	inline const Flt3 Flt3::operator *(float val) const
	{
		Flt3 temp(val, val, val);
		return (Flt3)XMVectorMultiply(*this, temp);
	}

	inline const Flt3 Flt3::operator *(const Mat3x3 & mat) const {
		return (Flt3)XMVector3Transform(*this, mat);
	}

	inline const Flt3 Flt3::operator *(const Mat4x4 & mat) const {
		return (Flt3)XMVector3Transform(*this, mat);
	}

	inline Flt3 & Flt3::operator +=(const Flt3 & vec)
	{
		*this = *this + vec;
		return *this;
	}

	inline Flt3 & Flt3::operator +=(float val)
	{
		*this = *this + val;
		return *this;
	}


	inline Flt3 & Flt3::operator -=(const Flt3 & vec)
	{
		*this = *this - vec;
		return *this;
	}

	inline Flt3 & Flt3::operator -=(float val)
	{
		*this = *this - val;
		return *this;
	}


	inline Flt3 & Flt3::operator *=(float val)
	{
		*this = *this * val;
		return *this;
	}

	inline Flt3& Flt3::operator *=(const Mat3x3 & mat) {
		*this = *this * mat;
		return *this;
	}

	inline Flt3& Flt3::operator *=(const Mat4x4 & mat) {
		*this = *this * mat;
		return *this;
	}



	inline const Flt3 Flt3::operator /(float val) const
	{
		Flt3 temp(XMVectorReciprocal(Flt3(val, val, val)));
		return (Flt3)XMVectorMultiply(*this, temp);
	}

	inline Flt3 & Flt3::operator /=(float val)
	{
		*this = *this / val;
		return *this;
	}

	inline const Flt3 Flt3::operator -() const
	{
		Flt3 temp(-1.0f, -1.0f, -1.0f);
		return (Flt3)XMVectorMultiply(*this, temp);
	}

	inline Flt3& Flt3::normalize() {
		*this = (Flt3)XMVector3Normalize(*this);
		return *this;
	}

	inline Flt3& Flt3::floor(int len) {
		x = bsm::floor(x, len);
		y = bsm::floor(y, len);
		z = bsm::floor(z, len);
		return *this;

	}

	inline Flt3& Flt3::round(int len) {
		x = bsm::round(x, len);
		y = bsm::round(y, len);
		z = bsm::round(z, len);
		return *this;
	}

	inline Flt3& Flt3::ceil(int len) {
		x = bsm::ceil(x, len);
		y = bsm::ceil(y, len);
		z = bsm::ceil(z, len);
		return *this;
	}


	inline float Flt3::dot(const Flt3 & vec1)const {
		return ((Flt3)XMVector3Dot(*this, vec1)).x;
	}

	inline Flt3& Flt3::cross(const Flt3 & vec1) {
		*this = (Flt3)XMVector3Cross(*this, vec1);
		return *this;
	}



	inline float Flt3::lengthSqr()const {
		return ((Flt3)XMVector3LengthSq(*this)).x;
	}

	inline float Flt3::length()const {
		return ((Flt3)XMVector3Length(*this)).x;

	}

	inline Flt3& Flt3::reflect(const Flt3 & normal) {
		*this = (Flt3)XMVector3Reflect(*this, normal);
		return *this;
	}

	inline Flt3& Flt3::slide(const Flt3 & normal) {
		//*this�Ɩ@�����璼�s���̒����i���ςŋ��߂�j
		float Len = bsm::dot(*this, normal);
		//���̒����ɐL�΂�
		Flt3 Contact = normal * Len;
		//�X���C�h��������͌��݂̃x�N�g����������Z
		*this -= Contact;
		return *this;
	}

	inline bool Flt3::isNaN() const {
		return XMVector3IsNaN(*this);
	}
	inline bool Flt3::isInfinite() const {
		return XMVector3IsInfinite(*this);
	}



	//--------------------------------------------------------------------------------------
	///	Flt4�C�����C������
	//--------------------------------------------------------------------------------------
	inline Flt4::Flt4() :
		XMFLOAT4(0, 0, 0, 0)
	{
	}


	inline Flt4::Flt4(const Flt4 & vec) :
		XMFLOAT4(vec.x, vec.y, vec.z, vec.w)
	{
	}

	inline Flt4::Flt4(const Flt2 & vec) :
		XMFLOAT4(vec.x, vec.y, 0, 0)
	{
	}

	inline Flt4::Flt4(const Flt3 & vec) :
		XMFLOAT4(vec.x, vec.y, vec.z, 0)
	{
	}


	inline Flt4::Flt4(const Flt3& vec, float _w):
		XMFLOAT4(vec.x, vec.y, vec.z, _w)
	{}


	inline Flt4::Flt4(const XMFLOAT4& v) :
		XMFLOAT4(v)
	{
	}


	inline Flt4::Flt4(float _x, float _y, float _z, float _w) :
		XMFLOAT4(_x, _y, _z, _w)
	{
	}

	inline Flt4::Flt4(float val) :
		XMFLOAT4(val, val, val, val)
	{
	}

	inline Flt4::Flt4(const XMVECTOR& vec) :
		XMFLOAT4()
	{
		XMVECTOR temp = vec;
		XMStoreFloat4((XMFLOAT4*)this, temp);
	}

	inline Flt4::operator XMVECTOR() const {
		XMFLOAT4 temp = *this;
		XMVECTOR Vec = XMLoadFloat4(&temp);
		return Vec;
	}

	inline Flt4::operator Flt3() const {
		return Flt3(x, y, z);
	}

	inline Flt4& Flt4::operator =(const Flt2& other) {
		x = other.x;
		y = other.y;
		z = 0;
		w = 0;
		return *this;
	}


	inline Flt4& Flt4::operator =(const Flt3& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		w = 0;
		return *this;
	}

	inline Flt4 & Flt4::operator =(const Flt4 & other)
	{
		if (this != &other) {
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		return *this;
	}


	inline Flt4& Flt4::operator=(const XMFLOAT4& other) {
		(XMFLOAT4)*this = other;
		return *this;
	}


	inline Flt4&  Flt4::operator=(const XMVECTOR& other) {
		XMVECTOR temp = other;
		XMStoreFloat4((XMFLOAT4*)this, temp);
		return *this;
	}

	inline bool Flt4::operator==(const Flt4& other)const {
		return XMVector4Equal(*this, other);
	}

	inline bool Flt4::operator!=(const Flt4& other)const {
		return !XMVector4Equal(*this, other);
	}

	inline Flt4& Flt4::setAll(float val) {
		x = y = z = w = val;
		return *this;
	}

	inline Flt4& Flt4::set(float _x, float _y, float _z, float _w) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
		return *this;
	}


	inline Flt4 & Flt4::setX(float _x)
	{
		x = _x;
		return *this;
	}


	inline Flt4 & Flt4::setY(float _y)
	{
		y = _y;
		return *this;
	}


	inline Flt4 & Flt4::setZ(float _z)
	{
		z = _z;
		return *this;
	}

	inline Flt4 & Flt4::setW(float _w)
	{
		w = _w;
		return *this;
	}

	inline float Flt4::getX()const {
		return x;
	}
	inline float Flt4::getY()const {
		return y;
	}
	inline float Flt4::getZ()const {
		return z;
	}
	inline float Flt4::getW()const {
		return w;
	}

	inline Flt4 & Flt4::setElem(int idx, float value)
	{
		*(&x + idx) = value;
		return *this;
	}

	inline float Flt4::getElem(int idx) const
	{
		return *(&x + idx);
	}

	inline float & Flt4::operator [](int idx)
	{
		return *(&x + idx);
	}

	inline float Flt4::operator [](int idx) const
	{
		return *(&x + idx);
	}

	inline const Flt4 Flt4::operator +(const Flt4 & vec) const
	{
		return (Flt4)XMVectorAdd(*this, vec);
	}

	inline const Flt4 Flt4::operator +(float val) const
	{
		Flt4 temp(val, val, val, val);
		return (Flt4)XMVectorAdd(*this, temp);
	}


	inline const Flt4 Flt4::operator -(const Flt4 & vec) const
	{
		return (Flt4)XMVectorSubtract(*this, vec);
	}

	inline const Flt4 Flt4::operator -(float val) const
	{
		Flt4 temp(val, val, val, val);
		return (Flt4)XMVectorSubtract(*this, temp);
	}

	inline const Flt4 Flt4::operator *(float val) const
	{
		Flt4 temp(val, val, val, val);
		return (Flt4)XMVectorMultiply(*this, temp);
	}

	inline const Flt4 Flt4::operator *(const Mat3x3 & mat) const {
		return (Flt4)XMVector4Transform(*this, mat);
	}

	inline const Flt4  Flt4::operator *(const Mat4x4 & mat) const {
		return (Flt4)XMVector4Transform(*this, mat);
	}

	inline Flt4 & Flt4::operator +=(const Flt4 & vec)
	{
		*this = *this + vec;
		return *this;
	}

	inline Flt4 & Flt4::operator +=(float val)
	{
		*this = *this + val;
		return *this;
	}

	inline Flt4 & Flt4::operator -=(const Flt4 & vec)
	{
		*this = *this - vec;
		return *this;
	}

	inline Flt4 & Flt4::operator -=(float val)
	{
		*this = *this - val;
		return *this;
	}

	inline Flt4 & Flt4::operator *=(float val)
	{
		*this = *this * val;
		return *this;
	}

	inline Flt4& Flt4::operator *=(const Mat3x3 & mat) {
		*this = *this * mat;
		return *this;
	}

	inline Flt4& Flt4::operator *=(const Mat4x4 & mat) {
		*this = *this * mat;
		return *this;
	}



	inline const Flt4 Flt4::operator /(float val) const
	{
		Flt4 temp(XMVectorReciprocal(Flt4(val, val, val, val)));
		return (Flt4)XMVectorMultiply(*this, temp);
	}

	inline Flt4 & Flt4::operator /=(float val)
	{
		*this = *this / val;
		return *this;
	}

	inline const Flt4 Flt4::operator -() const
	{
		Flt4 temp(-1.0f, -1.0f, -1.0f, -1.0f);
		return (Flt4)XMVectorMultiply(*this, temp);
	}

	inline Flt4& Flt4::normalize() {
		*this = (Flt4)XMVector4Normalize(*this);
		return *this;
	}

	inline Flt4& Flt4::floor(int len) {
		x = bsm::floor(x, len);
		y = bsm::floor(y, len);
		z = bsm::floor(z, len);
		w = bsm::floor(w, len);
		return *this;
	}

	inline Flt4& Flt4::round(int len) {
		x = bsm::round(x, len);
		y = bsm::round(y, len);
		z = bsm::round(z, len);
		w = bsm::round(w, len);
		return *this;
	}

	inline Flt4& Flt4::ceil(int len) {
		x = bsm::ceil(x, len);
		y = bsm::ceil(y, len);
		z = bsm::ceil(z, len);
		w = bsm::ceil(w, len);
		return *this;
	}




	inline float Flt4::dot(const Flt4 & vec1)const {
		return ((Flt4)XMVector4Dot(*this, vec1)).x;
	}

	inline float Flt4::lengthSqr()const {
		return ((Flt4)XMVector4LengthSq(*this)).x;
	}

	inline float Flt4::length()const {
		return ((Flt4)XMVector4Length(*this)).x;

	}

	inline bool Flt4::isNaN() const {
		return XMVector4IsNaN(*this);
	}
	inline bool Flt4::isInfinite() const {
		return XMVector4IsInfinite(*this);
	}



	//--------------------------------------------------------------------------------------
	///	Quat�C�����C���֐�
	//--------------------------------------------------------------------------------------
	inline Quat::Quat():
		XMFLOAT4()
	{
		identity();
	}

	inline Quat::Quat(const Quat & quat):
		XMFLOAT4(quat.x, quat.y, quat.z, quat.w)
	{
	}

	inline Quat::Quat(const XMFLOAT4& v) :
		XMFLOAT4(v)
	{
	}

	inline Quat::Quat(float _x, float _y, float _z, float _w):
		XMFLOAT4(_x, _y, _z, _w)
	{
	}

	inline Quat::Quat(const Flt3& vec, float r):
		XMFLOAT4()
	{
		*this = (Quat)XMQuaternionRotationAxis(vec, r);
	}

	inline Quat::Quat(float val):
		XMFLOAT4()
	{
		x = val;
		y = val;
		z = val;
		w = val;
	}

	inline Quat::Quat(const XMVECTOR& other) :
		XMFLOAT4() 
	{
		XMVECTOR temp = other;
		XMStoreFloat4((XMFLOAT4*)this, temp);
	}

	inline Quat::Quat(const Mat3x3& m) {
		Mat4x4 m4(m);
		*this = m4.quatInMatrix();
	}

	inline Quat::Quat(const Mat4x4& m) {
		*this = m.quatInMatrix();
	}

	inline Quat::operator XMVECTOR() const {
		XMFLOAT4 temp = *this;
		XMVECTOR Vec = XMLoadFloat4(&temp);
		return Vec;
	}


	inline Quat& Quat::operator=(const XMFLOAT4& other) {
		(XMFLOAT4)*this = other;
		return *this;
	}


	inline Quat& Quat::operator=(const XMVECTOR& other) {
		XMVECTOR temp = other;
		XMStoreFloat4((XMFLOAT4*)this, temp);
		return *this;
	}

	inline Quat & Quat::operator =(const Quat & quat)
	{
		if (this != &quat) {
			x = quat.x;
			y = quat.y;
			z = quat.z;
			w = quat.w;
		}
		return *this;
	}

	inline bool Quat::operator==(const Quat& other)const {
		return XMQuaternionEqual(*this, other);
	}

	inline bool Quat::operator!=(const Quat& other)const {
		return !XMQuaternionEqual(*this, other);
	}

	inline Quat & Quat::setXYZ(const Flt3 & vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	inline const Flt3 Quat::getXYZ() const
	{
		return Flt3(x, y, z);
	}

	inline Quat & Quat::setX(float _x)
	{
		x = _x;
		return *this;
	}

	inline Quat & Quat::setY(float _y)
	{
		y = _y;
		return *this;
	}

	inline Quat & Quat::setZ(float _z)
	{
		z = _z;
		return *this;
	}

	inline Quat & Quat::setW(float _w)
	{
		w = _w;
		return *this;
	}

	inline float Quat::getX()const {
		return x;
	}
	inline float Quat::getY()const {
		return y;
	}
	inline float Quat::getZ()const {
		return z;
	}
	inline float Quat::getW()const {
		return w;
	}

	inline Quat & Quat::setElem(int idx, float value)
	{
		*(&x + idx) = value;
		return *this;
	}

	inline float Quat::getElem(int idx) const
	{
		return *(&x + idx);
	}

	inline float & Quat::operator [](int idx)
	{
		return *(&x + idx);
	}

	inline float Quat::operator [](int idx) const
	{
		return *(&x + idx);
	}

	inline const Quat Quat::operator +(const Quat & quat) const
	{
		return (Quat)XMVectorAdd(*this, quat);
	}

	inline const Quat Quat::operator -(const Quat & quat) const
	{
		return (Quat)XMVectorSubtract(*this, quat);
	}

	inline const Quat Quat::operator *(const Quat & quat) const
	{
		return (Quat)XMQuaternionMultiply(*this, quat);
	}

	inline const Quat Quat::operator *(float val) const
	{
		Quat temp(val, val, val, val);
		return (Quat)XMVectorMultiply(*this, temp);
	}

	inline Quat& Quat::operator *=(const Quat& quat) {
		*this = *this * quat;
		return *this;
	}


	inline Quat& Quat::normalize() {
		*this = (Quat)XMQuaternionNormalize(*this);
		return *this;
	}

	inline float Quat::dot(const Quat& quat)const {
		return ((Quat)XMQuaternionDot(*this, quat)).x;
	}

	inline Quat& Quat::conj(const Quat& quat) {
		*this = (Quat)XMQuaternionConjugate(quat);
		return *this;
	}

	inline Quat& Quat::identity()
	{
		*this = (Quat)XMQuaternionIdentity();
		return *this;
	}


	inline Quat&  Quat::rotationX(float radians)
	{
		*this = (Quat)XMQuaternionRotationAxis(XMVECTOR(Flt3(1,0,0)), radians);
		return *this;
	}

	inline Quat&  Quat::rotationY(float radians)
	{
		*this = (Quat)XMQuaternionRotationAxis(Flt3(0, 1, 0), radians);
		return *this;
	}

	inline Quat&  Quat::rotationZ(float radians)
	{
		*this = (Quat)XMQuaternionRotationAxis(Flt3(0, 0, 1), radians);
		return *this;
	}

	inline Quat&  Quat::rotation(float radians, const Flt3 & unitVec) {
		*this = (Quat)XMQuaternionRotationAxis(unitVec, radians);
		return *this;
	}

	inline Quat& Quat::rotation(const Flt3& unitVec, float radians) {
		*this = (Quat)XMQuaternionRotationAxis(unitVec, radians);
		return *this;
	}


	inline Quat& Quat::rotationRollPitchYawFromVector(const Flt3& rotVec) {
		*this = (Quat)XMQuaternionRotationRollPitchYawFromVector(rotVec);
		return *this;
	}

	inline const Flt3 Quat::toRotVec() const {
		Quat Temp = *this;
		Temp.normalize();
		Mat4x4 mt(Temp);
		Flt3 Rot;
		if (mt._32 == 1.0f) {
			Rot.x = XM_PI / 2.0f;
			Rot.y = 0;
			Rot.z = -atan2(mt._21, mt._11);
		}
		else if (mt._32 == -1.0f) {
			Rot.x = -XM_PI / 2.0f;
			Rot.y = 0;
			Rot.z = -atan2(mt._21, mt._11);
		}
		else {
			Rot.x = -asin(mt._32);
			Rot.y = -atan2(-mt._31, mt._33);
			Rot.z = atan2(mt._12, mt._11);
		}
		return Rot;
	}

	inline Quat& Quat::inverse() {
		*this = (Quat)XMQuaternionInverse(*this);
		return *this;
	}

	inline Quat& Quat::facing(const Flt3& norm) {
		Flt3 DefUp(0, 1.0f, 0);
		Flt3 Temp = norm;
		Flt2 TempVec2(Temp.x, Temp.z);
		if (bsm::length(TempVec2) < 0.1f) {
			DefUp = bsm::Flt3(0, 0, 1.0f);
		}
		Temp.normalize();
		Mat4x4 RotMatrix;
		RotMatrix = XMMatrixLookAtLH(bsm::Flt3(0, 0, 0), Temp, DefUp);
		RotMatrix = bsm::inverse(RotMatrix);
		*this = RotMatrix.quatInMatrix();
		(*this).normalize();
		return *this;
	}

	inline Quat& Quat::facingY(const Flt3& norm) {
		Flt3 Temp = norm;
		Temp.normalize();
		*this = XMQuaternionRotationRollPitchYaw(0, atan2(Temp.x, Temp.z), 0);
		(*this).normalize();
		return *this;
	}

	inline bool Quat::isNaN() const {
		return XMQuaternionIsNaN(*this);
	}
	inline bool Quat::isInfinite() const {
		return XMQuaternionIsInfinite(*this);
	}



	//--------------------------------------------------------------------------------------
	///	Mat3x3�C�����C���֐�
	//--------------------------------------------------------------------------------------
	inline Mat3x3::Mat3x3():
		XMFLOAT3X3()
	{
		identity();
	}

	inline Mat3x3::Mat3x3(const Mat3x3 & mat):
		XMFLOAT3X3(mat)
	{
	}

	inline Mat3x3::Mat3x3(float val):
		XMFLOAT3X3()
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				m[i][j] = val;
			}
		}
	}

	inline Mat3x3::Mat3x3(const XMFLOAT3X3& other):
		XMFLOAT3X3(other)
	{

	}

	inline Mat3x3::Mat3x3(const XMMATRIX& other):
		XMFLOAT3X3()
	{
		XMMATRIX temp = other;
		XMStoreFloat3x3((XMFLOAT3X3*)this, temp);
	}

	inline  Mat3x3::operator XMMATRIX() const {
		Mat3x3 temp = *this;
		XMMATRIX m = XMLoadFloat3x3(&temp);
		return m;
	}

	inline Mat3x3& Mat3x3::operator=(const XMFLOAT3X3& other) {
		(XMFLOAT3X3)*this = other;
		return *this;
	}


	inline Mat3x3& Mat3x3::operator=(const XMMATRIX& other) {
		XMMATRIX temp = other;
		XMStoreFloat3x3((XMFLOAT3X3*)this, temp);
		return *this;
	}

	inline Mat3x3 & Mat3x3::operator =(const Mat3x3& mat)
	{
		if (this != &mat) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					m[i][j] = mat[i][j];
				}
			}
		}
		return *this;
	}

	inline bool Mat3x3::operator==(const Mat3x3& other)const {
		for (int i = 0; i < 3; i++) {
			if (!XMVector3Equal(getMajor(i), other.getMajor(i))) {
				return false;
			}
		}
		return true;
	}

	inline bool Mat3x3::operator!=(const Mat3x3& other)const {
		for (int i = 0; i < 3; i++) {
			if (!XMVector3Equal(getMajor(i), other.getMajor(i))) {
				return true;
			}
		}
		return false;
	}



	inline Mat3x3::Mat3x3(const Quat & unitQuat):
		XMFLOAT3X3()
	{
		*this = (Mat3x3)XMMatrixRotationQuaternion(unitQuat);
	}

	inline Mat3x3::Mat3x3(const Flt3 & _major0, const Flt3 & _major1, const Flt3 & _major2):
		XMFLOAT3X3()
	{
		setMajor(0, _major0);
		setMajor(1, _major1);
		setMajor(2, _major2);
	}

	inline Mat3x3 & Mat3x3::setMajor0(const Flt3 & _major0)
	{
		setMajor(0, _major0);
		return *this;
	}

	inline Mat3x3 & Mat3x3::setMajor1(const Flt3 & _major1)
	{
		setMajor(1, _major1);
		return *this;
	}

	inline Mat3x3 & Mat3x3::setMajor2(const Flt3 & _major2)
	{
		setMajor(2, _major2);
		return *this;
	}

	inline Mat3x3 & Mat3x3::setMajor(int major, const Flt3 & vec)
	{
		m[major][0] = vec.x;
		m[major][1] = vec.y;
		m[major][2] = vec.z;
		return *this;
	}

	inline Mat3x3 & Mat3x3::setMinor(int minor, const Flt3 & vec)
	{
		setElem(0, minor, vec.getElem(0));
		setElem(1, minor, vec.getElem(1));
		setElem(2, minor, vec.getElem(2));
		return *this;
	}

	inline Mat3x3 & Mat3x3::setElem(int major, int minor, float val)
	{
		m[major][minor] = val;
		return *this;
	}

	inline float Mat3x3::getElem(int major, int minor) const
	{
		return this->getMajor(major).getElem(minor);
	}

	inline const Flt3 Mat3x3::getMajor0() const
	{
		return getMajor(0);
	}

	inline const Flt3 Mat3x3::getMajor1() const
	{
		return getMajor(1);
	}

	inline const Flt3 Mat3x3::getMajor2() const
	{
		return getMajor(2);
	}

	inline const Flt3 Mat3x3::getMajor(int major) const
	{
		Flt3 temp(m[major][0], m[major][1], m[major][2]);
		return temp;
	}

	inline const Flt3 Mat3x3::getMinor(int minor) const
	{
		return Flt3(m[0][minor], m[1][minor], m[2][minor]);
	}

	inline Flt3 & Mat3x3::operator [](int major)
	{
		return (Flt3&)(m[major][0]);
	}

	inline const Flt3 Mat3x3::operator [](int major) const
	{
		return (Flt3&)(m[major][0]);
	}


	inline const Mat3x3 Mat3x3::operator +(const Mat3x3 & mat) const
	{
		Mat3x3 temp;
		for (int i = 0; i < 3; i++) {
			temp[i] = getMajor(i) + mat.getMajor(i);
		}
		return temp;
	}

	inline const Mat3x3 Mat3x3::operator -(const Mat3x3 & mat) const
	{
		Mat3x3 temp;
		for (int i = 0; i < 3; i++) {
			temp[i] = getMajor(i) - mat.getMajor(i);
		}
		return temp;
	}

	inline const Mat3x3 Mat3x3::operator -() const
	{
		Mat3x3 temp;
		for (int i = 0; i < 3; i++) {
			temp[i] = -getMajor(i);
		}
		return temp;
	}

	inline const Mat3x3 Mat3x3::operator *(float val) const
	{
		Mat3x3 temp;
		for (int i = 0; i < 3; i++) {
			temp[i] = getMajor(i) * val;
		}
		return temp;
	}

	inline const Flt3 Mat3x3::operator *(const Flt3 & vec) const
	{
		return (Flt3)XMVector3Transform(vec, *this);
	}


	inline const Mat3x3 Mat3x3::operator *(const Mat3x3 & mat) const
	{
		return  (Mat3x3)XMMatrixMultiply(*this, mat);
	}



	inline Mat3x3 & Mat3x3::operator +=(const Mat3x3 & mat)
	{
		*this = *this + mat;
		return *this;
	}

	inline Mat3x3 & Mat3x3::operator -=(const Mat3x3 & mat)
	{
		*this = *this - mat;
		return *this;
	}

	inline Mat3x3 & Mat3x3::operator *=(float val) {
		*this = *this * val;
		return *this;
	}

	inline Mat3x3 & Mat3x3::operator *=(const Mat3x3 & mat) {
		*this = *this * mat;
		return *this;
	}


	inline Mat3x3& Mat3x3::identity()
	{
		*this =  (Mat3x3)XMMatrixIdentity();
		return *this;
	}

	inline Mat3x3& Mat3x3::scale(const Flt3 & scaleVec)
	{
		*this = (Mat3x3)XMMatrixScalingFromVector(scaleVec);
		return *this;
	}

	inline Mat3x3& Mat3x3::rotation(const Quat & unitQuat)
	{
		*this = (Mat3x3)XMMatrixRotationQuaternion(unitQuat);
		return *this;
	}

	inline Mat3x3& Mat3x3::transpose() {
		*this = (Mat3x3)XMMatrixTranspose(*this);
		return *this;
	}
	inline Mat3x3& Mat3x3::inverse() {
		XMVECTOR Vec;
		*this = (Mat3x3)XMMatrixInverse(&Vec, *this);
		return *this;
	}

	inline Mat3x3& Mat3x3::crossMatrix(const Flt3 & vec) {
		*this = Mat3x3(
			Flt3(0.0f, vec.z, -vec.y),
			Flt3(-vec.z, 0.0f, vec.x),
			Flt3(vec.y, -vec.x, 0.0f)
		);
		return *this;
	}


	inline Flt3 Mat3x3::rotXInMatrix()const {
		Flt3 ret(_11, _12, _13);
		return ret;
	}
	inline Flt3 Mat3x3::rotYInMatrix()const {
		Flt3 ret(_21, _22, _23);
		return ret;
	}
	inline Flt3 Mat3x3::rotZInMatrix()const {
		Flt3 ret(_31, _32, _33);
		return ret;
	}

	//--------------------------------------------------------------------------------------
	///	Mat4x4�C�����C���֐�
	//--------------------------------------------------------------------------------------
	inline Mat4x4::Mat4x4():
		XMFLOAT4X4()
	{
		identity();
	}


	inline Mat4x4::Mat4x4(const Mat4x4 & mat) :
		XMFLOAT4X4(mat)
	{
	}

	inline Mat4x4::Mat4x4(const Mat3x3 & mat) :
		XMFLOAT4X4()
	{
		identity();
		this->setUpper3x3(mat);
	}


	inline Mat4x4::Mat4x4(const Flt3 & _major0, const Flt3 & _major1, const Flt3 & _major2, const Flt3& _major3) :
		XMFLOAT4X4()
	{
		identity();
		setMajor(0, _major0);
		setMajor(1, _major1);
		setMajor(2, _major2);
		setMajor(3, _major3);
	}

	inline Mat4x4::Mat4x4(const Flt4 & _major0, const Flt4 & _major1, const Flt4 & _major2, const Flt4& _major3) :
		XMFLOAT4X4()
	{
		identity();
		setMajor(0, _major0);
		setMajor(1, _major1);
		setMajor(2, _major2);
		setMajor(3, _major3);
	}

	inline Mat4x4::Mat4x4(const Mat3x3 & tfrm, const Flt3 & translateVec)
	{
		identity();
		this->setUpper3x3(tfrm);
		this->setTranslation(translateVec);
	}

	inline Mat4x4::Mat4x4(const Quat & unitQuat, const Flt3 & translateVec)
	{
		identity();
		this->setUpper3x3(Mat3x3(unitQuat));
		this->setTranslation(translateVec);
	}


	inline Mat4x4::Mat4x4(float val) :
		XMFLOAT4X4()
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				m[i][j] = val;
			}
		}
	}

	inline Mat4x4::Mat4x4(const XMFLOAT4X4& other):
		XMFLOAT4X4(other)
	{

	}


	inline Mat4x4::Mat4x4(const XMMATRIX& other) :
		XMFLOAT4X4()
	{
		XMMATRIX temp = other;
		XMStoreFloat4x4((XMFLOAT4X4*)this, temp);
	}

	inline  Mat4x4::operator XMMATRIX() const {
		Mat4x4 temp = *this;
		XMMATRIX m = XMLoadFloat4x4(&temp);
		return m;
	}

	inline Mat4x4& Mat4x4::operator=(const XMFLOAT4X4& other) {
		(XMFLOAT4X4)*this = other;
		return *this;
	}

	inline Mat4x4& Mat4x4::operator=(const XMMATRIX& other) {
		XMMATRIX temp = other;
		XMStoreFloat4x4((XMFLOAT4X4*)this, temp);
		return *this;
	}

	inline bool Mat4x4::operator==(const Mat4x4& other)const {
		for (int i = 0; i < 4; i++) {
			if (!XMVector4Equal(getMajor(i), other.getMajor(i))) {
				return false;
			}
		}
		return true;
	}

	inline bool Mat4x4::operator!=(const Mat4x4& other)const {
		for (int i = 0; i < 4; i++) {
			if (!XMVector4Equal(getMajor(i), other.getMajor(i))) {
				return true;
			}
		}
		return false;
	}

	inline bool Mat4x4::equalInt(const Mat4x4& other)const {
		for (int i = 0; i < 4; i++) {
			if (!XMVector4EqualInt(getMajor(i), other.getMajor(i))) {
				return false;
			}
		}
		return true;
	}

	inline bool Mat4x4::nearEqual(const Mat4x4& other, float epsilon)const {
		Flt4 temp(epsilon, epsilon, epsilon, epsilon);
		for (int i = 0; i < 4; i++) {
			if (!XMVector4NearEqual(getMajor(i), other.getMajor(i), temp)) {
				return false;
			}
		}
		return true;

	}

	inline Mat4x4::Mat4x4(const Quat & unitQuat) :
		XMFLOAT4X4()
	{
		*this = (Mat4x4)XMMatrixRotationQuaternion(XMVECTOR(unitQuat));
	}

	inline Mat4x4 & Mat4x4::setUpper3x3(const Mat3x3 & tfrm)
	{
		this->operator[](0) = tfrm.getMajor0();
		this->operator[](1) = tfrm.getMajor1();
		this->operator[](2) = tfrm.getMajor2();
		return *this;
	}

	inline const Mat3x3 Mat4x4::getUpper3x3() const
	{
		return Mat3x3(getMajor0(), getMajor1(), getMajor2());
	}

	inline Mat4x4 & Mat4x4::setTranslation(const Flt3 & translateVec)
	{
		this->operator[](3) = translateVec;
		m[3][3] = 1.0f;
		return *this;
	}

	inline const Flt3 Mat4x4::getTranslation() const
	{
		return (Flt3)getMajor3();
	}
	inline Mat4x4 & Mat4x4::setMajor0(const Flt3 & _major0)
	{
		setMajor(0, _major0);
		m[0][3] = 0.0f;
		return *this;
	}

	inline Mat4x4 & Mat4x4::setMajor1(const Flt3 & _major1)
	{
		setMajor(1, _major1);
		m[1][3] = 0.0f;
		return *this;
	}

	inline Mat4x4 & Mat4x4::setMajor2(const Flt3 & _major2)
	{
		setMajor(2, _major2);
		m[2][3] = 0.0f;
		return *this;
	}

	inline Mat4x4 & Mat4x4::setMajor3(const Flt3 & _major3)
	{
		setMajor(3, _major3);
		m[3][3] = 1.0f;
		return *this;
	}

	inline Mat4x4 & Mat4x4::setMajor0(const Flt4 & _major0)
	{
		setMajor(0, _major0);
		return *this;
	}

	inline Mat4x4 & Mat4x4::setMajor1(const Flt4 & _major1)
	{
		setMajor(1, _major1);
		return *this;
	}

	inline Mat4x4 & Mat4x4::setMajor2(const Flt4 & _major2)
	{
		setMajor(2, _major2);
		return *this;
	}

	inline Mat4x4 & Mat4x4::setMajor3(const Flt4 & _major3)
	{
		setMajor(3, _major3);
		return *this;
	}


	inline Mat4x4 & Mat4x4::setMajor(int major, const Flt3 & vec)
	{
		m[major][0] = vec.x;
		m[major][1] = vec.y;
		m[major][2] = vec.z;
		return *this;
	}

	inline Mat4x4 & Mat4x4::setMajor(int major, const Flt4 & vec)
	{
		m[major][0] = vec.x;
		m[major][1] = vec.y;
		m[major][2] = vec.z;
		m[major][3] = vec.w;
		m[3][3] = 1.0f;
		return *this;
	}


	inline Mat4x4 & Mat4x4::setMinor(int minor, const Flt3 & vec)
	{
		setElem(0, minor, vec.getElem(0));
		setElem(1, minor, vec.getElem(1));
		setElem(2, minor, vec.getElem(2));
		return *this;
	}

	inline Mat4x4 & Mat4x4::setMinor(int minor, const Flt4 & vec)
	{
		setElem(0, minor, vec.getElem(0));
		setElem(1, minor, vec.getElem(1));
		setElem(2, minor, vec.getElem(2));
		setElem(3, minor, vec.getElem(3));
		m[3][3] = 1.0f;
		return *this;
	}
	inline Mat4x4 & Mat4x4::setElem(int major, int minor, float val)
	{
		m[major][minor] = val;
		return *this;
	}

	inline float Mat4x4::getElem(int major, int minor) const
	{
		return this->getMajor(major).getElem(minor);
	}

	inline const Flt4 Mat4x4::getMajor0() const
	{
		return getMajor(0);
	}

	inline const Flt4 Mat4x4::getMajor1() const
	{
		return getMajor(1);
	}

	inline const Flt4 Mat4x4::getMajor2() const
	{
		return getMajor(2);
	}

	inline const Flt4 Mat4x4::getMajor3() const
	{
		return getMajor(3);
	}


	inline const Flt4 Mat4x4::getMajor(int major) const
	{
		Flt4 temp(m[major][0], m[major][1], m[major][2], m[major][3]);
		return temp;
	}

	inline const Flt4 Mat4x4::getMinor(int minor) const
	{
		return Flt4(m[0][minor], m[1][minor], m[2][minor], m[3][minor]);
	}

	inline Flt4 & Mat4x4::operator [](int major)
	{
		return (Flt4&)(m[major][0]);
	}

	inline const Flt4 Mat4x4::operator [](int major) const
	{
		return (Flt4&)(m[major][0]);
	}

	inline Mat4x4 & Mat4x4::operator =(const Mat4x4& mat)
	{
		if (this != &mat) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					m[i][j] = mat[i][j];
				}
			}
		}
		return *this;
	}


	inline const Mat4x4 Mat4x4::operator +(const Mat4x4 & mat) const
	{
		Mat4x4 temp;
		for (int i = 0; i < 4; i++) {
			temp[i] = getMajor(i) + mat.getMajor(i);
		}
		temp[3][3] = 1.0f;
		return temp;
	}

	inline const Mat4x4 Mat4x4::operator -(const Mat4x4 & mat) const
	{
		Mat4x4 temp;
		for (int i = 0; i < 4; i++) {
			temp[i] = getMajor(i) - mat.getMajor(i);
		}
		temp[3][3] = 1.0f;
		return temp;
	}

	inline const Mat4x4 Mat4x4::operator -() const
	{
		Mat4x4 temp;
		for (int i = 0; i < 4; i++) {
			temp[i] = -getMajor(i);
		}
		temp[3][3] = 1.0f;
		return temp;
	}

	inline const Mat4x4 Mat4x4::operator *(float val) const
	{
		Mat4x4 temp;
		for (int i = 0; i < 4; i++) {
			temp[i] = getMajor(i) * val;
		}
		temp[3][3] = 1.0f;
		return temp;
	}

	inline const Flt3 Mat4x4::operator *(const Flt3 & vec) const
	{
		return (Flt3)XMVector3Transform(vec, *this);
	}

	inline const Flt4 Mat4x4::operator *(const Flt4 & vec) const
	{
		return (Flt4)XMVector4Transform(vec, *this);
	}



	inline const Mat4x4 Mat4x4::operator *(const Mat4x4 & mat) const
	{
		return  (Mat4x4)XMMatrixMultiply(*this, mat);
	}

	inline const Mat4x4 Mat4x4::operator *(const Mat3x3 & mat) const
	{
		return  (Mat4x4)XMMatrixMultiply(*this, mat);
	}


	inline Mat4x4 & Mat4x4::operator +=(const Mat4x4 & mat)
	{
		*this = *this + mat;
		return *this;
	}

	inline Mat4x4 & Mat4x4::operator -=(const Mat4x4 & mat)
	{
		*this = *this - mat;
		return *this;
	}

	inline Mat4x4&  Mat4x4::operator *=(float val) {
		*this = *this * val;
		return *this;
	}


	inline Mat4x4 & Mat4x4::operator *=(const Mat4x4 & tfrm)
	{
		*this = *this * tfrm;
		return *this;
	}

	inline Mat4x4& Mat4x4::operator *=(const Mat3x3 & tfrm)
	{
		*this = *this * tfrm;
		return *this;
	}

	inline void Mat4x4::decompose(Flt3& rScaling, Quat& rQt, Flt3& rTranslation)const {
		XMVECTOR Scale;
		XMVECTOR Qt;
		XMVECTOR Translation;
		if (XMMatrixDecompose(&Scale, &Qt, &Translation, *this)) {
			rScaling = Scale;
			rQt = Qt;
			rTranslation = Translation;
		}
		else {
			//�X�P�[�����O�}�C�i�X�̏ꍇ�̏���
			auto XLen = length(Flt3(_11, _12, _13));
			auto YLen = length(Flt3(_21, _22, _23));
			auto ZLen = length(Flt3(_31, _32, _33));
			//�X�P�[�����O��0�̗v�f������
			if (XLen == 0.0f) {
				XLen = 0.0001f;
			}
			if (YLen == 0.0f) {
				YLen = 0.0001f;
			}
			if (ZLen == 0.0f) {
				ZLen = 0.0001f;
			}
			rScaling = Flt3(XLen, YLen, ZLen);
			rTranslation = Flt3(_41, _42, _43);

			Flt3 vX = Flt3(_11, _12, _13) / XLen;
			Flt3 vY = Flt3(_21, _22, _23) / XLen;
			Flt3 vZ = Flt3(_31, _32, _33) / XLen;

			Mat4x4 retM;
			retM.identity();
			retM._11 = vX.x;
			retM._12 = vX.y;
			retM._13 = vX.z;

			retM._21 = vY.x;
			retM._22 = vY.y;
			retM._23 = vY.z;

			retM._31 = vZ.x;
			retM._32 = vZ.y;
			retM._33 = vZ.z;

			rQt = (Quat)XMQuaternionRotationMatrix(retM);

		}
	}

	inline Flt3 Mat4x4::scaleInMatrix()const {
		Flt3 scale, pos;
		Quat qt;
		decompose(scale, qt, pos);
		return scale;
	}
	inline Quat Mat4x4::quatInMatrix()const {
		Flt3 scale, pos;
		Quat qt;
		decompose(scale, qt, pos);
		return qt;
	}
	inline Flt3 Mat4x4::transInMatrix()const {
		Flt3 pos;
		pos.x = _41;
		pos.y = _42;
		pos.z = _43;
		return pos;
	}



	inline Mat4x4& Mat4x4::identity()
	{
		*this = (Mat4x4)XMMatrixIdentity();
		return *this;
	}

	inline Mat4x4&  Mat4x4::scaleIdentity() {
		Flt3 Scale, Translation;
		Quat Qt;
		decompose(Scale, Qt, Translation);
		Scale = Flt3(1.0f, 1.0f, 1.0f);
		return affineTransformation(Scale, Flt3(0.0f,0.0f,0.0f), Qt, Translation);
	}


	inline Mat4x4& Mat4x4::scale(const Flt3 & scaleVec)
	{
		*this = (Mat4x4)XMMatrixScalingFromVector(scaleVec);
		return *this;
	}

	inline Mat4x4& Mat4x4::rotation(const Quat & unitQuat)
	{
		*this = (Mat4x4)XMMatrixRotationQuaternion(unitQuat);
		return *this;
	}

	inline Mat4x4& Mat4x4::translation(const Flt3 & transVec) {
		*this = (Mat4x4)XMMatrixTranslationFromVector(transVec);
		return *this;
	}

	inline Mat4x4& Mat4x4::transpose() {
		*this = (Mat4x4)XMMatrixTranspose(*this);
		return *this;
	}
	inline Mat4x4& Mat4x4::inverse() {
		XMVECTOR Vec;
		*this = (Mat4x4)XMMatrixInverse(&Vec, *this);
		return *this;
	}

	inline Mat4x4& Mat4x4::affineTransformation2D(
		const Flt2 & scaleVec,
		const Flt2 & rotOrigin,
		const float & rotation,
		const Flt2 & transVec) {
		*this = (Mat4x4)XMMatrixAffineTransformation2D(scaleVec, rotOrigin,
			rotation, transVec);
		return *this;
	}


	inline Mat4x4& Mat4x4::affineTransformation(
		const Flt3 & scaleVec,
		const Flt3 & rotOrigin,
		const Quat & unitQuat,
		const Flt3 & transVec) {
		*this = (Mat4x4)XMMatrixAffineTransformation(scaleVec, rotOrigin,
			unitQuat, transVec);
		return *this;
	}

	inline Mat4x4& Mat4x4::affineTransformation(
		const Flt3 & scaleVec,
		const Flt3 & rotOrigin,
		const Flt3 & rotVec,
		const Flt3 & transVec) {
		Quat Qt = (Quat)XMQuaternionRotationRollPitchYawFromVector(rotVec);
		Qt.normalize();
		return affineTransformation(scaleVec, rotOrigin, Qt, transVec);
	}

	inline Mat4x4& Mat4x4::lookatLH(
		const Flt3 & eye,
		const Flt3 & at,
		const Flt3 & up) {
		*this = (Mat4x4)XMMatrixLookAtLH(eye, at,up);
		return *this;
	}

	inline Mat4x4& Mat4x4::orthographicLH(
		float width,
		float height,
		float n,
		float f) {
		*this = (Mat4x4)XMMatrixOrthographicLH(width, height, n, f);
		return *this;
	}


	inline Mat4x4& Mat4x4::strTransformation(
		const Flt3 & scaleVec,
		const Flt3 & transVec,
		const Quat & unitQuat
	) {
		Mat4x4 Scale = (Mat4x4)XMMatrixScalingFromVector(scaleVec);
		Mat4x4 Trans = (Mat4x4)XMMatrixTranslationFromVector(transVec);
		Mat4x4 Qt = (Mat4x4)XMMatrixRotationQuaternion(unitQuat);
		*this = Scale * Trans * Qt;
		return *this;
	}

	inline Flt3 Mat4x4::rotXInMatrix()const {
		Flt3 ret(_11, _12, _13);
		Flt3 Scale = scaleInMatrix();
		ret.x /= Scale.x;
		ret.y /= Scale.x;
		ret.z /= Scale.x;
		return ret;
	}
	inline Flt3 Mat4x4::rotYInMatrix()const {
		Flt3 ret(_21, _22, _23);
		Flt3 Scale = scaleInMatrix();
		ret.x /= Scale.y;
		ret.y /= Scale.y;
		ret.z /= Scale.y;
		return ret;
	}
	inline Flt3 Mat4x4::rotZInMatrix()const {
		Flt3 ret(_31, _32, _33);
		Flt3 Scale = scaleInMatrix();
		ret.x /= Scale.z;
		ret.y /= Scale.z;
		ret.z /= Scale.z;
		return ret;
	}

	//--------------------------------------------------------------------------------------
	///	���[�e�B���e�B�֐��Qfloat
	//--------------------------------------------------------------------------------------
	inline float floor(float f, int len) {
		double ret;

		ret = (double)f * pow(10.0, len);
		ret = (double)(int)(ret);

		return (float)(ret * pow(10.0, -len));

	}

	inline float round(float f, int len) {
		double	ret;

		ret = (double)f * pow(10.0, len);
		ret = (double)(int)(ret + 0.5);

		return (float)(ret * pow(10.0, -len));

	}

	inline float ceil(float f, int len) {
		double	ret;

		ret = (double)f * pow(10.0, len);
		ret = (double)(int)(ret + 0.9);

		return (float)(ret * pow(10.0, -len));

	}

	//--------------------------------------------------------------------------------------
	///	���[�e�B���e�B�֐��QFlt2
	//--------------------------------------------------------------------------------------
	inline const Flt2 operator *(float val, const Flt2 & vec)
	{
		Flt2 temp( val, val);
		return (Flt2)XMVectorMultiply(vec, temp);
	}
	inline const Flt2 operator /(float val, const Flt2 & vec)
	{
		Flt2 temp(XMVectorReciprocal(Flt2(val, val)));
		return (Flt2)XMVectorMultiply(vec, temp);
	}


	inline const Flt2 mulPerElem(const Flt2 & vec0, const Flt2 & vec1)
	{
		return (Flt2)XMVectorMultiply(vec0, vec1);
	}

	inline const Flt2 divPerElem(const Flt2 & vec0, const Flt2 & vec1)
	{
		Flt2 temp(XMVectorReciprocal(vec1));
		return (Flt2)XMVectorMultiply(vec0, temp);
	}


	inline const Flt2 absPerElem(const Flt2 & vec)
	{
		return (Flt2)XMVectorAbs(vec);
	}

	inline const Flt2 maxPerElem(const Flt2 & vec0, const Flt2 & vec1)
	{
		return (Flt2)XMVectorMax(vec0, vec1);
	}

	inline const Flt2 minPerElem(const Flt2 & vec0, const Flt2 & vec1)
	{
		return (Flt2)XMVectorMin(vec0, vec1);
	}

	inline float dot(const Flt2 & vec0, const Flt2 & vec1)
	{
		return ((Flt2)XMVector2Dot(vec0, vec1)).x;
	}

	inline float lengthSqr(const Flt2 & vec)
	{
		return ((Flt2)XMVector2LengthSq(vec)).x;
	}

	inline float length(const Flt2 & vec)
	{
		return ((Flt2)XMVector2Length(vec)).x;
	}

	inline const Flt2 normalize(const Flt2 & vec)
	{
		return (Flt2)XMVector2Normalize(vec);
	}


	//--------------------------------------------------------------------------------------
	///	���[�e�B���e�B�֐��QFlt3
	//--------------------------------------------------------------------------------------
	inline const Flt3 operator *(float val, const Flt3 & vec)
	{
		Flt3 temp(val, val, val);
		return (Flt3)XMVectorMultiply(vec, temp);
	}
	inline const Flt3 operator /(float val, const Flt3 & vec)
	{
		Flt3 temp(XMVectorReciprocal(Flt3(val,val,val)));
		return (Flt3)XMVectorMultiply(vec, temp);
	}


	inline const Flt3 mulPerElem(const Flt3 & vec0, const Flt3 & vec1)
	{
		return (Flt3)XMVectorMultiply(vec0, vec1);
	}

	inline const Flt3 divPerElem(const Flt3 & vec0, const Flt3 & vec1)
	{
		Flt3 temp(XMVectorReciprocal(vec1));
		return (Flt3)XMVectorMultiply(vec0, temp);
	}


	inline const Flt3 absPerElem(const Flt3 & vec)
	{
		return (Flt3)XMVectorAbs(vec);
	}

	inline const Flt3 maxPerElem(const Flt3 & vec0, const Flt3 & vec1)
	{
		return (Flt3)XMVectorMax(vec0, vec1);
	}

	inline const Flt3 minPerElem(const Flt3 & vec0, const Flt3 & vec1)
	{
		return (Flt3)XMVectorMin(vec0, vec1);
	}

	inline float dot(const Flt3 & vec0, const Flt3 & vec1)
	{
		return ((Flt3)XMVector3Dot(vec0, vec1)).x;
	}

	inline float lengthSqr(const Flt3 & vec)
	{
		return ((Flt3)XMVector3LengthSq(vec)).x;
	}

	inline float length(const Flt3 & vec)
	{
		return ((Flt3)XMVector3Length(vec)).x;
	}

	inline const Flt3 normalize(const Flt3 & vec)
	{
		return (Flt3)XMVector3Normalize(vec);
	}

	inline const Flt3 cross(const Flt3 & vec0, const Flt3 & vec1)
	{
		return (Flt3)XMVector3Cross(vec0, vec1);
	}

	inline const Flt3 reflect(const Flt3 & vec, const Flt3 & normal)
	{
		return (Flt3)XMVector3Reflect(vec, normal);
	}

	inline const Flt3 slide(const Flt3 & vec, const Flt3 & normal)
	{
		//vec�Ɩ@�����璼�s���̒����i���ςŋ��߂�j
		float Len = bsm::dot(vec, normal);
		//���̒����ɐL�΂�
		Flt3 Contact = normal * Len;
		//�X���C�h��������͌��݂̃x�N�g����������Z
		return (vec - Contact);
	}

	inline float maxElem(const Flt3 & vec)
	{
		float result;
		result = (vec.x > vec.y) ? vec.x : vec.y;
		result = (vec.z > result) ? vec.z : result;
		return result;
	}

	inline float minElem(const Flt3 & vec)
	{
		float result;
		result = (vec.x < vec.y) ? vec.x : vec.y;
		result = (vec.z < result) ? vec.z : result;
		return result;
	}

	inline float angleBetweenNormals(const Flt3 & norm11, const Flt3 & norm2) {
		return ((Flt3)XMVector3AngleBetweenNormals(norm11, norm2)).x;
	}

	//--------------------------------------------------------------------------------------
	///	���[�e�B���e�B�֐��QFlt4
	//--------------------------------------------------------------------------------------
	inline const Flt4 operator *(float val, const Flt4 & vec)
	{
		Flt4 temp(val, val, val, val);
		return (Flt4)XMVectorMultiply(vec, temp);
	}
	inline const Flt4 operator /(float val, const Flt4 & vec)
	{
		Flt4 temp(XMVectorReciprocal(Flt4(val, val, val, val)));
		return (Flt4)XMVectorMultiply(vec, temp);
	}


	inline const Flt4 mulPerElem(const Flt4 & vec0, const Flt4 & vec1)
	{
		return (Flt4)XMVectorMultiply(vec0, vec1);
	}

	inline const Flt4 divPerElem(const Flt4 & vec0, const Flt4 & vec1)
	{
		Flt4 temp(XMVectorReciprocal(vec1));
		return (Flt4)XMVectorMultiply(vec0, temp);
	}


	inline const Flt4 absPerElem(const Flt4 & vec)
	{
		return (Flt4)XMVectorAbs(vec);
	}

	inline const Flt4 maxPerElem(const Flt4 & vec0, const Flt4 & vec1)
	{
		return (Flt4)XMVectorMax(vec0, vec1);
	}

	inline const Flt4 minPerElem(const Flt4 & vec0, const Flt4 & vec1)
	{
		return (Flt4)XMVectorMin(vec0, vec1);
	}

	inline float dot(const Flt4 & vec0, const Flt4 & vec1)
	{
		return ((Flt4)XMVector4Dot(vec0, vec1)).x;
	}

	inline float lengthSqr(const Flt4 & vec)
	{
		return ((Flt4)XMVector4LengthSq(vec)).x;
	}

	inline float length(const Flt4 & vec)
	{
		return ((Flt4)XMVector4Length(vec)).x;
	}

	inline const Flt4 normalize(const Flt4 & vec)
	{
		return (Flt4)XMVector4Normalize(vec);
	}
	//--------------------------------------------------------------------------------------
	///	���[�e�B���e�B�֐��QQuat�֘A
	//--------------------------------------------------------------------------------------
	inline const Quat normalize(const Quat & quat)
	{
		return (Quat)XMQuaternionNormalize(quat);
	}
	inline const Flt3 rotate(const Quat & quat, const Flt3 & vec)
	{
		return (Flt3)XMVector3Rotate(vec, quat);
	}

	inline const Quat rotation(const Flt3 & unitVec0, const Flt3 & unitVec1)
	{
		float cosHalfAngleX2, recipCosHalfAngleX2;
		cosHalfAngleX2 = sqrtf((2.0f * (1.0f + dot(unitVec0, unitVec1))));
		recipCosHalfAngleX2 = (1.0f / cosHalfAngleX2);
		return Quat((cross(unitVec0, unitVec1) * recipCosHalfAngleX2), (cosHalfAngleX2 * 0.5f));
	}


	inline const Quat conj(const Quat & quat) {
		return (Quat)XMQuaternionConjugate(quat);
	}

	inline const Quat inverse(const Quat & quat) {
		return (Quat)XMQuaternionInverse(quat);
	}

	inline float dot(const Quat& quat0, const Quat& quat1) {
		return ((Quat)XMQuaternionDot(quat0, quat1)).x;
	}


	inline const Quat facing(const Flt3& norm) {
		Quat ret;
		ret.facing(norm);
		return ret;
	}

	inline const Quat facingY(const Flt3& norm) {
		Quat ret;
		ret.facingY(norm);
		return ret;
	}




	//--------------------------------------------------------------------------------------
	///	���[�e�B���e�B�֐��QMat3x3�֘A
	//--------------------------------------------------------------------------------------
	inline const Mat3x3 absPerElem(const Mat3x3 & mat)
	{
		return Mat3x3(
			absPerElem(mat.getMajor0()),
			absPerElem(mat.getMajor1()),
			absPerElem(mat.getMajor2())
		);
	}

	inline const Mat3x3 operator *(float val, const Mat3x3 & mat)
	{
		return mat * val;
	}

	inline const Mat3x3 transpose(const Mat3x3 & mat)
	{
		return  (Mat3x3)XMMatrixTranspose(mat);
	}

	inline const Mat3x3 inverse(const Mat3x3 & mat)
	{
		XMVECTOR Vec;
		return (Mat3x3)XMMatrixInverse(&Vec, mat);
	}

	inline const Mat3x3 crossMatrix(const Flt3 & vec)
	{
		return Mat3x3(
			Flt3(0.0f, vec.z, -vec.y),
			Flt3(-vec.z, 0.0f, vec.x),
			Flt3(vec.y, -vec.x, 0.0f)
		);
	}

	//--------------------------------------------------------------------------------------
	///	���[�e�B���e�B�֐��QMat4x4�֘A
	//--------------------------------------------------------------------------------------
	inline const Mat4x4 absPerElem(const Mat4x4 & mat)
	{
		return Mat4x4(
			absPerElem(mat.getMajor0()),
			absPerElem(mat.getMajor1()),
			absPerElem(mat.getMajor2()),
			absPerElem(mat.getMajor3())
		);
	}

	inline const Mat4x4 operator *(float val, const Mat4x4 & mat)
	{
		return mat * val;
	}

	inline const Mat4x4 transpose(const Mat4x4 & mat)
	{
		return  (Mat4x4)XMMatrixTranspose(mat);
	}

	inline const Mat4x4 inverse(const Mat4x4 & mat)
	{
		XMVECTOR Vec;
		return (Mat4x4)XMMatrixInverse(&Vec, mat);
	}

	inline const Mat4x4 orthoInverse(const bsm::Mat4x4 & tfrm)
	{
		Flt3 inv0, inv1, inv2;
		inv0 = Flt3(tfrm.getMajor0().x, tfrm.getMajor1().x, tfrm.getMajor2().x);
		inv1 = Flt3(tfrm.getMajor0().y, tfrm.getMajor1().y, tfrm.getMajor2().y);
		inv2 = Flt3(tfrm.getMajor0().z, tfrm.getMajor1().z, tfrm.getMajor2().z);
		return Mat4x4(
			inv0,
			inv1,
			inv2,
			Flt3((-((inv0 * tfrm.getMajor3().x) + ((inv1 * tfrm.getMajor3().y) + (inv2 * tfrm.getMajor3().z)))))
		);
	}

	using Vec2 = Flt2;
	using Vec3 = Flt3;
	using Vec4 = Flt4;
	using Pt2 = Flt2;
	using Pt3 = Flt3;
	using Col4 = Flt4;
	using Plane4 = Flt4;


}
// end bsm
}
//end basedx12
