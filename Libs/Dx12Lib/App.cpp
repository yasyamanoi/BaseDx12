#include "stdafx.h"

namespace basedx12 {

	HWND App::m_hwnd = nullptr;
	SceneBase* App::m_pSceneBase = nullptr;
	//サイズはWinMainから渡される
	int App::m_width = 0;
	int App::m_height = 0;

	shared_ptr<BaseDevice> App::m_baseDevice = nullptr;

	StepTimer App::m_timer;
	Controler App::m_controler;


	wstring App::m_classsName = L"BaseDx12Class";
	wstring App::m_title = L"BaseDx12Title";

	wstring App::m_wstrModulePath;		///< モジュール名フルパス
	wstring App::m_wstrDir;				///< モジュールがあるディレクトリ
	wstring App::m_wstrDataPath;			///< 絶対パスのメディアディレクトリ
	wstring App::m_wstrShadersPath;		///< 絶対パスのシェーダディレクトリ
	wstring App::m_wstrRelativeDataPath;	///< 相対パスのメディアディレクトリ
	wstring App::m_wstrRelativeShadersPath;	///< 相対パスのシェーダディレクトリ
	wstring App::m_wstrRelativeAssetsPath;	///< 相対パスのアセットディレクトリ


	void App::SetAssetsPath() {
		//基準ディレクトリの設定
		//相対パスにすると、ファイルダイアログでカレントパスが狂うので
		//絶対パス指定
		wchar_t Modulebuff[MAX_PATH];
		wchar_t Drivebuff[_MAX_DRIVE];
		wchar_t Dirbuff[_MAX_DIR];
		wchar_t FileNamebuff[_MAX_FNAME];
		wchar_t Extbuff[_MAX_EXT];

		::ZeroMemory(Modulebuff, sizeof(Modulebuff));
		::ZeroMemory(Drivebuff, sizeof(Drivebuff));
		::ZeroMemory(Dirbuff, sizeof(Dirbuff));
		::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
		::ZeroMemory(Extbuff, sizeof(Extbuff));

		//モジュール名（プログラムファイル名）を得る
		if (!::GetModuleFileName(nullptr, Modulebuff, sizeof(Modulebuff))) {
			throw runtime_error("モジュールが取得できません。");
		}
		m_wstrModulePath = Modulebuff;
		//モジュール名から、各ブロックに分ける
		_wsplitpath_s(Modulebuff,
			Drivebuff, _MAX_DRIVE,
			Dirbuff, _MAX_DIR,
			FileNamebuff, _MAX_FNAME,
			Extbuff, _MAX_EXT);

		//ドライブ名の取得
		m_wstrDir = Drivebuff;
		//ディレクトリ名の取得
		m_wstrDir += Dirbuff;
		//mediaディレクトリを探す
		m_wstrDataPath = m_wstrDir;
		m_wstrDataPath += L"media";
		//まず、実行ファイルと同じディレクトリを探す
		DWORD RetCode;
		RetCode = GetFileAttributes(m_wstrDataPath.c_str());
		if (RetCode == 0xFFFFFFFF) {
			//失敗した
			m_wstrDataPath = m_wstrDir;
			m_wstrDataPath += L"..\\media";
			RetCode = GetFileAttributes(m_wstrDataPath.c_str());
			if (RetCode == 0xFFFFFFFF) {
				//再び失敗した
				throw runtime_error("mediaディレクトリを確認できません。");
			}
			else {
				m_wstrDataPath += L"\\";
				//相対パスの設定
				m_wstrRelativeDataPath = L"..\\media\\";
			}
		}
		else {
			m_wstrDataPath += L"\\";
			//相対パスの設定
			m_wstrRelativeDataPath = L"media\\";
		}
		m_wstrShadersPath = m_wstrDataPath + L"Shaders\\";
		m_wstrRelativeShadersPath = m_wstrRelativeDataPath + L"Shaders\\";


		//Assetsディレクトリを探す
		m_wstrRelativeAssetsPath = m_wstrDir;
		m_wstrRelativeAssetsPath += L"..\\..\\Assets";
		//相対ディレクトリを探す
		RetCode = GetFileAttributes(m_wstrRelativeAssetsPath.c_str());
		if (RetCode == 0xFFFFFFFF) {
			//失敗した
			//アセットディレクトリをメディアディレクトリにする
			m_wstrRelativeAssetsPath = m_wstrRelativeDataPath;
		}
		else {
			//成功した
			m_wstrRelativeAssetsPath += L"\\";
		}
	}
	void App::Init(HWND hwd, SceneBase* pSceneBase, HINSTANCE hInstance, int nCmdShow, int width, int height)
	{
		m_width = width;
		m_height = height;
		m_pSceneBase = pSceneBase;
		m_hwnd = hwd;
		SetAssetsPath();
		//デバイスの初期化はシーンから呼ばれる
		m_pSceneBase->OnInit();
	}

	void  App::UpdateDraw() {
		m_timer.Tick();
		m_controler.ResetControlerState();
		if (m_pSceneBase && m_baseDevice)
		{
			m_baseDevice->OnUpdate();
			m_baseDevice->OnDraw();
		}
	}

	void App::Destroy() {
		if (m_pSceneBase) {
			m_pSceneBase->OnDestroy();
		}
		if (m_baseDevice) {
			m_baseDevice->OnDestroy();
			m_baseDevice.reset();
		}
		m_pSceneBase = nullptr;
	}


}
//end basedx12

