#pragma once

namespace basedx12 {

    class SceneBase;
    class Dx12Device;

    //--------------------------------------------------------------------------------------
    ///	�A�v���P�[�V�����N���X
    //--------------------------------------------------------------------------------------
    class App
    {
    public:
        static int Run(SceneBase* pSceneBase, HINSTANCE hInstance, int nCmdShow, int width, int height);
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
            double d = m_Timer.GetElapsedSeconds();
            //0���Z����̂���1000����1�����͕Ԃ�Ȃ��悤�ɂ���
            if (d < 0.001) {
                d = 0.001;
            }
            return (float)d;
        }

    protected:
        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
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

        static wstring m_wstrModulePath;		///< ���W���[�����t���p�X
        static wstring m_wstrDir;				///< ���W���[��������f�B���N�g��
        static wstring m_wstrDataPath;			///< ��΃p�X�̃��f�B�A�f�B���N�g��
        static wstring m_wstrShadersPath;		///< ��΃p�X�̃V�F�[�_�f�B���N�g��
        static wstring m_wstrRelativeDataPath;	///< ���΃p�X�̃��f�B�A�f�B���N�g��
        static wstring m_wstrRelativeShadersPath;	///< ���΃p�X�̃V�F�[�_�f�B���N�g��
        static wstring	m_wstrRelativeAssetsPath;	///< ���΃p�X�̃A�Z�b�g�f�B���N�g��

        static StepTimer m_Timer;										///< �^�C�}�[


    };

}
//end basedx12

