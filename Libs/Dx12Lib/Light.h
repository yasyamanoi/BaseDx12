#pragma once
#include "stdafx.h"


namespace basedx12 {

	//--------------------------------------------------------------------------------------
	//	ライト
	//--------------------------------------------------------------------------------------
	struct Light {
		Float3 m_directional;	//ライトの向き
		Float4 m_diffuseColor;	//ディフィーズ色
		Float4 m_specularColor;	//スペキュラー色
		Light() :
			m_directional(0, -1.0f, 0),
			m_diffuseColor(1.0f, 1.0f, 1.0f, 1.0f),
			m_specularColor(0.2f, 0.2f, 0.2f, 1.0f)
		{
		}
		Light(const Float3& dir, const Float4& def, const Float4& sp) :
			m_directional(dir),
			m_diffuseColor(def),
			m_specularColor(sp)
		{
		}
		Light(const Light& other) :
			m_directional(other.m_directional),
			m_diffuseColor(other.m_diffuseColor),
			m_specularColor(other.m_specularColor)
		{
		}
		Light& operator=(const Light& other) {
			if (this != &other) {
				m_directional = other.m_directional;
				m_diffuseColor = other.m_diffuseColor;
				m_specularColor = other.m_specularColor;
			}
			return *this;
		}
		~Light() {}
		void SetPositionToDirectional(const Float3& pos, const Float3& at) {
			Float3 dir = at - pos;
			dir.normalize();
			m_directional = dir;
		}
	};

	//--------------------------------------------------------------------------------------
	//	ライトのセット
	//--------------------------------------------------------------------------------------
	class LightSet {
		const size_t m_maxLights = 3;
		vector<Light> m_lights;
		Float4 m_ambient;
		size_t m_mainIndex;
		LightSet() {}
	public:
		virtual ~LightSet() {}
		const Light& GetLight(size_t index) const;
		size_t GetNumLights() const {
			return m_lights.size();
		}
		void SetLight(size_t index,const Light& light);
		void AddLight(const Light& light);
		Float4 GetAmbient() const { return m_ambient; }
		void SetAmbient(const Float4& a) { m_ambient = a; }
		size_t GetMainIndex() const {return m_mainIndex;}
		void SetMainIndex(size_t index) { m_mainIndex = index; }

		static shared_ptr<LightSet>
		CreateDefaultLights();
	};



}
// end basedx12
