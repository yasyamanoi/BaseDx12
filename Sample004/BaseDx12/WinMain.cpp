

#include "stdafx.h"
#include "Scene.h"

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	int RetCode = 0;
	try {
		if (FAILED(::CoInitialize(nullptr))) {
			MessageBoxA(nullptr, "Com初期化に失敗しました。", "エラー", MB_OK);
			return 1;
		}
		// デバッグ時、deleteもれのチェック用
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		setlocale(LC_ALL, "JPN");
		basedx12::Scene scene;
		RetCode = basedx12::App::Run(&scene, hInstance, nCmdShow, 1280, 720);
	}
	catch (exception & e) {
		//STLエラー
		MessageBoxA(nullptr, e.what(), "エラー", MB_OK);
		RetCode = 1;
	}
	catch (...) {
		//STLエラー
		MessageBoxA(nullptr, "原因不明のエラー", "エラー", MB_OK);
		RetCode = 1;
	}
	::CoUninitialize();
	return RetCode;
}
