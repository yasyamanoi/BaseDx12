
#include "stdafx.h"
#include "Scene.h"

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	try{
		basedx12::Scene scene;
		return basedx12::App::Run(&scene, hInstance, nCmdShow, 1280, 720);
	}
	catch (exception & e) {
		//STLエラー
		MessageBoxA(nullptr, e.what(), "エラー", MB_OK);
		return 1;
	}
    catch (...) {
		//STLエラー
		MessageBoxA(nullptr, "原因不明のエラー", "エラー", MB_OK);
		return 1;
    }
	return 1;
}
