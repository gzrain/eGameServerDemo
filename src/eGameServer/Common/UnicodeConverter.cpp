#include "UnicodeConverter.h"
#include <windows.h>
#include <tchar.h>


void unicodeToUTF8(const wstring &src, string& result)
{
	int n = WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, 0, 0, 0, 0);
	CHAR *pstrResult = new CHAR[n];
	memset(pstrResult, 0, n);
	::WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, (char*)pstrResult, n, 0, 0);

	result = pstrResult;
	delete[]pstrResult;
}

void unicodeToGB2312(const wstring& wstr, string& result)
{
	int n = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, 0, 0, 0, 0);
	CHAR *pstrResult = new CHAR[n];
	memset(pstrResult, 0, n);
	::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, (char*)pstrResult, n, 0, 0);

	result = pstrResult;
	delete[]pstrResult;
}

void utf8ToUnicode(const string& src, wstring& result)
{
	int n = MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, NULL, 0);
	WCHAR *pstrResult = new WCHAR[n];
	memset(pstrResult,0,sizeof(WCHAR)*n);
	::MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, (LPWSTR)pstrResult, n);

	result = pstrResult;
	delete[]pstrResult;

}

void gb2312ToUnicode(const string& src, wstring& result)
{
	int n = MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0);
	WCHAR *pstrResult = new WCHAR[n];
	memset(pstrResult, 0, sizeof(WCHAR)*n);
	::MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, (LPWSTR)pstrResult, n);

	result = pstrResult;
	delete[]pstrResult;
}

void gb2312ToUTF8(const string &src, string& result)
{
	wstring strUnicode;
	gb2312ToUnicode(src, strUnicode);
	unicodeToUTF8(strUnicode, result);
}

void utf8ToGB2312(const string &src, string& result)
{
	wstring strUnicode;
	utf8ToUnicode(src, strUnicode);
	unicodeToGB2312(strUnicode, result);
}
