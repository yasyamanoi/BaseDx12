#include "stdafx.h"
#include "App.h"
#include "Scene.h"
namespace basedx12 {

    HWND App::m_hwnd = nullptr;
    SceneBase* App::m_pSceneBase = nullptr;
    //�T�C�Y��WinMain����n�����
    int App::m_width = 0;
    int App::m_height = 0;

    shared_ptr<Dx12Device> App::m_device = nullptr;


    wstring App::m_classsName = L"BaseDx12Class";
    wstring App::m_title = L"BaseDx12Title";

    wstring App::m_wstrModulePath;		///< ���W���[�����t���p�X
    wstring App::m_wstrDir;				///< ���W���[��������f�B���N�g��
    wstring App::m_wstrDataPath;			///< ��΃p�X�̃��f�B�A�f�B���N�g��
    wstring App::m_wstrShadersPath;		///< ��΃p�X�̃V�F�[�_�f�B���N�g��
    wstring App::m_wstrRelativeDataPath;	///< ���΃p�X�̃��f�B�A�f�B���N�g��
    wstring App::m_wstrRelativeShadersPath;	///< ���΃p�X�̃V�F�[�_�f�B���N�g��
    wstring App::m_wstrRelativeAssetsPath;	///< ���΃p�X�̃A�Z�b�g�f�B���N�g��


    void App::SetAssetsPath() {
        //��f�B���N�g���̐ݒ�
        //���΃p�X�ɂ���ƁA�t�@�C���_�C�A���O�ŃJ�����g�p�X�������̂�
        //��΃p�X�w��
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

        //���W���[�����i�v���O�����t�@�C�����j�𓾂�
        if (!::GetModuleFileName(nullptr, Modulebuff, sizeof(Modulebuff))) {
            throw runtime_error("���W���[�����擾�ł��܂���B");
        }
        m_wstrModulePath = Modulebuff;
        //���W���[��������A�e�u���b�N�ɕ�����
        _wsplitpath_s(Modulebuff,
            Drivebuff, _MAX_DRIVE,
            Dirbuff, _MAX_DIR,
            FileNamebuff, _MAX_FNAME,
            Extbuff, _MAX_EXT);

        //�h���C�u���̎擾
        m_wstrDir = Drivebuff;
        //�f�B���N�g�����̎擾
        m_wstrDir += Dirbuff;
        //media�f�B���N�g����T��
        m_wstrDataPath = m_wstrDir;
        m_wstrDataPath += L"media";
        //�܂��A���s�t�@�C���Ɠ����f�B���N�g����T��
        DWORD RetCode;
        RetCode = GetFileAttributes(m_wstrDataPath.c_str());
        if (RetCode == 0xFFFFFFFF) {
            //���s����
            m_wstrDataPath = m_wstrDir;
            m_wstrDataPath += L"..\\media";
            RetCode = GetFileAttributes(m_wstrDataPath.c_str());
            if (RetCode == 0xFFFFFFFF) {
                //�Ăю��s����
                throw runtime_error("media�f�B���N�g�����m�F�ł��܂���B");
            }
            else {
                m_wstrDataPath += L"\\";
                //���΃p�X�̐ݒ�
                m_wstrRelativeDataPath = L"..\\media\\";
            }
        }
        else {
            m_wstrDataPath += L"\\";
            //���΃p�X�̐ݒ�
            m_wstrRelativeDataPath = L"media\\";
        }
        m_wstrShadersPath = m_wstrDataPath + L"Shaders\\";
        m_wstrRelativeShadersPath = m_wstrRelativeDataPath + L"Shaders\\";


        //Assets�f�B���N�g����T��
        m_wstrRelativeAssetsPath = m_wstrDir;
        m_wstrRelativeAssetsPath += L"..\\..\\Assets";
        //���΃f�B���N�g����T��
        RetCode = GetFileAttributes(m_wstrRelativeAssetsPath.c_str());
        if (RetCode == 0xFFFFFFFF) {
            //���s����
            //�A�Z�b�g�f�B���N�g�������f�B�A�f�B���N�g���ɂ���
            m_wstrRelativeAssetsPath = m_wstrRelativeDataPath;
        }
        else {
            //��������
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

