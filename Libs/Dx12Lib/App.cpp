#include "stdafx.h"

namespace basedx12 {

	HWND App::m_hwnd = nullptr;
	SceneBase* App::m_pSceneBase = nullptr;
	//�T�C�Y��WinMain����n�����
	int App::m_width = 0;
	int App::m_height = 0;

	shared_ptr<BaseDevice> App::m_baseDevice = nullptr;

	StepTimer App::m_timer;
	Controler App::m_controler;


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
	void App::Init(HWND hwd, SceneBase* pSceneBase, HINSTANCE hInstance, int nCmdShow, int width, int height)
	{
		m_width = width;
		m_height = height;
		m_pSceneBase = pSceneBase;
		m_hwnd = hwd;
		SetAssetsPath();
		//�f�o�C�X�̏������̓V�[������Ă΂��
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

