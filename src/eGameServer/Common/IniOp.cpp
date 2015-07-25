
#include "stdafx.h"
#include "IniOp.h"
#include "ModulePath.h"
#include <tchar.h>
#include "tstring.h"

UINT INI_GetInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT nDefault, LPCTSTR lpFileName)
{
	tstring strFileName = GetModulePath() + lpFileName;
	return GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, strFileName.c_str());
}

DWORD INI_GetStringA(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault, LPSTR lpReturnedString, DWORD dwSize, LPCSTR lpFileName)
{
	string strFileName = GetModulePathA() + lpFileName;
	return GetPrivateProfileStringA(lpAppName, lpKeyName, lpDefault, lpReturnedString, dwSize, strFileName.c_str());
}

BOOL INI_WriteStringA(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpString, LPCSTR lpFileName)
{
	string strFileName = GetModulePathA() + lpFileName;
	return WritePrivateProfileStringA(lpAppName, lpKeyName, lpString, strFileName.c_str());
}

DWORD INI_GetStringW(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD dwSize, LPCTSTR lpFileName)
{
	tstring strFileName = GetModulePath() + lpFileName;
	return GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, lpReturnedString, dwSize, strFileName.c_str());
}

BOOL INI_WriteStringW(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString, LPCTSTR lpFileName)
{
	tstring strFileName = GetModulePath() + lpFileName;
	return WritePrivateProfileString(lpAppName, lpKeyName, lpString, strFileName.c_str());
}
