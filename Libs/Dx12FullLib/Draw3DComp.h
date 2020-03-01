#pragma once
#include "stdafx.h"

namespace basedx12 {

    DECLARE_DX12SHADER(VSPNTStatic)
    DECLARE_DX12SHADER(PSPNTStatic)


	//--------------------------------------------------------------------------------------
	///	Basicシェーダー用コンスタントバッファ
	//--------------------------------------------------------------------------------------
	struct BasicConstants
	{
		Float4 diffuseColor;
		Float4 emissiveColor;
		Float4 specularColorAndPower;

		Float4 lightDirection[3];
		Float4 lightDiffuseColor[3];
		Float4 lightSpecularColor[3];

		Float4 eyePosition;

		Float4 fogColor;
		Float4 fogVector;

		Mat4x4 world;
		Float4 worldInverseTranspose[3];
		Mat4x4 worldViewProj;
		//汎用フラグ
		XMUINT4 activeFlg;
		//以下影
		Float4 lightPos;
		Float4 eyePos;
		Mat4x4 lightView;
		Mat4x4 lightProjection;

		Float4 bones[3 * 72];

	};

	//--------------------------------------------------------------------------------------
	///	3D描画コンポーネント
	//--------------------------------------------------------------------------------------
	class Draw3D : public DrawComponent {
	protected:
		//エミッシブ色
		Float4 m_emissive;
		//デフューズ色
		Float4 m_diffuse;
		//透明度
		float m_alpha;
		//スペキュラーとパワー色
		Float4 m_specularColorAndPower;
		//コンスタントバッファの実体
		BasicConstants m_constantBufferData;
		//カメラのキー
		wstring m_cameraKey;
		Draw3D(const shared_ptr<GameObject>& gameObjectPtr):
			DrawComponent(gameObjectPtr),
			m_emissive(0.0f),
			m_diffuse(1.0f),
			m_alpha(1.0f),
			m_specularColorAndPower(0,0,0,1)
		{}
		virtual ~Draw3D() {}
	public:
		const wstring& GetCameraKey() const {
			return m_cameraKey;
		}
		void SetCameraKey(const wstring& key) {
			m_cameraKey = key;
		}
		Float4 GetEmissive() const {return m_emissive;}
		void SetEmissive(const Float4& e) { m_emissive = e; }
		Float4 GetDiffuse() const { return m_diffuse; }
		void SetDiffuse(const Float4& d) { m_diffuse = d; }
		Float3 GetSpecular() const {
			Float3 sp(m_specularColorAndPower);
			return sp; 
		}
		void SetSpecular(const Float3& sp) {
			m_specularColorAndPower.x = sp.x;
			m_specularColorAndPower.y = sp.y;
			m_specularColorAndPower.z = sp.z;
		}
		float GetPower() const { return m_specularColorAndPower.z; }
		void SetPower(float f) { m_specularColorAndPower.z = f; }
		Float4 GetSpecularColorAndPower() const { return m_specularColorAndPower; }
		void SetSpecularColorAndPower(const Float4& sp) { m_specularColorAndPower = sp; }
		float GetAlpha() const { return m_alpha; }
		void SetAlpha(float f) { m_alpha = f; }

	};


    //--------------------------------------------------------------------------------------
    ///	PNT頂点Static描画コンポーネント
    //--------------------------------------------------------------------------------------
    class PNTStaticDraw : public Draw3D {
        //テクスチャ
        shared_ptr<Dx12Texture> m_texture;
        wstring m_textureFileName;
		//ライトセットのキー
		wstring m_lightSetKey;
		void SetConstants();
	public:
        explicit PNTStaticDraw(const shared_ptr<GameObject>& gameObjectPtr);
        virtual ~PNTStaticDraw();
        void SetTextureFile(const wstring& textureName);
		const wstring& GetLightSetKey() const {
			return m_lightSetKey;
		}
		void SetLightSetKey(const wstring& key) {
			m_lightSetKey = key;
		}
        virtual void OnInit()override;
        virtual void OnUpdate()override {}
        virtual void OnDraw()override;
        virtual void OnDestroy()override {}
    };


}
//end basedx12
