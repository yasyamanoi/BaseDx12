#pragma once
#include "stdafx.h"

namespace basedx12 {

	class SceneBase;
	class Dx12Device;

	//--------------------------------------------------------------------------------------
	///	アプリケーションクラス
	//--------------------------------------------------------------------------------------
	class App
	{
	public:
		static void Init(HWND hwd, SceneBase* pSceneBase, HINSTANCE hInstance, int nCmdShow, int width, int height);
		static void UpdateDraw();
		static void Destroy();
		static HWND GetHwnd() { return m_hwnd; }
		static const int GetGameWidth() { return m_width; }
		static const int GetGameHeight() { return m_height; }
		static const wstring& GetClasssName() { return m_classsName; }
		static const wstring& GetTitle() { return m_title; }
		static const wstring& GetModulePath() { return m_wstrModulePath; }
		static const wstring& GetModuleDir() { return m_wstrDir; }
		static const wstring& GetDataPath() { return m_wstrDataPath; }
		static const wstring& GetShadersPath() { return m_wstrShadersPath; }
		static const wstring& GetRelativeDataPath() { return m_wstrRelativeDataPath; }
		static const wstring& GetRelativeShadersPath() { return m_wstrRelativeShadersPath; }
		static const wstring& GetRelativeAssetsPath() { return m_wstrRelativeAssetsPath; }

		static SceneBase& GetSceneBase() {
			return *m_pSceneBase;
		}
		static shared_ptr<Dx12Device>& GetDx12Device() {
			return m_device;
		}
		static void SetDx12Device(const shared_ptr<Dx12Device>& ptr) {
			m_device = ptr;
		}
		static ComPtr<ID3D12Device> GetID3D12Device() {
			return m_device->GetID3D12Device();
		}

		static float GetElapsedTime() {
			double d = m_timer.GetElapsedSeconds();
			return (float)d;
		}

		static const vector<CONTROLER_STATE>& GetControlers() {
			return m_controler.GetControlers();
		}
	protected:
	private:
		App() {}
		~App() {}
		static void SetAssetsPath();
		static HWND m_hwnd;
		static SceneBase* m_pSceneBase;

		static shared_ptr<Dx12Device> m_device;

		static int m_width;
		static int m_height;
		static wstring m_classsName;
		static wstring m_title;

		static wstring m_wstrModulePath;		///< モジュール名フルパス
		static wstring m_wstrDir;				///< モジュールがあるディレクトリ
		static wstring m_wstrDataPath;			///< 絶対パスのメディアディレクトリ
		static wstring m_wstrShadersPath;		///< 絶対パスのシェーダディレクトリ
		static wstring m_wstrRelativeDataPath;	///< 相対パスのメディアディレクトリ
		static wstring m_wstrRelativeShadersPath;	///< 相対パスのシェーダディレクトリ
		static wstring	m_wstrRelativeAssetsPath;	///< 相対パスのアセットディレクトリ

		static StepTimer m_timer;										///< タイマー
		static Controler m_controler;               ///<コントローラ
	};

}
//end basedx12

