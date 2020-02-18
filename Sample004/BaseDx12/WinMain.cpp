

#include "stdafx.h"
#include "Scene.h"

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	int RetCode = 0;
	try {
		if (FAILED(::CoInitialize(nullptr))) {
			MessageBoxA(nullptr, "Com�������Ɏ��s���܂����B", "�G���[", MB_OK);
			return 1;
		}
		// �f�o�b�O���Adelete����̃`�F�b�N�p
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		setlocale(LC_ALL, "JPN");
		basedx12::Scene scene;
		RetCode = basedx12::App::Run(&scene, hInstance, nCmdShow, 1280, 720);
	}
	catch (exception & e) {
		//STL�G���[
		MessageBoxA(nullptr, e.what(), "�G���[", MB_OK);
		RetCode = 1;
	}
	catch (...) {
		//STL�G���[
		MessageBoxA(nullptr, "�����s���̃G���[", "�G���[", MB_OK);
		RetCode = 1;
	}
	::CoUninitialize();
	return RetCode;
}
