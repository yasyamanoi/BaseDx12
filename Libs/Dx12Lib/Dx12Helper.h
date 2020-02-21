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
            _TRUNCATE	//すべて変換できなかったら切り捨て
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
            _TRUNCATE //すべて変換できなかったら切り捨て
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


    struct 	Util {


		//--------------------------------------------------------------------------------------
		/*!
		@brief	大きい方を求める.
		@tparam	T	比較する型
		@param[in]	v1	T型の値1
		@param[in]	v2	T型の値2
		@return	T型の大きい方
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static T Maximum(const T& v1, const T& v2) {
			return v1 > v2 ? v1 : v2;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	小さい方を求める.
		@tparam	T	比較する型
		@param[in]	v1	T型の値1
		@param[in]	v2	T型の値2
		@return	T型の小さい方
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static T Minimum(const T& v1, const T& v2) {
			return v1 < v2 ? v1 : v2;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	安全にCOMインターフェイスのポインタを取得するヘルパー関数.
		@tparam	T	取得する型
		@tparam	TCreateFunc	作成関数の型
		@param[in,out]	comPtr	COMポインタ
		@param[in]	mutex	ミューテックス
		@param[in]	createFunc	作成関数
		@return	COMが無効の場合は新しく作成して、それ以外はCOMから得たCOMインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename TCreateFunc>
		static T* DemandCreate(ComPtr<T>& comPtr, std::mutex& mutex, TCreateFunc createFunc)
		{
			T* result = comPtr.Get();
			//ロック状態をチェック
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
		@brief	安全にCOMインターフェイスのポインタを取得するヘルパー関数.<br />
		無効の場合は例外を出す
		@tparam	T	取得する型
		@param[in]	comPtr	COMポインタ
		@return　COMが無効の場合は例外を出して、それ以外はCOMから得たCOMインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static T* GetComPtr(ComPtr<T>& comPtr) {
			T* result = comPtr.Get();
			//ロック状態をチェック
			MemoryBarrier();
			if (!result)
			{
				//失敗
				throw BaseException(
					L"このCOMインターフェイスを取得できません",
					Util::GetWSTypeName<T>(),
					L"Util::GetComPtr()"
				);
			}
			return result;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	両端から空白を取り除く<br/>
		元になる文字列から取り除く
		@param[in,out]	wstr	もとになる文字列（リターンにも使用される）
		@param[in]	trimCharacterList = L" \t\v\r\n"	取り除く文字
		@return　なし
		*/
		//--------------------------------------------------------------------------------------
		static void WStrTrim(wstring& wstr, const wchar_t* TrimCharList = L" \t\v\r\n") {
			wstring result(L"");
			if (wstr.size() <= 0) {
				wstr = result;
				return;
			}
			wstring::size_type left = wstr.find_first_not_of(TrimCharList);
			if (left != wstring::npos) {
				wstring::size_type right = wstr.find_last_not_of(TrimCharList);
				result = wstr.substr(left, right - left + 1);
			}
			wstr = result;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	ワイド文字列からマルチバイト文字列変換<br />
		ロケール依存のため、WinMain()等で、setlocale( LC_ALL, "JPN" );が必要
		@param[in] src	変換する文字列（ワイドキャラ）
		@param[out]	dest	変換後の文字列（マルチバイト）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void WStoMB(const wstring& src, string& dest) {
			size_t i;
			char* pMBstr = new char[src.length() * MB_CUR_MAX + 1];
			wcstombs_s(
				&i,
				pMBstr,
				src.length() * MB_CUR_MAX + 1,
				src.c_str(),
				_TRUNCATE	//すべて変換できなかったら切り捨て
			);
			dest = pMBstr;
			delete[] pMBstr;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	ワイド文字列からマルチバイトUTF8文字列変換<br />
		ロケール依存のため、WinMain()等で、setlocale( LC_ALL, "JPN" );が必要
		@param[in]	src	変換する文字列（ワイドキャラ）
		@param[out]	dest	変換後の文字列（マルチバイト）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void ConvertWstringtoUtf8(const wstring& src, string& dest) {
			INT bufsize = ::WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, NULL, 0, NULL, NULL);
			char* Temp = new char[bufsize + 1];
			::WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, Temp, bufsize, NULL, NULL);
			dest = Temp;
			delete[] Temp;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	マルチバイトUTF8文字からワイド文字列変換<br />
		ロケール依存のため、WinMain()等で、setlocale( LC_ALL, "JPN" );が必要
		@param[in]	src	変換する文字列（マルチバイトUTF8）
		@param[out]	dest	変換後文字列（ワイドキャラ）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void ConvertUtf8toWstring(const string& src, wstring& dest) {
			INT bufsize = ::MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, (wchar_t*)NULL, 0);
			wchar_t* Temp = (wchar_t*)new wchar_t[bufsize];
			::MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, Temp, bufsize);
			dest = Temp;
			delete[] Temp;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マルチバイト文字列からワイド文字列変換<br />
		ロケール依存のため、WinMain()等で、setlocale( LC_ALL, "JPN" );が必要
		@param[in]	src	変換する文字列（マルチバイト）
		@param[out]	dest	変換後の文字列（ワイドキャラ）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void MBtoWS(const string& src, wstring& dest) {
			size_t i;
			wchar_t* WCstr = new wchar_t[src.length() + 1];
			mbstowcs_s(
				&i,
				WCstr,
				src.length() + 1,
				src.c_str(),
				_TRUNCATE //すべて変換できなかったら切り捨て
			);
			dest = WCstr;
			delete[] WCstr;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	文字列をデリミタ文字により区切って配列に保存
		@param[out]	wstrvec	保存される配列
		@param[in]	line	元となる文字列
		@param[in]	delimiter	デリミタ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void WStrToTokenVector(vector<wstring>& wstrvec, const wstring& line, wchar_t delimiter) {
			wstring::size_type i = 0;
			wstring wks(L"");
			for (i = 0; i < line.size(); i++) {
				if (line[i] == delimiter) {
					if (wks != L"") {
						wstrvec.push_back(wks);
					}
					wks = L"";
				}
				else {
					wks += line[i];
				}
			}
			wstrvec.push_back(wks);
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	T型の内部機器別名を得る（wstring）
		@tparam T	型名を得る型
		@return	T型の内部機器別名
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static wstring GetWSTypeName() {
			wstring clsname;
			MBtoWS(typeid(T).name(), clsname);
			return clsname;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	T型の内部機器別名を得る（string）
		@tparam T	型名を得る型
		@return	T型の内部機器別名
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static string GetMBTypeName() {
			string clsname = typeid(T).name();
			return clsname;
		}

		//--------------------------------------------------------------------------------------
		/// float型を文字列に変換する場合の形式
		//--------------------------------------------------------------------------------------
		enum FloatModify {
			Default = 0,	///< デフォルト（浮動小数点）
			Fixed,	///< 数字を出力
			Scientific,	///< e+09などの形式
		};

		//--------------------------------------------------------------------------------------
		/*!
		@brief	浮動小数点を文字列に変換する（wstring版）
		@param[in]	Val	浮動小数点値
		@param[in]	Precision = 0	精度0でデフォルト
		@param[in]	Modify = FloatModify::Default	表示形式
		@return	変換した文字列
		*/
		//--------------------------------------------------------------------------------------
		static wstring FloatToWStr(float Val, streamsize Precision = 0,
			FloatModify Modify = FloatModify::Default) {
			//返す文字列
			wstring str;
			//書式を整えるストリーム
			wostringstream stream;
			//浮動小数点の精度を決める
			if (Precision > 0) {
				stream.precision(Precision);
			}
			switch (Modify) {
			case FloatModify::Fixed:
				stream << std::fixed << Val;
				break;
			case FloatModify::Scientific:
				stream << std::scientific << Val;
				break;
			case FloatModify::Default:
			default:
				stream << Val;
				break;
			}
			str = stream.str();
			return str;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	浮動小数点を文字列に変換する（string版）
		@param[in]	Val	浮動小数点値
		@param[in]	Precision = 0	精度0でデフォルト
		@param[in]	Modify = FloatModify::Default	表示形式
		@return	変換した文字列
		*/
		//--------------------------------------------------------------------------------------
		static string FloatToStr(float Val, streamsize Precision = 0,
			FloatModify Modify = FloatModify::Default) {
			//返す文字列
			string str;
			//書式を整えるストリーム
			ostringstream stream;
			//浮動小数点の精度を決める
			if (Precision > 0) {
				stream.precision(Precision);
			}
			switch (Modify) {
			case FloatModify::Fixed:
				stream << std::fixed << Val;
				break;
			case FloatModify::Scientific:
				stream << std::scientific << Val;
				break;
			case FloatModify::Default:
			default:
				stream << Val;
				break;
			}
			str = stream.str();
			return str;
		}


		//--------------------------------------------------------------------------------------
		/// 整数型を文字列に変換する場合の形式
		//--------------------------------------------------------------------------------------
		enum NumModify {
			Dec = 0,	///< 10進数
			Hex,	///< 16進数
			Oct,	///< 8進数
		};
		//--------------------------------------------------------------------------------------
		/*!
		@brief	unsigned数値を文字列に変換する（wstring版）
		@param[in]	num	unsigned数値
		@param[in]	Modify = NumModify::Dec	進数
		@return	変換した文字列
		*/
		//--------------------------------------------------------------------------------------
		static wstring UintToWStr(UINT num, NumModify Modify = NumModify::Dec) {
			//返す文字列
			wstring str;
			//書式を整えるストリーム
			wostringstream stream;
			//表示形式を決める
			switch (Modify) {
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	size_t数値を文字列に変換する（wstring版）(UINT)にキャストするだけなので、32ビットを超える場合は注意が必要
		@param[in]	num	unsigned数値
		@param[in]	Modify = NumModify::Dec	進数
		@return	変換した文字列
		*/
		//--------------------------------------------------------------------------------------
		static wstring SizeTToWStr(size_t num, NumModify Modify = NumModify::Dec) {
			//返す文字列
			wstring str;
			//書式を整えるストリーム
			wostringstream stream;
			//表示形式を決める
			switch (Modify) {
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	unsigned数値を文字列に変換する（string版）
		@param[in]	num	unsigned数値
		@param[in]	Modify = NumModify::Dec	進数
		@return	変換した文字列
		*/
		//--------------------------------------------------------------------------------------
		static string UintToStr(UINT num, NumModify Modify = NumModify::Dec) {
			//返す文字列
			string str;
			//書式を整えるストリーム
			ostringstream stream;
			//表示形式を決める
			switch (Modify) {
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	unsigned数値を文字列に変換する（string版）(UINT)にキャストするだけなので、32ビットを超える場合は注意が必要
		@param[in]	num	unsigned数値
		@param[in]	Modify = NumModify::Dec	進数
		@return	変換した文字列
		*/
		//--------------------------------------------------------------------------------------
		static string SizeTToStr(UINT num, NumModify Modify = NumModify::Dec) {
			return UintToStr((UINT)num, Modify);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	signed数値を文字列に変換する（wstring版）
		@param[in]	num	signed数値
		@param[in]	Modify = NumModify::Dec	進数
		@return	変換した文字列
		*/
		//--------------------------------------------------------------------------------------
		static wstring IntToWStr(int num, NumModify Modify = NumModify::Dec) {
			//返す文字列
			wstring str;
			//書式を整えるストリーム
			wostringstream stream;
			//表示形式を決める
			switch (Modify) {
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	signed数値を文字列に変換する（string版）
		@param[in]	num	signed数値
		@param[in]	Modify = NumModify::Dec	進数
		@return	変換した文字列
		*/
		//--------------------------------------------------------------------------------------
		static string IntToStr(int num, NumModify Modify = NumModify::Dec) {
			//返す文字列
			string str;
			//書式を整えるストリーム
			ostringstream stream;
			//表示形式を決める
			switch (Modify) {
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	Probability分の1の確率で、trueを返す<br />
		Probabilityは0より大きくなければならない（0除算はできない）
		@param[in]	Probability	確率。何分の1かを指定する
		@return	Probability分の1の確率になればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool DivProbability(UINT Probability) {
			if (Probability <= 0) {
				return false;
			}
			double Prob = (double)rand() / (double)RAND_MAX;
			double Seed = 1.0 / (double)Probability;
			if (Prob <= Seed) {
				return true;
			}
			return false;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	0から1.0fの間の乱数を返す<br />
		ZeroOKがfalseの場合、0が返る可能性はない（0.00001fになる）
		@param[in]	ZeroOK = false	0が返っていいかどうかの指定
		@return	0から1.0fの間の乱数
		*/
		//--------------------------------------------------------------------------------------
		static float RandZeroToOne(bool ZeroOK = false) {
			double Prob = (double)rand() / (double)RAND_MAX;
			if (Prob <= 0) {
				if (!ZeroOK) {
					Prob = 0.00001;
				}
				else {
					Prob = 0.0;
				}
			}
			return (float)Prob;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	切り上げ
		@param[in]	dSrc	元の数値
		@param[in]	iLen	小数点以下の桁数
		@return	処理後の数値
		*/
		//--------------------------------------------------------------------------------------
		static double Ceil(double dSrc, int iLen)
		{
			double	dRet;

			dRet = dSrc * pow(10.0, iLen);
			dRet = (double)(int)(dRet + 0.9);

			return dRet * pow(10.0, -iLen);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	切り下げ
		@param[in]	dSrc	元の数値
		@param[in]	iLen	小数点以下の桁数
		@return	処理後の数値
		*/
		//--------------------------------------------------------------------------------------
		static double Floor(double dSrc, int iLen)
		{
			double dRet;

			dRet = dSrc * pow(10.0, iLen);
			dRet = (double)(int)(dRet);

			return dRet * pow(10.0, -iLen);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	四捨五入
		@param[in]	dSrc	元の数値
		@param[in]	iLen	小数点以下の桁数
		@return	処理後の数値
		*/
		//--------------------------------------------------------------------------------------
		static double Round(double dSrc, int iLen)
		{
			double	dRet;

			dRet = dSrc * pow(10.0, iLen);
			dRet = (double)(int)(dRet + 0.5);

			return dRet * pow(10.0, -iLen);
		}

    };

	//--------------------------------------------------------------------------------------
	///	Objectインターフェイス
	//--------------------------------------------------------------------------------------
	class ObjectInterface {
		friend class ObjectFactory;
		//クリエイト済みかどうか
		//Create関数が呼び出し後にtrueになる
		bool m_Created{ false };
		void SetCreated(bool b) {
			m_Created = b;
		}
	protected:
		ObjectInterface() {}
		virtual ~ObjectInterface() {}
	public:
		template<typename T>
		std::shared_ptr<T> GetThis() {
			auto Ptr = dynamic_pointer_cast<T>(shared_from_this());
			if (Ptr) {
				return Ptr;
			}
			else {
				wstring str(L"thisを");
				str += Util::GetWSTypeName<T>();
				str += L"型にキャストできません";
				throw BaseException(
					str,
					L"if( ! dynamic_pointer_cast<T>(shared_from_this()) )",
					L"ObjectInterface::GetThis()"
				);
			}
			return nullptr;
		}
		bool IsCreated()const {
			return m_Created;
		}

		virtual void OnInit() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
		virtual void OnDestroy() = 0;
		virtual void OnKeyDown(UINT8 /*key*/) {}
		virtual void OnKeyUp(UINT8 /*key*/) {}
	};


	//--------------------------------------------------------------------------------------
	///	Objectを構築する
	//--------------------------------------------------------------------------------------
	class ObjectFactory {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief オブジェクト作成（static関数）
		@tparam T	作成する型
		@tparam Ts...	可変長パラメータ型
		@param[in]	params	可変長パラメータ
		@return　作成したオブジェクトのshared_ptr
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		static shared_ptr<T> Create(Ts&&... params) {
			shared_ptr<T> Ptr = shared_ptr<T>(new T(params...));
			//初期化関数呼び出し
			Ptr->OnInit();
			Ptr->SetCreated(true);
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief オブジェクト作成（static関数）。パラメータはOnInitに渡される
		@tparam T	作成する型
		@tparam Ts...	可変長パラメータ型
		@param[in]	params	可変長パラメータ
		@return　作成したオブジェクトのshared_ptr
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		static shared_ptr<T> CreateInitParam(Ts&&... params) {
			shared_ptr<T> Ptr = shared_ptr<T>(new T());
			//初期化関数呼び出し
			Ptr->OnInit(params...);
			Ptr->SetCreated(true);
			return Ptr;
		}

	};


    //--------------------------------------------------------------------------------------
    ///	シェーダ関連リソースのインターフェイス
    //--------------------------------------------------------------------------------------
    class Dx12ShaderResource {
    protected:
        Dx12ShaderResource();
        virtual ~Dx12ShaderResource();
        //シェーダアクセサ
        ID3DBlob* GetShaderBlob(const wstring& Filename, ComPtr<ID3DBlob>& ShaderComPtr);
        ComPtr<ID3DBlob>& GetShaderBlobComPtr(const wstring& Filename, ComPtr<ID3DBlob>& ShaderComPtr);
    private:
        //コピー禁止
        Dx12ShaderResource(const Dx12ShaderResource&) = delete;
        Dx12ShaderResource& operator=(const Dx12ShaderResource&) = delete;
        //ムーブ禁止
        Dx12ShaderResource(const Dx12ShaderResource&&) = delete;
        Dx12ShaderResource& operator=(const Dx12ShaderResource&&) = delete;
    };

    //--------------------------------------------------------------------------------------
    ///	シングルトンシェーダの親クラス(シェーダの種類にかかわらず同じクラス使用)
    //--------------------------------------------------------------------------------------
    template<typename ShaderType>
    class Dx12Shader : public Dx12ShaderResource {
        wstring m_Filename;
    protected:
        //デリーター
        struct Deleter
        {
            void operator()(ShaderType* p) { delete p; }
        };
        ComPtr<ID3DBlob> m_ShaderPtr;
        //構築と破棄
        Dx12Shader<ShaderType>(const wstring& Filename) : Dx12ShaderResource(), m_Filename(Filename) {}
        virtual ~Dx12Shader() {}
        static unique_ptr<ShaderType, Deleter> m_Ptr;
    public:
        //シェーダ公開アクセッサ
        ID3DBlob* GetShader() {
            return GetShaderBlob(m_Filename, m_ShaderPtr);
        }
        ComPtr<ID3DBlob>& GetShaderComPtr() {
            return GetShaderBlobComPtr(m_Filename, m_ShaderPtr);
        }
        //インスタンス取得
        static unique_ptr<ShaderType, Deleter>& GetPtr() {
            if (!m_Ptr) {
                m_Ptr.reset(new ShaderType());
            }
            return m_Ptr;
        }
    };


    //これより個別シェーダー
    //シェーダ宣言用マクロ
#define DECLARE_DX12SHADER(ShaderName) class ShaderName : public Dx12Shader<ShaderName>{ \
	public: \
		ShaderName(); \
	};
    //シェーダ実体用マクロ
#define IMPLEMENT_DX12SHADER(ShaderName,CsoFilename) unique_ptr<ShaderName, ShaderName::Deleter> ShaderName::m_Ptr; \
	ShaderName::ShaderName() : \
	Dx12Shader(CsoFilename){}

}
//end basedx12

