#include "stdafx.h"
#include "App.h"
#include "Scene.h"
namespace basedx12 {

    HWND App::m_hwnd = nullptr;
    SceneBase* App::m_pSceneBase = nullptr;
    //サイズはWinMainから渡される
    int App::m_width = 0;
    int App::m_height = 0;

    shared_ptr<Dx12Device> App::m_device = nullptr;


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


    int App::Run(SceneBase* pSceneBase, HINSTANCE hInstance, int nCmdShow, int width, int height)
    {
        m_width = width;
        m_height = height;

        SetAssetsPath();
        m_pSceneBase = pSceneBase;

        // Initialize the window class.
        WNDCLASSEX windowClass = { 0 };
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        windowClass.lpfnWndProc = WindowProc;
        windowClass.hInstance = hInstance;
        windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        windowClass.lpszClassName = m_classsName.c_str();
        RegisterClassEx(&windowClass);
        RECT windowRect = { 0,0,m_width ,m_height };

        AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

        // Create the window and store a handle to it.
        m_hwnd = CreateWindow(
            windowClass.lpszClassName,
            m_title.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            nullptr,
            nullptr,
            hInstance,
            nullptr);

        m_pSceneBase->OnInit();

        ShowWindow(m_hwnd, nCmdShow);

        MSG msg = {};
        while (msg.message != WM_QUIT)
        {
            // Process any messages in the queue.
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        m_pSceneBase->OnDestroy();
        if (m_device) {
            m_device->OnDestroy();
            m_device.reset();
        }
        m_pSceneBase = nullptr;

        return static_cast<char>(msg.wParam);
    }

    LRESULT CALLBACK App::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_CREATE:
        {
            // Save the DXSample* passed in to CreateWindow.
            LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
        }
        return 0;
        case WM_KEYDOWN:
            {
                UINT8 key = static_cast<UINT8>(wParam);
                if (key == VK_ESCAPE) {
                    DestroyWindow(hWnd);
                }
                else {
                    if (m_pSceneBase && m_device)
                    {
                        m_pSceneBase->OnKeyDown(key);
                        m_device->OnKeyDown(key);
                    }
                }
            }
            return 0;
        case WM_KEYUP:
            if (m_pSceneBase && m_device)
            {
                m_pSceneBase->OnKeyUp(static_cast<UINT8>(wParam));
                m_device->OnKeyUp(static_cast<UINT8>(wParam));
            }
            return 0;
        case WM_PAINT:
            if (m_pSceneBase && m_device)
            {                
                m_pSceneBase->OnUpdate();
                m_device->OnUpdate();
                m_pSceneBase->OnRender();
                m_device->OnRender();
            }
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
//end basedx12

