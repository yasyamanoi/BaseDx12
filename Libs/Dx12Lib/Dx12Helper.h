//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once
#include <stdexcept>



// Note that while ComPtr is used to manage the lifetime of resources on the CPU,
// it has no understanding of the lifetime of resources on the GPU. Apps must account
// for the GPU lifetime of resources to avoid destroying objects that may still be
// referenced by the GPU.
using Microsoft::WRL::ComPtr;

namespace basedx12 {


    inline std::string HrToString(HRESULT hr)
    {
        char s_str[64] = {};
        sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
        return std::string(s_str);
    }


    inline std::string WStoMB(const wstring& src) {
        size_t i;
        char* pMBstr = new char[src.length() * MB_CUR_MAX + 1];
        wcstombs_s(
            &i,
            pMBstr,
            src.length() * MB_CUR_MAX + 1,
            src.c_str(),
            _TRUNCATE	//���ׂĕϊ��ł��Ȃ�������؂�̂�
        );
        string ret = pMBstr;
        delete[] pMBstr;
        return ret;
    }

    inline std::wstring MBtoWS(const string& src) {
        size_t i;
        wchar_t* WCstr = new wchar_t[src.length() + 1];
        mbstowcs_s(
            &i,
            WCstr,
            src.length() + 1,
            src.c_str(),
            _TRUNCATE //���ׂĕϊ��ł��Ȃ�������؂�̂�
        );
        wstring ret = WCstr;
        delete[] WCstr;
        return ret;
    }


    class HrException : public std::runtime_error
    {
    public:
        HrException(HRESULT hr) : std::runtime_error(HrToString(hr)), m_hr(hr) {}
        HrException(HRESULT hr, const std::string& mess) :
            std::runtime_error(HrToString(hr) + mess),
            m_hr(hr)
        {}
        HRESULT Error() const { return m_hr; }
    private:
        const HRESULT m_hr;
    };

    class BaseException : public std::runtime_error
    {
    public:
        BaseException(const std::wstring& wstr1,
            const std::wstring& wstr2,
            const std::wstring& wstr3 = L""
        ) :
            std::runtime_error(WStoMB(wstr1) + WStoMB(wstr2) + WStoMB(wstr3))
        {}
        BaseException(const std::string& str1,
            const std::string& str2,
            const std::string& str3 = ""
        ) :
            std::runtime_error(str1 + str2 + str3)
        {}
    };

#define SAFE_RELEASE(p) if (p) (p)->Release()

    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw HrException(hr);
        }
    }

    inline void ThrowIfFailed(HRESULT hr,
        const std::wstring& wstr1,
        const std::wstring& wstr2 = std::wstring(),
        const std::wstring& wstr3 = std::wstring()
    )
    {
        if (FAILED(hr))
        {
            throw HrException(hr, WStoMB(wstr1) + WStoMB(wstr2) + WStoMB(wstr3));
        }
    }

    inline void ThrowIfFailed(HRESULT hr,
        const std::string& str1,
        const std::string& str2 = std::string(),
        const std::string& str3 = std::string()
    )
    {
        if (FAILED(hr))
        {
            throw HrException(hr, str1 + str2 + str3);
        }
    }

    inline void GetAssetsPath(_Out_writes_(pathSize) WCHAR* path, UINT pathSize)
    {
        if (path == nullptr)
        {
            throw std::exception();
        }

        DWORD size = GetModuleFileName(nullptr, path, pathSize);
        if (size == 0 || size == pathSize)
        {
            // Method failed or path was truncated.
            throw std::exception();
        }

        WCHAR* lastSlash = wcsrchr(path, L'\\');
        if (lastSlash)
        {
            *(lastSlash + 1) = L'\0';
        }
    }

    inline HRESULT ReadDataFromFile(LPCWSTR filename, byte** data, UINT* size)
    {
        using namespace Microsoft::WRL;

        CREATEFILE2_EXTENDED_PARAMETERS extendedParams = {};
        extendedParams.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
        extendedParams.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
        extendedParams.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
        extendedParams.dwSecurityQosFlags = SECURITY_ANONYMOUS;
        extendedParams.lpSecurityAttributes = nullptr;
        extendedParams.hTemplateFile = nullptr;

        Wrappers::FileHandle file(CreateFile2(filename, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, &extendedParams));
        if (file.Get() == INVALID_HANDLE_VALUE)
        {
            throw std::exception();
        }

        FILE_STANDARD_INFO fileInfo = {};
        if (!GetFileInformationByHandleEx(file.Get(), FileStandardInfo, &fileInfo, sizeof(fileInfo)))
        {
            throw std::exception();
        }

        if (fileInfo.EndOfFile.HighPart != 0)
        {
            throw std::exception();
        }

        *data = reinterpret_cast<byte*>(malloc(fileInfo.EndOfFile.LowPart));
        *size = fileInfo.EndOfFile.LowPart;

        if (!ReadFile(file.Get(), *data, fileInfo.EndOfFile.LowPart, nullptr, nullptr))
        {
            throw std::exception();
        }

        return S_OK;
    }

    // Assign a name to the object to aid with debugging.
#if defined(_DEBUG) || defined(DBG)
    inline void SetName(ID3D12Object* pObject, LPCWSTR name)
    {
        pObject->SetName(name);
    }
    inline void SetNameIndexed(ID3D12Object* pObject, LPCWSTR name, UINT index)
    {
        WCHAR fullName[50];
        if (swprintf_s(fullName, L"%s[%u]", name, index) > 0)
        {
            pObject->SetName(fullName);
        }
    }
#else
    inline void SetName(ID3D12Object*, LPCWSTR)
    {
    }
    inline void SetNameIndexed(ID3D12Object*, LPCWSTR, UINT)
    {
    }
#endif

    // Naming helper for ComPtr<T>.
    // Assigns the name of the variable as the name of the object.
    // The indexed variant will include the index in the name of the object.
#define NAME_D3D12_OBJECT(x) SetName((x).Get(), L#x)
#define NAME_D3D12_OBJECT_INDEXED(x, n) SetNameIndexed((x)[n].Get(), L#x, n)

    inline UINT CalculateConstantBufferByteSize(UINT byteSize)
    {
        // Constant buffer size is required to be aligned.
        return (byteSize + (D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1)) & ~(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1);
    }

#ifdef D3D_COMPILE_STANDARD_FILE_INCLUDE
    inline Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(
        const std::wstring& filename,
        const D3D_SHADER_MACRO* defines,
        const std::string& entrypoint,
        const std::string& target)
    {
        UINT compileFlags = 0;
#if defined(_DEBUG) || defined(DBG)
        compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

        HRESULT hr;

        Microsoft::WRL::ComPtr<ID3DBlob> byteCode = nullptr;
        Microsoft::WRL::ComPtr<ID3DBlob> errors;
        hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

        if (errors != nullptr)
        {
            OutputDebugStringA((char*)errors->GetBufferPointer());
        }
        ThrowIfFailed(hr);

        return byteCode;
    }
#endif

    // Resets all elements in a ComPtr array.
    template<class T>
    void ResetComPtrArray(T* comPtrArray)
    {
        for (auto& i : *comPtrArray)
        {
            i.Reset();
        }
    }


    // Resets all elements in a unique_ptr array.
    template<class T>
    void ResetUniquePtrArray(T* uniquePtrArray)
    {
        for (auto& i : *uniquePtrArray)
        {
            i.reset();
        }
    }

    //--------------------------------------------------------------------------------------
    ///	Object���\�z����
    //--------------------------------------------------------------------------------------
    class ObjectFactory {
    public:
        //--------------------------------------------------------------------------------------
        /*!
        @brief �I�u�W�F�N�g�쐬�istatic�֐��j
        @tparam T	�쐬����^
        @tparam Ts...	�ϒ��p�����[�^�^
        @param[in]	params	�ϒ��p�����[�^
        @return�@�쐬�����I�u�W�F�N�g��shared_ptr
        */
        //--------------------------------------------------------------------------------------
        template<typename T, typename... Ts>
        static shared_ptr<T> Create(Ts&&... params) {
            shared_ptr<T> Ptr = shared_ptr<T>(new T(params...));
            //�������֐��Ăяo��
            Ptr->OnInit();
            return Ptr;
        }
        //--------------------------------------------------------------------------------------
        /*!
        @brief �I�u�W�F�N�g�쐬�istatic�֐��j�B�p�����[�^��OnInit�ɓn�����
        @tparam T	�쐬����^
        @tparam Ts...	�ϒ��p�����[�^�^
        @param[in]	params	�ϒ��p�����[�^
        @return�@�쐬�����I�u�W�F�N�g��shared_ptr
        */
        //--------------------------------------------------------------------------------------
        template<typename T, typename... Ts>
        static shared_ptr<T> CreateInitParam(Ts&&... params) {
            shared_ptr<T> Ptr = shared_ptr<T>(new T());
            //�������֐��Ăяo��
            Ptr->OnInit(params...);
            return Ptr;
        }

    };

    struct 	Util {


        //--------------------------------------------------------------------------------------
        /*!
        @brief	���S��COM�C���^�[�t�F�C�X�̃|�C���^���擾����w���p�[�֐�.
        @tparam	T	�擾����^
        @tparam	TCreateFunc	�쐬�֐��̌^
        @param[in,out]	comPtr	COM�|�C���^
        @param[in]	mutex	�~���[�e�b�N�X
        @param[in]	createFunc	�쐬�֐�
        @return	COM�������̏ꍇ�͐V�����쐬���āA����ȊO��COM���瓾��COM�C���^�[�t�F�C�X�̃|�C���^
        */
        //--------------------------------------------------------------------------------------
        template<typename T, typename TCreateFunc>
        static T* DemandCreate(ComPtr<T>& comPtr, std::mutex& mutex, TCreateFunc createFunc)
        {
            T* result = comPtr.Get();
            //���b�N��Ԃ��`�F�b�N
            MemoryBarrier();
            if (!result)
            {
                std::lock_guard<std::mutex> lock(mutex);
                result = comPtr.Get();
                if (!result)
                {
                    createFunc(&result);
                    MemoryBarrier();
                    comPtr.Attach(result);
                }
            }
            return result;
        }

        //--------------------------------------------------------------------------------------
        /*!
        @brief	���S��COM�C���^�[�t�F�C�X�̃|�C���^���擾����w���p�[�֐�.<br />
        �����̏ꍇ�͗�O���o��
        @tparam	T	�擾����^
        @param[in]	comPtr	COM�|�C���^
        @return�@COM�������̏ꍇ�͗�O���o���āA����ȊO��COM���瓾��COM�C���^�[�t�F�C�X�̃|�C���^
        */
        //--------------------------------------------------------------------------------------
        template<typename T>
        static T* GetComPtr(ComPtr<T>& comPtr) {
            T* result = comPtr.Get();
            //���b�N��Ԃ��`�F�b�N
            MemoryBarrier();
            if (!result)
            {
                //���s
                throw BaseException(
                    L"����COM�C���^�[�t�F�C�X���擾�ł��܂���",
                    Util::GetWSTypeName<T>(),
                    L"Util::GetComPtr()"
                );
            }
            return result;
        }

    };

    //--------------------------------------------------------------------------------------
    ///	�V�F�[�_�֘A���\�[�X�̃C���^�[�t�F�C�X
    //--------------------------------------------------------------------------------------
    class Dx12ShaderResource {
    protected:
        Dx12ShaderResource();
        virtual ~Dx12ShaderResource();
        //�V�F�[�_�A�N�Z�T
        ID3DBlob* GetShaderBlob(const wstring& Filename, ComPtr<ID3DBlob>& ShaderComPtr);
        ComPtr<ID3DBlob>& GetShaderBlobComPtr(const wstring& Filename, ComPtr<ID3DBlob>& ShaderComPtr);
    private:
        //�R�s�[�֎~
        Dx12ShaderResource(const Dx12ShaderResource&) = delete;
        Dx12ShaderResource& operator=(const Dx12ShaderResource&) = delete;
        //���[�u�֎~
        Dx12ShaderResource(const Dx12ShaderResource&&) = delete;
        Dx12ShaderResource& operator=(const Dx12ShaderResource&&) = delete;
    };

    //--------------------------------------------------------------------------------------
    ///	�V���O���g���V�F�[�_�̐e�N���X(�V�F�[�_�̎�ނɂ�����炸�����N���X�g�p)
    //--------------------------------------------------------------------------------------
    template<typename ShaderType>
    class Dx12Shader : public Dx12ShaderResource {
        wstring m_Filename;
    protected:
        //�f���[�^�[
        struct Deleter
        {
            void operator()(ShaderType* p) { delete p; }
        };
        ComPtr<ID3DBlob> m_ShaderPtr;
        //�\�z�Ɣj��
        Dx12Shader<ShaderType>(const wstring& Filename) : Dx12ShaderResource(), m_Filename(Filename) {}
        virtual ~Dx12Shader() {}
        static unique_ptr<ShaderType, Deleter> m_Ptr;
    public:
        //�V�F�[�_���J�A�N�Z�b�T
        ID3DBlob* GetShader() {
            return GetShaderBlob(m_Filename, m_ShaderPtr);
        }
        ComPtr<ID3DBlob>& GetShaderComPtr() {
            return GetShaderBlobComPtr(m_Filename, m_ShaderPtr);
        }
        //�C���X�^���X�擾
        static unique_ptr<ShaderType, Deleter>& GetPtr() {
            if (!m_Ptr) {
                m_Ptr.reset(new ShaderType());
            }
            return m_Ptr;
        }
    };


    //������ʃV�F�[�_�[
    //�V�F�[�_�錾�p�}�N��
#define DECLARE_DX12SHADER(ShaderName) class ShaderName : public Dx12Shader<ShaderName>{ \
	public: \
		ShaderName(); \
	};
    //�V�F�[�_���̗p�}�N��
#define IMPLEMENT_DX12SHADER(ShaderName,CsoFilename) unique_ptr<ShaderName, ShaderName::Deleter> ShaderName::m_Ptr; \
	ShaderName::ShaderName() : \
	Dx12Shader(CsoFilename){}

}
//end basedx12

