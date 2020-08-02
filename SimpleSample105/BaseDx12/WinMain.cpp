
#include "stdafx.h"
#include "Scene.h"
#include "resource.h"

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

//�萔
const wchar_t* pClassName = L"BaseDx12Class";
const wchar_t* pWndTitle = L"BaseDx12Title";
//���ƍ���
int g_ClientWidth = 1280;
int g_ClientHeight = 720;


//--------------------------------------------------------------------------------------
//
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
//
//--------------------------------------------------------------------------------------
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_ICON1);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = pClassName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_ICON1);
	return RegisterClassEx(&wcex);
}

//--------------------------------------------------------------------------------------
//
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//--------------------------------------------------------------------------------------
HWND InitInstance(HINSTANCE hInstance, int nCmdShow, int iClientWidth, int iClientHeight)
{

	HWND hWnd = 0;
	// �E�B���h�E�̍쐬
	RECT rc = { 0, 0, iClientWidth, iClientHeight };
	//�E�C���h�E�傫���̒���
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	//�E�C���h�E�̍쐬
	hWnd = CreateWindow(
		pClassName,				// �o�^����Ă���N���X��
		pWndTitle,				// �E�C���h�E��
		WS_OVERLAPPEDWINDOW,	// �E�C���h�E�X�^�C���i�I�[�o�[���b�v�E�C���h�E���쐬�j
		CW_USEDEFAULT,			//�ʒu��Windows�ɔC����
		CW_USEDEFAULT,			//�ʒu��Windows�ɔC����
		rc.right - rc.left,		//���w��
		rc.bottom - rc.top,		//�����w��
		nullptr,					// �e�E�C���h�E�̃n���h���i�Ȃ��j
		nullptr,					// ���j���[��q�E�C���h�E�̃n���h��
		hInstance,				// �A�v���P�[�V�����C���X�^���X�̃n���h��
		nullptr					// �E�C���h�E�̍쐬�f�[�^
	);
	if (!hWnd) {
		//���s����
		MessageBox(nullptr, L"�E�C���h�E�쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return 0;   //�G���[�I��
	}
	//�E�C���h�E�̕\��
	ShowWindow(
		hWnd,       //�擾�����E�C���h�E�̃n���h��
		nCmdShow    //WinMain�ɓn���ꂽ�p�����[�^
	);
	UpdateWindow(hWnd);
	return hWnd;
}

//--------------------------------------------------------------------------------------
//	int MainLoop(HINSTANCE hInstance, HWND hWnd, bool isFullScreen, int iClientWidth, int iClientHeight);
//	�p�r: ���C�����[�v
//--------------------------------------------------------------------------------------
int MainLoop(HINSTANCE hInstance, HWND hWnd, int nCmdShow, int iClientWidth, int iClientHeight) {
	//�I���R�[�h
	int retCode = 0;
	//�E�C���h�E���B���b�Z�[�W�{�b�N�X�\���`�F�b�N�Ɏg�p
	WINDOWINFO winInfo;
	ZeroMemory(&winInfo, sizeof(winInfo));
	//��O�����J�n
	try {
		//COM�̏�����
		//�T�E���h�ȂǂŎg�p����
		if (FAILED(::CoInitialize(nullptr))) {
			// ���������s
			throw exception("Com�������Ɏ��s���܂����B");
		}
		basedx12::Scene scene;
		basedx12::App::Init(hWnd, &scene, hInstance, nCmdShow, iClientWidth, iClientHeight);
		//���b�Z�[�W���[�v
		MSG msg = { 0 };
		while (WM_QUIT != msg.message) {
			//�L�[��Ԃ������Ȃ���΃E�C���h�E���b�Z�[�W�𓾂�
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			//�X�V�`�揈��
			basedx12::App::UpdateDraw();
		}
		//msg.wParam�ɂ͏I���R�[�h�������Ă���
		basedx12::App::Destroy();
		retCode = (int)msg.wParam;
	}
	catch (exception & e) {
		//STL�G���[
		//�}���`�o�C�g�o�[�W�����̃��b�Z�[�W�{�b�N�X���Ă�
		if (GetWindowInfo(hWnd, &winInfo)) {
			MessageBoxA(hWnd, e.what(), "�G���[", MB_OK);
		}
		else {
			MessageBoxA(nullptr, e.what(), "�G���[", MB_OK);
		}
		retCode = 1;
	}
	catch (...) {
		//�����s�����s����
		if (GetWindowInfo(hWnd, &winInfo)) {
			MessageBox(hWnd, L"�����s���̃G���[�ł�", L"�G���[", MB_OK);
		}
		else {
			MessageBox(nullptr, L"�����s���̃G���[�ł�", L"�G���[", MB_OK);
		}
		retCode = 1;
	}
	//��O�����I��
	//COM�̃����[�X
	::CoUninitialize();
	return retCode;
}


//--------------------------------------------------------------------------------------
//	int APIENTRY _tWinMain();
//	�p�r: �G���g���|�C���g
//--------------------------------------------------------------------------------------
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// �f�o�b�O���Adelete����̃`�F�b�N�p
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//���P�[���̐ݒ�
	setlocale(LC_ALL, "JPN");

	MyRegisterClass(hInstance);
	// �A�v���P�[�V�����̏����������s���܂�:
	HWND hWnd = InitInstance(hInstance, nCmdShow, g_ClientWidth, g_ClientHeight);

	if (!hWnd)
	{
		return FALSE;
	}

	return  MainLoop(hInstance, hWnd, nCmdShow, g_ClientWidth, g_ClientHeight);

}

//--------------------------------------------------------------------------------------
//
//  �֐�: MakeWindowModeRectFunc()
//
//  �ړI: �E�C���h�E���[�h�Ɉڍs�����`���쐬����
//
//--------------------------------------------------------------------------------------
void MakeWindowModeRectFunc(RECT& rc) {
	rc = { 0, 0, g_ClientWidth, g_ClientHeight };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	int cxWidth = GetSystemMetrics(SM_CXSCREEN);
	int cyHeight = GetSystemMetrics(SM_CYSCREEN);
	int rcWidth = rc.right - rc.left;
	int rcHeight = rc.bottom - rc.top;
	int offsetLeft = cxWidth / 2 - rcWidth / 2;
	int offsetTop = cyHeight / 2 - rcHeight / 2;
	OffsetRect(&rc, offsetLeft, offsetTop);
}

//--------------------------------------------------------------------------------------
//
//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:    ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND	- �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT	- ���C�� �E�B���h�E�̕`��
//  WM_DESTROY	- ���~���b�Z�[�W��\�����Ė߂�
//
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		// �L�[�������ꂽ
		switch (wParam) {
		case VK_ESCAPE:
			//�E�C���h�E��j������
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_SIZE:
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

