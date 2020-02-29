
#include "stdafx.h"
#include "Scene.h"
#include "resource.h"

// このコード モジュールに含まれる関数の宣言を転送します:
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

//定数
const wchar_t* pClassName = L"BaseDx12Class";
const wchar_t* pWndTitle = L"BaseDx12Title";
//ウィンドウモードの時の幅と高さ
int g_ClientWidth = 1280;
int g_ClientHeight = 720;


//--------------------------------------------------------------------------------------
//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
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
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//--------------------------------------------------------------------------------------
HWND InitInstance(HINSTANCE hInstance, int nCmdShow, bool isFullScreen, int iClientWidth, int iClientHeight)
{

	HWND hWnd = 0;
	// ウィンドウの作成
	if (isFullScreen) {
		// フルスクリーン
		//ボーダーレスウインドウを使用
		iClientWidth = GetSystemMetrics(SM_CXSCREEN);
		iClientHeight = GetSystemMetrics(SM_CYSCREEN);
		hWnd = CreateWindow(
			pClassName,			// 登録されているクラス名
			pWndTitle,			// ウインドウ名
			WS_POPUP,			// ウインドウスタイル（ポップアップウインドウを作成）
			0,					// ウインドウの横方向の位置
			0,					// ウインドウの縦方向の位置
			iClientWidth,		// フルスクリーンウインドウの幅
			iClientHeight,		// フルスクリーンウインドウの高さ
			nullptr,				// 親ウインドウのハンドル（なし）
			nullptr,				// メニューや子ウインドウのハンドル
			hInstance,			// アプリケーションインスタンスのハンドル
			nullptr				// ウインドウの作成データ
		);
		if (!hWnd) {
			//失敗した
			MessageBox(nullptr, L"ウインドウ作成に失敗しました", L"エラー", MB_OK);
			return 0;   //エラー終了
		}
	}
	else {
		//ウインドウのサイズ調整
		RECT rc = { 0, 0, iClientWidth, iClientHeight };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		//ウインドウの作成
		hWnd = CreateWindow(
			pClassName,				// 登録されているクラス名
			pWndTitle,				// ウインドウ名
			WS_OVERLAPPEDWINDOW,	// ウインドウスタイル（オーバーラップウインドウを作成）
			CW_USEDEFAULT,			//位置はWindowsに任せる
			CW_USEDEFAULT,			//位置はWindowsに任せる
			rc.right - rc.left,		//幅指定
			rc.bottom - rc.top,		//高さ指定
			nullptr,					// 親ウインドウのハンドル（なし）
			nullptr,					// メニューや子ウインドウのハンドル
			hInstance,				// アプリケーションインスタンスのハンドル
			nullptr					// ウインドウの作成データ
		);
		if (!hWnd) {
			//失敗した
			MessageBox(nullptr, L"ウインドウ作成に失敗しました", L"エラー", MB_OK);
			return 0;   //エラー終了
		}
	}
	//ウインドウの表示
	ShowWindow(
		hWnd,       //取得したウインドウのハンドル
		nCmdShow    //WinMainに渡されたパラメータ
	);
	UpdateWindow(hWnd);
	return hWnd;
}

//--------------------------------------------------------------------------------------
//	int MainLoop(HINSTANCE hInstance, HWND hWnd, bool isFullScreen, int iClientWidth, int iClientHeight);
//	用途: メインループ
//--------------------------------------------------------------------------------------
int MainLoop(HINSTANCE hInstance, HWND hWnd, int nCmdShow, bool isFullScreen, int iClientWidth, int iClientHeight) {
	//終了コード
	int RetCode = 0;
	//ウインドウ情報。メッセージボックス表示チェックに使用
	WINDOWINFO WinInfo;
	ZeroMemory(&WinInfo, sizeof(WinInfo));
	//例外処理開始
	try {
		//COMの初期化
		//サウンドなどで使用する
		if (FAILED(::CoInitialize(nullptr))) {
			// 初期化失敗
			throw exception("Com初期化に失敗しました。");
		}
		basedx12::Scene scene;
		basedx12::App::Init(hWnd, &scene, hInstance, nCmdShow, iClientWidth, iClientHeight);
		//メッセージループ
		MSG msg = { 0 };
		while (WM_QUIT != msg.message) {
			//キー状態が何もなければウインドウメッセージを得る
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			//更新描画処理
			basedx12::App::UpdateDraw();
		}
		//msg.wParamには終了コードが入っている
		basedx12::App::Destroy();
		RetCode = (int)msg.wParam;
	}
	catch (exception & e) {
		//STLエラー
		//マルチバイトバージョンのメッセージボックスを呼ぶ
		if (GetWindowInfo(hWnd, &WinInfo)) {
			MessageBoxA(hWnd, e.what(), "エラー", MB_OK);
		}
		else {
			MessageBoxA(nullptr, e.what(), "エラー", MB_OK);
		}
		RetCode = 1;
	}
	catch (...) {
		//原因不明失敗した
		if (GetWindowInfo(hWnd, &WinInfo)) {
			MessageBox(hWnd, L"原因不明のエラーです", L"エラー", MB_OK);
		}
		else {
			MessageBox(nullptr, L"原因不明のエラーです", L"エラー", MB_OK);
		}
		RetCode = 1;
	}
	//例外処理終了
	//COMのリリース
	::CoUninitialize();
	return RetCode;
}


//--------------------------------------------------------------------------------------
//	int APIENTRY _tWinMain();
//	用途: エントリポイント
//--------------------------------------------------------------------------------------
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// デバッグ時、deleteもれのチェック用
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//ロケールの設定
	setlocale(LC_ALL, "JPN");

	// フルスクリーンにするかどうかの判定
	// コマンドラインに/fが設定されていたらフルスクリーンにする
	bool isFullScreen = false;
	wstring wstrcmd = lpCmdLine;
	if (wstrcmd == L"/f" || wstrcmd == L"/F") {
		isFullScreen = true;     // フラグをtrueに設定
	}

	MyRegisterClass(hInstance);
	// アプリケーションの初期化を実行します:
	HWND hWnd = InitInstance(hInstance, nCmdShow, isFullScreen, g_ClientWidth, g_ClientHeight);

	if (!hWnd)
	{
		return FALSE;
	}

	return  MainLoop(hInstance, hWnd, nCmdShow, isFullScreen, g_ClientWidth, g_ClientHeight);

}

//--------------------------------------------------------------------------------------
//
//  関数: MakeWindowModeRectFunc()
//
//  目的: ウインドウモードに移行する矩形を作成する
//
//--------------------------------------------------------------------------------------
void MakeWindowModeRectFunc(RECT& rc) {
	rc = { 0, 0, g_ClientWidth, g_ClientHeight };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	int CXWidth = GetSystemMetrics(SM_CXSCREEN);
	int CYHeight = GetSystemMetrics(SM_CYSCREEN);
	int RCWidth = rc.right - rc.left;
	int RCHeight = rc.bottom - rc.top;
	int OffsetLeft = CXWidth / 2 - RCWidth / 2;
	int OffsetTop = CYHeight / 2 - RCHeight / 2;
	OffsetRect(&rc, OffsetLeft, OffsetTop);
}

//--------------------------------------------------------------------------------------
//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 中止メッセージを表示して戻る
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
		// キーが押された
		switch (wParam) {
		case VK_ESCAPE:
			//ウインドウを破棄する
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

